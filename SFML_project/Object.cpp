#include "Object.h"
#include "Button.h"

extern double const epsilon;
extern RenderWindow  window;
extern View view;
extern Menu mainMenu;

Plane* Plane::plane = nullptr;
//Color Plane::color = Color(200, 200, 200);
Plane::Plane()
{
	/*Shape.setOutlineThickness(OutlineThickness);
	Shape.setFillColor(color);
	Shape.setOutlineColor(Color::Black);*/
}

Plane* Plane::getInstance()
{
	if (plane == nullptr) {
		plane = new Plane();
	}
	return plane;
}
/*
double Plane::getDistance(Vector2f point)
{
	return NULL;
}

bool Plane::isNearby(Vector2f mouseCoord)
{
	Vector2i MouseCoords = Mouse::getPosition(window);
	Vector2f Offset = getOffset(view);
	return Shape.getGlobalBounds().contains(MouseCoords.x + Offset.x, MouseCoords.y + Offset.y);
}


void Plane::draw()
{
	//mainMenu.update();
	Vector2f position = view.getCenter() + view.getSize();
	position.x = view.getCenter().x - (view.getSize().x / 2);
	position.y = view.getCenter().y - (view.getSize().y / 2);
	Shape.setPosition(position);
	Shape.setSize(view.getSize());
	window.draw(Shape);
}

Color Plane::getColor()
{
	return color;
}
*/

UnitCircle* UnitCircle::unitCircle = nullptr;
UnitCircle::UnitCircle()
{
	Shape.setPosition(Vector2f(0, 0));
	Shape.setRadius(unitSeg);
	Shape.setOrigin(unitSeg, unitSeg);
	Shape.setOutlineThickness(unitSeg / 50);
	//Shape.setFillColor(Plane::getColor());
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

bool UnitCircle::isNearby(Vector2f mouseCoord)
{
	return getDistance(mouseCoord) < epsilon;
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
	double x = abs((A * point.x + B * point.y + C) / sqrt(A * A + B * B));
	return x;
}
bool Line::isNearby(Vector2f mouseCoord)
{
	return distance(mouseCoord) < epsilon;
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
/*
Line::equationLine Line::getEquation()
{
	return lineEq;
}
*/


double Point::distance(Vector2f Point)
{
	Vector2f coord = shape.getPosition();
	return sqrt(pow((coord.x - Point.x), 2) + pow((coord.y - Point.y), 2));
}

/*Vector2f Point::intersectLines(Line::equationLine first, Line::equationLine second)
{

	Vector2f pointCoord = Vector2f(
		(first.B * second.C - first.C * second.B)
		/ (first.A * second.B - first.B * second.A),
		(first.C * second.A - first.A * second.C)
		/ (first.A * second.B - first.B * second.A)
	);
	return pointCoord;
}*/

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

Point::Point(Vector2f mouseCoord)
{
	construction = new ByComplexScalar(new ComplexScalar(mouseCoord));
	Init();
}

Point::Point(Line* first, Line* second)
{
	construction = new IntersectionOfTwoLines(first, second);
	Init();
}
/*
Point::Point(Line* line, Vector2f mouseCoord)
{
	shape.setOrigin(pointSize, pointSize);
	Line::equationLine equation = line->getEquation();
	Line::equationLine perpendicular = equation;
	swap(perpendicular.A, perpendicular.B);
	perpendicular.B *= -1;
	perpendicular.C = -(perpendicular.A * mouseCoord.x + perpendicular.B * mouseCoord.y);
	shape.setPosition(intersectLines(perpendicular, equation));
	shape.setFillColor(Color::Black);
	//complex coord
}*/

Point::Point(Line* line, Point* point)
{
	shape.setOrigin(pointSize, pointSize);
	try
	{
		if (true)
		{

		}
	}
	catch (int e)
	{

	}
}

Point::Point(UnitCircle* unitCircle, Vector2f mouseCoord)
{
	shape.setFillColor(Color::Black);
	shape.setOrigin(pointSize, pointSize);
	Vector2f coord = mouseCoord
		/ sqrt(mouseCoord.x * mouseCoord.x + mouseCoord.y * mouseCoord.y) * (float)unitSeg;
	shape.setPosition(coord);
}

bool Point::isNearby(Vector2f mouseCoord)
{
	return distance(mouseCoord) < epsilon;
}

void Point::draw()
{
	window.draw(shape);
}

void Point::drawDescription()
{

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

ByComplexScalar::ByComplexScalar(ComplexScalar* ComplexScalar)
	:parent(ComplexScalar)
{
}

Equation* ByComplexScalar::recreate()
{
	ComplexScalarEquation* equation = dynamic_cast<ComplexScalarEquation*>(parent->getEquation());
	return new PointEquation((*equation).point);
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
		((*firstEquation).B * (*secondEquation).C - (*firstEquation).C * (*secondEquation).B)
		/ ((*firstEquation).A * (*secondEquation).B - (*firstEquation).B * (*secondEquation).A),
		((*firstEquation).C * (*secondEquation).A - (*firstEquation).A * (*secondEquation).C)
		/ ((*firstEquation).A * (*secondEquation).B - (*firstEquation).B * (*secondEquation).A)
	);

	return new PointEquation(pointCoord);
}

Equation* ByTwoPointsAndScalar::recreate()
{
	PointEquation* firstEquation = dynamic_cast<PointEquation*>(firstParent->getEquation());
	PointEquation* secondEquation = dynamic_cast<PointEquation*>(secondParent->getEquation());
	ScalarEquation* thirdEquation = dynamic_cast<ScalarEquation*>(firstParent->getEquation());
	Vector2f firstCoord = (*firstEquation).point;
	Vector2f secondCoord = (*secondEquation).point;
	float ratio = (*thirdEquation).ratio;
	Vector2f pointCoord = (secondCoord * ratio + firstCoord * 1.f) / (ratio + 1.f);
	return new PointEquation(pointCoord);
}

Equation* Pole::recreate()
{
	return new Equation;
}

ByTwoPoints::ByTwoPoints(Point* _firstParent, Point* _secondParent)
	:firstParent(_firstParent), secondParent(_secondParent)
{
}

Equation* ByTwoPoints::recreate()
{
	PointEquation* firstEquation = dynamic_cast<PointEquation*>(firstParent->getEquation());
	PointEquation* secondEquation = dynamic_cast<PointEquation*>(secondParent->getEquation());
	Vector2f firstCoord = (*firstEquation).point;
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

Perpendicular::Perpendicular(Point* _firstParent, Line* _secondParent)
	:firstParent(_firstParent), secondParent(_secondParent)
{
}

Equation* Perpendicular::recreate()
{
	PointEquation* firstEquation = dynamic_cast<PointEquation*>(firstParent->getEquation());
	LineEquation* secondEquation = dynamic_cast<LineEquation*>(secondParent->getEquation());
	Vector2f coord = (*firstEquation).point;
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

Equation* Tangent::recreate()
{
	return new Equation;
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

Equation* Object::getEquation()
{
	return equation;
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

LineEquation::LineEquation(double _A, double _B, double _C)
	:A(_A), B(_B), C(_C)
{
}

PointEquation::PointEquation(Vector2f _point)
	: point(_point)
{
}

Equation::~Equation()
{
}

ComplexScalar::ComplexScalar(Vector2f coord)
{
	equation = new ComplexScalarEquation(coord);
}

ComplexScalarEquation::ComplexScalarEquation(Vector2f _point)
	:point(_point)
{
}
