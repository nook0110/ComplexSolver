#include "Object.h"
#include "Button.h"

extern double const epsilon;
extern RenderWindow  window;
extern View view;
extern Menu mainMenu;

Equation* Object::getEquation()
{
	return equation;
}

Object::~Object()
{
}

void Object::deleteChildren()
{
	for (auto child : children)
		delete child;
}

void Object::eraseChild(Object* child)
{
	children.remove(child);
}

void Object::addChild(Object* child)
{
	children.push_back(child);
}

void VisibleObject::insert()
{
	ConstructionData::allVisibleObjects.push_back(this);
	it = prev(ConstructionData::allVisibleObjects.end());
	Creation::Create();
}

void VisibleObject::erase()
{
	ConstructionData::allVisibleObjects.erase(it);
}

bool VisibleObject::isOnCircle()
{
	return dynamic_cast<ByCircleAndScalar*>(construction);
}




Plane* Plane::plane = nullptr;
Plane::Plane()
{
}

Plane* Plane::getInstance()
{
	if (plane == nullptr) {
		plane = new Plane();
	}
	return plane;
}


UnitCircle* UnitCircle::unitCircle = nullptr;
UnitCircle::UnitCircle()
{
	Shape.setPointCount(100);
	Shape.setPosition(Vector2f(0, 0));
	Shape.setOrigin(unitSeg - outlineThickness / 2, unitSeg - outlineThickness / 2);
	Shape.setOutlineThickness(outlineThickness);
	Shape.setRadius(unitSeg - outlineThickness / 2);
	Shape.setOutlineColor(Color::Black);
}

UnitCircle* UnitCircle::getInstance()
{
	if (unitCircle == nullptr) {
		unitCircle = new UnitCircle();
	}
	return unitCircle;
}

double UnitCircle::getDistance(Vector2f point)
{
	Vector2f center = Shape.getPosition();
	double radius = Shape.getRadius();
	double distanceToCenter = sqrt(
		pow(point.x - 0, 2) +
		pow(point.y - 0, 2));
	return abs(distanceToCenter - radius);
}

bool UnitCircle::isNearby(Vector2f mousePosition)
{
	return getDistance(mousePosition) < epsilon;
}

void UnitCircle::draw()
{
	window.draw(Shape);
}

double Line::distance(Vector2f point)
{
	LineEquation* lineEquation = dynamic_cast<LineEquation*>(getEquation());
	double A = (*lineEquation).A;
	double B = (*lineEquation).B;
	double C = (*lineEquation).C;
	return abs((A * point.x + B * point.y + C) / sqrt(A * A + B * B));
}

bool Line::isNearby(Vector2f mousePosition)
{
	return distance(mousePosition) < epsilon;
}

void Line::draw()
{
	LineEquation lineEq = *(dynamic_cast<LineEquation*>(getEquation()));
	if (lineEq.A == 0)
	{
		double x1 = view.getCenter().x - (view.getSize().x / 2);
		double x2 = view.getCenter().x + (view.getSize().x / 2);
		double y1 = -(x1 * lineEq.A + lineEq.C) / lineEq.B;
		double y2 = -(x2 * lineEq.A + lineEq.C) / lineEq.B;
		sf::Vertex line[] =
		{
			sf::Vertex(sf::Vector2f(x1, y1), Color::Black),
			sf::Vertex(sf::Vector2f(x2, y2), Color::Black)
		};
		window.draw(line, 2, sf::Lines);
	}
	else if (lineEq.B == 0)
	{
		double y1 = view.getCenter().y - (view.getSize().y / 2);
		double y2 = view.getCenter().y + (view.getSize().y / 2);
		double x1 = -(y1 * lineEq.B + lineEq.C) / lineEq.A;
		double x2 = -(y2 * lineEq.B + lineEq.C) / lineEq.A;
		sf::Vertex line[] =
		{
			sf::Vertex(sf::Vector2f(x1, y1), Color::Black),
			sf::Vertex(sf::Vector2f(x2, y2), Color::Black)
		};
		window.draw(line, 2, sf::Lines);
	}
	else
	{
		if (abs(lineEq.A / lineEq.B) > view.getSize().y / view.getSize().x)
		{
			double y1 = view.getCenter().y - (view.getSize().y / 2);
			double y2 = view.getCenter().y + (view.getSize().y / 2);
			double x1 = -(y1 * lineEq.B + lineEq.C) / lineEq.A;
			double x2 = -(y2 * lineEq.B + lineEq.C) / lineEq.A;
			sf::Vertex line[] =
			{
				sf::Vertex(sf::Vector2f(x1, y1), Color::Black),
				sf::Vertex(sf::Vector2f(x2, y2), Color::Black)
			};
			window.draw(line, 2, sf::Lines);
		}
		else
		{
			double x1 = view.getCenter().x - (view.getSize().x / 2);
			double x2 = view.getCenter().x + (view.getSize().x / 2);
			double y1 = -(x1 * lineEq.A + lineEq.C) / lineEq.B;
			double y2 = -(x2 * lineEq.A + lineEq.C) / lineEq.B;
			sf::Vertex line[] =
			{
				sf::Vertex(sf::Vector2f(x1, y1), Color::Black),
				sf::Vertex(sf::Vector2f(x2, y2), Color::Black)
			};
			window.draw(line, 2, sf::Lines);
		}
	}
}

void Line::drawDescription()
{
}

Line::Line(UnitCircle* first, Point* second)
{
	construction = new Tangent(first, second);
	equation = construction->recreate();
}

Line::Line(Point* first, Point* second)
{
	construction = new ByTwoPoints(first, second);
	equation = construction->recreate();
}

Line::Line(Point* first, Line* second)
{
	construction = new Perpendicular(first, second);
	equation = construction->recreate();
}

double Point::distance(Vector2f Point)
{
	Vector2f coord = shape.getPosition();
	return sqrt(pow((coord.x - Point.x), 2) + pow((coord.y - Point.y), 2));
}

Vector2f Point::getCoordinate()
{
	return shape.getPosition();
}

void Point::Init()
{
	equation = construction->recreate();
	shape.setOrigin(pointSize, pointSize);
	PointEquation* pointEquation = dynamic_cast<PointEquation*>(construction->recreate());
	Vector2f position = (*pointEquation).point;
	shape.setPosition(position);
	shape.setFillColor(Color::Black);
}

Point::Point(Vector2f mousePosition)
{
	auto parent = new ComplexScalar(mousePosition);
	construction = new ByComplexScalar(parent);
	parent->addChild(this);
	Init();
}

Point::Point(Line* first, Line* second)
{
	construction = new IntersectionOfTwoLines(first, second);
	first->addChild(this);
	second->addChild(this);
	Init();
}

Point::Point(Line* line, Vector2f mousePosition)
{
	auto scalar = new Scalar(0.0);
	construction = new ByLineAndScalar(line, scalar);
	line->addChild(this);
	scalar->addChild(this);
	Init();
	//complex coord*/
}

Point::Point(Line* line, Point* point)
{
	throw std::runtime_error("Constructor uncompleted");
}

Point::Point(UnitCircle* unitCircle, Vector2f mousePosition)
{
	auto scalar = new Scalar(atan2(mousePosition.y, mousePosition.x));
	construction = new ByCircleAndScalar(unitCircle, scalar);
	unitCircle->addChild(this);
	scalar->addChild(this);
	Init();
}

bool Point::isNearby(Vector2f mousePosition)
{
	return distance(mousePosition) < epsilon;
}

void Point::draw()
{
	window.draw(shape);
}

void Point::drawDescription()
{
}

ByComplexScalar::ByComplexScalar(ComplexScalar* ComplexScalar)
	:parent(ComplexScalar)
{
}

Equation* ByComplexScalar::recreate()
{
	ComplexScalarEquation* equation = dynamic_cast<ComplexScalarEquation*>(parent->getEquation());
	return new PointEquation((*equation).point);
}

ByComplexScalar::~ByComplexScalar()
{
	parent->eraseChild(object);
}

IntersectionOfTwoLines::IntersectionOfTwoLines(Line* first, Line* second)
	:firstParent(first), secondParent(second)
{
}

Equation* IntersectionOfTwoLines::recreate()
{
	LineEquation* firstEquation = dynamic_cast<LineEquation*>(firstParent->getEquation());
	LineEquation* secondEquation = dynamic_cast<LineEquation*>(secondParent->getEquation());
	Vector2f pointCoord = Vector2f(
		(firstEquation->B * (*secondEquation).C - firstEquation->C * (*secondEquation).B)
		/ (firstEquation->A * (*secondEquation).B - firstEquation->B * (*secondEquation).A),
		(firstEquation->C * (*secondEquation).A - firstEquation->A * (*secondEquation).C)
		/ (firstEquation->A * (*secondEquation).B - firstEquation->B * (*secondEquation).A)
	);
	return new PointEquation(pointCoord);
}

IntersectionOfTwoLines::~IntersectionOfTwoLines()
{
	firstParent->eraseChild(object);
	secondParent->eraseChild(object);
}

ByTwoPointsAndScalar::ByTwoPointsAndScalar(Point* firstParent, Point* secondParent, Scalar* thirdParent)
	:firstParent(firstParent), secondParent(secondParent), thirdParent(thirdParent)
{
}

Equation* ByTwoPointsAndScalar::recreate()
{
	PointEquation* firstEquation = dynamic_cast<PointEquation*>(firstParent->getEquation());
	PointEquation* secondEquation = dynamic_cast<PointEquation*>(secondParent->getEquation());
	ScalarEquation* thirdEquation = dynamic_cast<ScalarEquation*>(firstParent->getEquation());
	Vector2f firstCoord = firstEquation->point;
	Vector2f secondCoord = (*secondEquation).point;
	float ratio = (*thirdEquation).value;
	Vector2f pointCoord = (secondCoord * ratio + firstCoord * 1.f) / (ratio + 1.f);
	return new PointEquation(pointCoord);
}

Equation* Pole::recreate()
{
	return new Equation;
}

ByTwoPoints::ByTwoPoints(Point* firstParent, Point* secondParent)
	:firstParent(firstParent), secondParent(secondParent)
{
}

Equation* ByTwoPoints::recreate()
{
	PointEquation* firstEquation = dynamic_cast<PointEquation*>(firstParent->getEquation());
	PointEquation* secondEquation = dynamic_cast<PointEquation*>(secondParent->getEquation());
	Vector2f firstCoord = firstEquation->point;
	Vector2f secondCoord = (*secondEquation).point;
	double A = -firstCoord.y + secondCoord.y;
	double B = -(-firstCoord.x + secondCoord.x);
	double C = firstCoord.x * (firstCoord.y - secondCoord.y) - firstCoord.y * (firstCoord.x - secondCoord.x);
	return new LineEquation(A, B, C);
}

Equation* PerpendicularBisector::recreate()
{
	return new Equation;
}

Perpendicular::Perpendicular(Point* firstParent, Line* secondParent)
	:firstParent(firstParent), secondParent(secondParent)
{
}

Equation* Perpendicular::recreate()
{
	PointEquation* firstEquation = dynamic_cast<PointEquation*>(firstParent->getEquation());
	LineEquation* secondEquation = dynamic_cast<LineEquation*>(secondParent->getEquation());
	Vector2f coord = firstEquation->point;
	double A = -(*secondEquation).B;
	double B = (*secondEquation).A;
	double C = (*secondEquation).B * coord.x - (*secondEquation).A * coord.y;
	return new LineEquation(A, B, C);
}

Equation* Polar::recreate()
{
	return new Equation;
}

Equation* Parallel::recreate()
{
	return new Equation;
}

Tangent::Tangent(UnitCircle* firstParent, Point* secondParent)
	:firstParent(firstParent), secondParent(secondParent)
{
	if (!secondParent->isOnCircle())
		throw invalid_argument("Point isnt on circle?!");
}

Equation* Tangent::recreate()
{
	PointEquation* secondEquation = dynamic_cast<PointEquation*>(secondParent->getEquation());
	Vector2f coord = secondEquation->point;
	double A = coord.x;
	double B = coord.y;
	double C = -(coord.x * coord.x + coord.y * coord.y);
	return new LineEquation(A, B, C);
}

Equation* ConstructionLine::recreate()
{
	return new Equation;
}

Equation* ConstructionPoint::recreate()
{
	return new Equation;
}

Equation* ConstructionData::recreate()
{
	return new Equation;
}

LineEquation::LineEquation(double A, double B, double C)
	:A(A), B(B), C(C)
{
}

PointEquation::PointEquation(Vector2f point)
	: point(point)
{
}

Equation::~Equation()
{
}

Vector2f Equation::Projection(LineEquation lineEquation, PointEquation pointEquation)
{
	double A = lineEquation.A;
	double B = lineEquation.B;
	double C = lineEquation.C;
	double x = pointEquation.point.x;
	double y = pointEquation.point.y;
	return Vector2f(
		(B * B * x - A * (B * y + C)) / (A * A + B * B),
		(A * A * y - B * (A * x + C)) / (A * A + B * B));
}

ComplexScalar::ComplexScalar(Vector2f coord)
{
	equation = new ComplexScalarEquation(coord);
}

ComplexScalarEquation::ComplexScalarEquation(Vector2f point)
	:point(point)
{
}

ByLineAndScalar::ByLineAndScalar(Line* firstParent, Scalar* secondParent)
	: firstParent(firstParent), secondParent(secondParent)
{
}

Equation* ByLineAndScalar::recreate()
{
	LineEquation* lineEquation = dynamic_cast<LineEquation*>(firstParent->getEquation());
	ScalarEquation* scalarEquation = dynamic_cast<ScalarEquation*>(secondParent->getEquation());
	double A = (*lineEquation).A;
	double B = (*lineEquation).B;
	double C = (*lineEquation).C;
	Vector2f direction = Vector2f(-B, A);
	direction /= (float)sqrt(A * A + B * B);
	direction *= (float)(*scalarEquation).value;
	//Vector2f projection = Projection(lineEquation, new PointEquation(Vector2f(0, 0));
	//return new PointEquation(projection + direction);
	return nullptr;
}

Scalar::Scalar(double value)
{
	equation = new ScalarEquation(value);
}

ScalarEquation::ScalarEquation(double value)
	: value(value)
{
}

ByCircleAndScalar::ByCircleAndScalar(UnitCircle* firstParent, Scalar* secondParent)
	: firstParent(firstParent), secondParent(secondParent)
{
}

Equation* ByCircleAndScalar::recreate()
{
	ScalarEquation* scalarEquation = dynamic_cast<ScalarEquation*>(secondParent->getEquation());
	double angle = (*scalarEquation).value;
	return new PointEquation(Vector2f(cos(angle) * unitSeg, sin(angle) * unitSeg));
}