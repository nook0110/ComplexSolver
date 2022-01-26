#include "Object.h"


extern double const epsilon;
extern RenderWindow  window;
extern View view;

Equation* Object::getEquation()
{
	return equation;
}

Object::~Object()
{
	deleteChildren();
}

void Object::reposition()
{
}

void Object::reposeChildren()
{
	reposition();
	for (auto child : children)
	{
		child->reposeChildren();
	}
}

void Object::deleteChildren()
{
	while (!children.empty())
	{
		auto child = *children.begin();
		delete (child);
		child = nullptr;
	}
}

void Object::eraseChild(Object* child)
{
	children.remove(child);
}

void Object::addChild(Object* child)
{
	children.push_back(child);
}

void Object::clearChildren()
{
	children.remove(nullptr);
}

void VisibleObject::reposition()
{
}

Color VisibleObject::getColor()
{
	return visible ? visibleColor : unvisibleColor;
}

void VisibleObject::changeVisibility(bool visibility)
{
	visible = visibility;
}

void VisibleObject::changeVisibility()
{
	visible = !visible;
}

bool VisibleObject::getVisibility()
{
	return visible;
}

void VisibleObject::erase()
{
	Drawer::allVisibleObjects.remove(this);
}

VisibleObject::~VisibleObject()
{
	erase();
	delete construction;
	construction = nullptr;
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
	shape.setPointCount(100);
	shape.setPosition(Vector2f(0, 0));
	shape.setOrigin(unitSeg - outlineThickness / 2, unitSeg - outlineThickness / 2);
	shape.setOutlineThickness(outlineThickness);
	shape.setRadius(unitSeg - outlineThickness / 2);
	shape.setOutlineColor(getColor());
	shape.setFillColor(Color(0, 0, 0, 0));
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
	Vector2f center = shape.getPosition();
	double radius = shape.getRadius();
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
	shape.setOutlineColor(getColor());
	window.draw(shape);
}

double Line::distance(Vector2f point)
{
	LineEquation* lineEquation = dynamic_cast<LineEquation*>(getEquation());
	double A = (*lineEquation).A;
	double B = (*lineEquation).B;
	double C = (*lineEquation).C;
	return abs((A * point.x + B * point.y + C) / sqrt(A * A + B * B));
}

Line::Line(Line* first, Point* second)
{
	first->addChild(this);
	second->addChild(this);
	construction = new Parallel(this, first, second);
	equation = new LineEquation(0, 0, 0);
	reposition();
	Init();
}

bool Line::isNearby(Vector2f mousePosition)
{
	return distance(mousePosition) < epsilon;
}

void Line::draw()
{
	LineEquation lineEq = *(dynamic_cast<LineEquation*>(getEquation()));
	double x1, x2, y1, y2;
	if (lineEq.A == 0)
	{
		x1 = view.getCenter().x - (view.getSize().x / 2);
		x2 = view.getCenter().x + (view.getSize().x / 2);
		y1 = -(x1 * lineEq.A + lineEq.C) / lineEq.B;
		y2 = -(x2 * lineEq.A + lineEq.C) / lineEq.B;

	}
	else if (lineEq.B == 0)
	{
		y1 = view.getCenter().y - (view.getSize().y / 2);
		y2 = view.getCenter().y + (view.getSize().y / 2);
		x1 = -(y1 * lineEq.B + lineEq.C) / lineEq.A;
		x2 = -(y2 * lineEq.B + lineEq.C) / lineEq.A;
	}
	else
	{
		if (abs(lineEq.A / lineEq.B) > view.getSize().y / view.getSize().x)
		{
			y1 = view.getCenter().y - (view.getSize().y / 2);
			y2 = view.getCenter().y + (view.getSize().y / 2);
			x1 = -(y1 * lineEq.B + lineEq.C) / lineEq.A;
			x2 = -(y2 * lineEq.B + lineEq.C) / lineEq.A;
		}
		else
		{
			x1 = view.getCenter().x - (view.getSize().x / 2);
			x2 = view.getCenter().x + (view.getSize().x / 2);
			y1 = -(x1 * lineEq.A + lineEq.C) / lineEq.B;
			y2 = -(x2 * lineEq.A + lineEq.C) / lineEq.B;
		}
	}
	sf::Vertex line[] =
	{
		sf::Vertex(sf::Vector2f(x1, y1), getColor()),
		sf::Vertex(sf::Vector2f(x2, y2), getColor())
	};
	window.draw(line, 2, sf::Lines);
}

void Line::drawDescription()
{
}

void Line::reposition()
{
	construction->recreate(equation);
}

void Line::Init()
{
	Drawer::allVisibleObjects.push_back(this);
}

Line::Line()
{}

Line::Line(UnitCircle* first, Point* second)
{
	first->addChild(this);
	second->addChild(this);
	construction = new Tangent(this, first, second);
	equation = new LineEquation(0, 0, 0);
	reposition();
	Init();
}

Line::Line(Point* first, Point* second)
{
	first->addChild(this);
	second->addChild(this);
	construction = new ByTwoPoints(this, first, second);
	equation = new LineEquation(0, 0, 0);
	reposition();
	Init();
}

Line::Line(Point* first, Line* second)
{
	first->addChild(this);
	second->addChild(this);
	construction = new Perpendicular(this, first, second);
	equation = new LineEquation(0, 0, 0);
	reposition();
	Init();
}

Chord::Chord(UnitPoint* first, UnitPoint* second)
{
	first->addChild(this);
	second->addChild(this);
	construction = new ByTwoPoints(this, first, second);
	equation = new LineEquation(0, 0, 0);
	reposition();
	Init();
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

Point::Point(Point* first, Point* second, Scalar* scalar)
{
	construction = new ByTwoPointsAndScalar(this, first, second, scalar);
	first->addChild(this);
	second->addChild(this);
	scalar->addChild(this);
	Init();
}

void Point::reposition()
{
	construction->recreate(equation);
	PointEquation* pointEquation = dynamic_cast<PointEquation*>(equation);
	Vector2f position = (*pointEquation).point;
	shape.setPosition(position);
}

Point::Point()
{
}

void Point::moveTo(Vector2f coords)
{
	dynamic_cast<ConstructionPoint*>(construction)->moveTo(coords);
	construction->recreate(equation);
	reposeChildren();
}

void Point::Init()
{
	equation = new PointEquation(Vector2f(0, 0));
	shape.setOrigin(pointSize, pointSize);
	shape.setFillColor(Color::Black);
	reposition();
	Drawer::allVisibleObjects.push_back(this);
}

Point::Point(Vector2f mousePosition)
{
	auto parent = new ComplexScalar(mousePosition);
	construction = new ByComplexScalar(this, parent);
	parent->addChild(this);
	Init();
}

Point::Point(Line* first, Line* second)
{
	construction = new IntersectionOfTwoLines(this, first, second);
	first->addChild(this);
	second->addChild(this);
	Init();
}

Point::Point(Line* line, Vector2f mousePosition)
{
	LineEquation* lineEquation = dynamic_cast<LineEquation*>(line->getEquation());
	Vector2f firstProj = Equation::Projection(*lineEquation, PointEquation(mousePosition));
	Vector2f secondProj = Equation::Projection(*lineEquation);
	Vector2f altitude = secondProj - projPoint;
	Vector2f delta = firstProj - secondProj;
	double distance = sqrt(delta.x * delta.x + delta.y * delta.y);
	double crossProduct = altitude.x * delta.y - altitude.y * delta.x;
	int sign = (crossProduct > 0) - (crossProduct < 0);
	Scalar* scalar = new Scalar(distance * sign);
	construction = new ByLineAndScalar(this, line, scalar);
	line->addChild(this);
	scalar->addChild(this);
	Init();
	//complex coord*/
}

Point::Point(Point* point, Line* line)
{
	construction = new Projection(this, point, line);
	point->addChild(this);
	line->addChild(this);
	Init();
}

bool Point::isNearby(Vector2f mousePosition)
{
	return distance(mousePosition) < epsilon;
}

void Point::draw()
{
	shape.setFillColor(getColor());
	window.draw(shape);
}

void Point::drawDescription()
{
}

ByComplexScalar::ByComplexScalar(Object* object, ComplexScalar* ComplexScalar)
	:parent(ComplexScalar)
{
	ConstructionData::object = object;
}


void ByComplexScalar::recreate(Equation* equation)
{
	ComplexScalarEquation* complexScalarequation = dynamic_cast<ComplexScalarEquation*>(parent->getEquation());
	*dynamic_cast<PointEquation*>(equation) = PointEquation((*complexScalarequation).point);
}

void ByComplexScalar::moveTo(Vector2f coords)
{
	*parent = (ComplexScalar(coords));
}

ByComplexScalar::~ByComplexScalar()
{
	parent->eraseChild(object);
	delete parent;
	parent = nullptr;
}

IntersectionOfTwoLines::IntersectionOfTwoLines(Object* object, Line* first, Line* second)
	:firstParent(first), secondParent(second)
{
	ConstructionData::object = object;
}


void IntersectionOfTwoLines::recreate(Equation* equation)
{
	LineEquation* firstEquation = dynamic_cast<LineEquation*>(firstParent->getEquation());
	LineEquation* secondEquation = dynamic_cast<LineEquation*>(secondParent->getEquation());
	Vector2f pointCoord = Vector2f(
		(firstEquation->B * (*secondEquation).C - firstEquation->C * (*secondEquation).B)
		/ (firstEquation->A * (*secondEquation).B - firstEquation->B * (*secondEquation).A),
		(firstEquation->C * (*secondEquation).A - firstEquation->A * (*secondEquation).C)
		/ (firstEquation->A * (*secondEquation).B - firstEquation->B * (*secondEquation).A)
	);
	*dynamic_cast<PointEquation*>(equation) = (PointEquation(pointCoord));
}

void IntersectionOfTwoLines::moveTo(Vector2f coords)
{
}

IntersectionOfTwoLines::~IntersectionOfTwoLines()
{
	firstParent->eraseChild(object);
	secondParent->eraseChild(object);
}



CentralProjection::CentralProjection(Object* object, UnitCircle* first, Point* second, UnitPoint* third)
	:firstParent(first), secondParent(second), thirdParent(third)
{
	ConstructionData::object = object;
}

void CentralProjection::recreate(Equation* equation)
{
	PointEquation* firstEquation = dynamic_cast<PointEquation*>(secondParent->getEquation());
	PointEquation* secondEquation = dynamic_cast<PointEquation*>(thirdParent->getEquation());
	Vector2f firstCoord = firstEquation->point;
	Vector2f secondCoord = (*secondEquation).point;
	double A = -firstCoord.y + secondCoord.y;
	double B = -(-firstCoord.x + secondCoord.x);
	double C = firstCoord.x * (firstCoord.y - secondCoord.y) - firstCoord.y * (firstCoord.x - secondCoord.x);
	LineEquation lineEquation(A, B, C);
	Vector2f projection = Equation::Projection(lineEquation, PointEquation(Vector2f(0, 0)));
	Vector2f delta = projection - thirdParent->getCoordinate();
	*dynamic_cast<PointEquation*>(equation) = PointEquation(projection + delta);
}

void CentralProjection::moveTo(Vector2f coords)
{
}

CentralProjection::~CentralProjection()
{
	firstParent->eraseChild(object);
	secondParent->eraseChild(object);
	thirdParent->eraseChild(object);
}


ByTwoPointsAndScalar::ByTwoPointsAndScalar(Object* object, Point* firstParent, Point* secondParent, Scalar* thirdParent)
	:firstParent(firstParent), secondParent(secondParent), thirdParent(thirdParent)
{
	ConstructionData::object = object;
}

ByTwoPointsAndScalar::~ByTwoPointsAndScalar()
{
	firstParent->eraseChild(object);
	secondParent->eraseChild(object);
	thirdParent->eraseChild(object);
	delete thirdParent;
	thirdParent = nullptr;
}


void ByTwoPointsAndScalar::recreate(Equation* equation)
{
	PointEquation* firstEquation = dynamic_cast<PointEquation*>(firstParent->getEquation());
	PointEquation* secondEquation = dynamic_cast<PointEquation*>(secondParent->getEquation());
	ScalarEquation* thirdEquation = dynamic_cast<ScalarEquation*>(thirdParent->getEquation());
	Vector2f firstCoord = firstEquation->point;
	Vector2f secondCoord = (*secondEquation).point;
	float ratio = (*thirdEquation).value;
	Vector2f pointCoord = (secondCoord * ratio + firstCoord * 1.f) / (ratio + 1.f);
	*dynamic_cast<PointEquation*>(equation) = PointEquation(pointCoord);
}

void Pole::recreate(Equation* equation)
{
	return;
}

ByTwoPoints::ByTwoPoints(Object* object, Point* firstParent, Point* secondParent)
	:firstParent(firstParent), secondParent(secondParent)
{
	ConstructionData::object = object;
}

void ByTwoPoints::recreate(Equation* equation)
{
	if (!equation)
	{
		equation = new LineEquation(0, 0, 0);
	}
	PointEquation* firstEquation = dynamic_cast<PointEquation*>(firstParent->getEquation());
	PointEquation* secondEquation = dynamic_cast<PointEquation*>(secondParent->getEquation());
	Vector2f firstCoord = firstEquation->point;
	Vector2f secondCoord = (*secondEquation).point;
	double A = -firstCoord.y + secondCoord.y;
	double B = -(-firstCoord.x + secondCoord.x);
	double C = firstCoord.x * (firstCoord.y - secondCoord.y) - firstCoord.y * (firstCoord.x - secondCoord.x);
	*dynamic_cast<LineEquation*>(equation) = LineEquation(A, B, C);
}

ByTwoPoints::~ByTwoPoints()
{
	firstParent->eraseChild(object);
	secondParent->eraseChild(object);
}

ByTwoUnitPoints::ByTwoUnitPoints(Object* object, UnitPoint* firstParent, UnitPoint* secondParent) : ByTwoPoints(object, firstParent, secondParent)
{
}

void PerpendicularBisector::recreate(Equation* equation)
{
	return;
}

Perpendicular::Perpendicular(Object* object, Point* firstParent, Line* secondParent)
	:firstParent(firstParent), secondParent(secondParent)
{
	ConstructionData::object = object;
}

Perpendicular::~Perpendicular()
{
	firstParent->eraseChild(object);
	secondParent->eraseChild(object);
}

void Perpendicular::recreate(Equation* equation)
{
	PointEquation* firstEquation = dynamic_cast<PointEquation*>(firstParent->getEquation());
	LineEquation* secondEquation = dynamic_cast<LineEquation*>(secondParent->getEquation());
	Vector2f coord = firstEquation->point;
	double A = -(*secondEquation).B;
	double B = (*secondEquation).A;
	double C = (*secondEquation).B * coord.x - (*secondEquation).A * coord.y;
	*dynamic_cast<LineEquation*>(equation) = LineEquation(A, B, C);
}

void Polar::recreate(Equation* equation)
{
	return;
}

Parallel::Parallel(Object* object, Line* first, Point* second)
	:firstParent(first), secondParent(second)
{
	ConstructionData::object = object;
}

Parallel::~Parallel()
{
	firstParent->eraseChild(object);
	secondParent->eraseChild(object);
}

void Parallel::recreate(Equation* equation)
{
	LineEquation* lineEquation = dynamic_cast<LineEquation*>(firstParent->getEquation());
	PointEquation* pointEquation = dynamic_cast<PointEquation*>(secondParent->getEquation());
	Vector2f coord = pointEquation->point;
	double A = (*lineEquation).A;
	double B = (*lineEquation).B;
	double C = -(*lineEquation).A * coord.x - (*lineEquation).B * coord.y;
	*dynamic_cast<LineEquation*>(equation) = LineEquation(A, B, C);
}

Tangent::Tangent(Object* object, UnitCircle* firstParent, Point* secondParent)
	:firstParent(firstParent), secondParent(secondParent)
{
	if (!secondParent->isOnCircle())
		throw invalid_argument("Point isnt on circle?!");
	ConstructionData::object = object;
}

Tangent::~Tangent()
{
	firstParent->eraseChild(object);
	secondParent->eraseChild(object);
}

void Tangent::recreate(Equation* equation)
{
	PointEquation* secondEquation = dynamic_cast<PointEquation*>(secondParent->getEquation());
	Vector2f coord = secondEquation->point;
	double A = coord.x;
	double B = coord.y;
	double C = -(coord.x * coord.x + coord.y * coord.y);
	*dynamic_cast<LineEquation*>(equation) = LineEquation(A, B, C);
}


void ConstructionLine::recreate(Equation*)
{
}

ConstructionLine::~ConstructionLine()
{
}



void ConstructionPoint::recreate(Equation*)
{
}

void ConstructionPoint::moveTo(Vector2f coords)
{
}



void ConstructionData::recreate(Equation*)
{
}

ConstructionData::~ConstructionData()
{
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

Equation::Equation()
{
}

Vector2f Equation::Projection(LineEquation lineEquation, PointEquation pointEquation)
{
	double A = lineEquation.A;
	double B = lineEquation.B;
	double C = lineEquation.C;
	double x = pointEquation.point.x;
	double y = pointEquation.point.y;
	Vector2f point(
		(B * B * x - A * (B * y + C)) / (A * A + B * B),
		(A * A * y - B * (A * x + C)) / (A * A + B * B));
	return point;
}

Vector2f Equation::Projection(LineEquation lineEquation)
{
	double A = lineEquation.A;
	double B = lineEquation.B;
	double C = lineEquation.C;
	double x = projPoint.x;
	double y = projPoint.y;
	Vector2f point(
		(B * B * x - A * (B * y + C)) / (A * A + B * B),
		(A * A * y - B * (A * x + C)) / (A * A + B * B));
	return point;
}

ComplexScalar::ComplexScalar(Vector2f coord)
{
	equation = new ComplexScalarEquation(coord);
}

ComplexScalar& ComplexScalar::operator+=(Vector2f delta)
{
	dynamic_cast<ComplexScalarEquation*>(equation)->operator+=(delta);
	return *this;
}

ComplexScalarEquation::ComplexScalarEquation(Vector2f point)
	:point(point)
{
}

ComplexScalarEquation& ComplexScalarEquation::operator+=(Vector2f delta)
{
	point += delta;
	return *this;
}

void ByLineAndScalar::checkDirectionSign(float A, float B, float C)
{
	double yProjPoint = (-C - A * projPoint.x) / B;
	double deltaY = -(yProjPoint - projPoint.y);
	int signDeltaY = (deltaY > 0) - (deltaY < 0);
	if (signDeltaY != lastSignDeltaY)
	{
		if (abs(deltaY) < 100)
		{
			directionSign *= -1.f;
		}
		lastSignDeltaY = signDeltaY;
	}
}

ByLineAndScalar::ByLineAndScalar(Object* object, Line* firstParent, Scalar* secondParent)
	: firstParent(firstParent), secondParent(secondParent)
{
	LineEquation* lineEquation = dynamic_cast<LineEquation*>(firstParent->getEquation());
	double A = (*lineEquation).A;
	double B = (*lineEquation).B;
	double C = (*lineEquation).C;
	double yProjPoint = (-C - A * projPoint.x) / B;
	lastSignDeltaY = -(yProjPoint - projPoint.y);
	lastSignDeltaY = (lastSignDeltaY > 0) - (lastSignDeltaY < 0);
	ConstructionData::object = object;
}

void ByLineAndScalar::moveTo(Vector2f coord)
{
	LineEquation* lineEquation = dynamic_cast<LineEquation*>(firstParent->getEquation());
	Vector2f firstProj = Equation::Projection(*lineEquation, PointEquation(coord));
	Vector2f secondProj = Equation::Projection(*lineEquation);
	Vector2f delta = firstProj - secondProj;
	Vector2f altitude = secondProj - projPoint;
	double distance = sqrt(delta.x * delta.x + delta.y * delta.y);
	double crossProduct = altitude.x * delta.y - altitude.y * delta.x;
	int sign = (crossProduct > 0) - (crossProduct < 0);
	*secondParent = (distance * sign * directionSign);
}

ByLineAndScalar::~ByLineAndScalar()
{
	firstParent->eraseChild(object);
	secondParent->eraseChild(object);
	delete secondParent;
	secondParent = nullptr;
}

void ByLineAndScalar::recreate(Equation* equation)
{
	LineEquation* lineEquation = dynamic_cast<LineEquation*>(firstParent->getEquation());
	ScalarEquation* scalarEquation = dynamic_cast<ScalarEquation*>(secondParent->getEquation());
	double A = (*lineEquation).A;
	double B = (*lineEquation).B;
	double C = (*lineEquation).C;
	Vector2f direction = Vector2f(-B, A);
	direction /= (float)sqrt(A * A + B * B);
	direction *= (float)(*scalarEquation).value;
	Vector2f projection = Equation::Projection(*lineEquation);
	Vector2f altitude = projection - projPoint;
	double crossProduct = altitude.x * direction.y - altitude.y * direction.x;
	int signCross = (crossProduct > 0) - (crossProduct < 0);
	int signScalar = ((*scalarEquation).value > 0) - ((*scalarEquation).value < 0);
	if (signCross != signScalar)
	{
		direction *= -1.f;
	}
	checkDirectionSign(A, B, C);
	Vector2f position = projection + direction * directionSign;
	*dynamic_cast<PointEquation*>(equation) = (PointEquation(position));
}

Scalar::Scalar(double value)
{
	equation = new ScalarEquation(value);
}

Scalar& Scalar::operator+=(double deltaValue)
{
	dynamic_cast<ScalarEquation*>(equation)->operator+=(deltaValue);
	return *this;
}

ScalarEquation::ScalarEquation(double value)
	: value(value)
{
}

ScalarEquation& ScalarEquation::operator+=(double deltaValue)
{
	value += deltaValue;
	return *this;
}

ByCircleAndScalar::ByCircleAndScalar(Object* object, UnitCircle* firstParent, Scalar* secondParent)
	: firstParent(firstParent), secondParent(secondParent)
{
	ConstructionData::object = object;
}

ByCircleAndScalar::~ByCircleAndScalar()
{
	firstParent->eraseChild(object);
	secondParent->eraseChild(object);
	delete secondParent;
	secondParent = nullptr;
}

void ByCircleAndScalar::moveTo(Vector2f coord)
{
	double angle = atan2(coord.y, coord.x);
	*secondParent = (angle);
}
void ByCircleAndScalar::recreate(Equation* equation)
{
	ScalarEquation* scalarEquation = dynamic_cast<ScalarEquation*>(secondParent->getEquation());
	double angle = (*scalarEquation).value;
	*dynamic_cast<PointEquation*>(equation) = PointEquation(Vector2f(cos(angle) * unitSeg, sin(angle) * unitSeg));
}

CenterPoint::CenterPoint() : Point()
{
	auto parent = new ComplexScalar(Vector2f(0, 0));
	construction = new ByComplexScalar(this, parent);
	parent->addChild(this);
	equation = new PointEquation(Vector2f(0, 0));
	shape.setOrigin(pointSize, pointSize);
	shape.setFillColor(Color::Black);
	Drawer::allVisibleObjects.push_front(this);
}

void CenterPoint::reposition()
{
	shape.setPosition(Vector2f(0, 0));
}

CenterPoint* CenterPoint::centerPoint;
CenterPoint* CenterPoint::getInstance()
{
	if (centerPoint == nullptr) {
		centerPoint = new CenterPoint();
	}
	return centerPoint;
}

CenterPoint::~CenterPoint()
{
	centerPoint = new CenterPoint();
}

void CenterPoint::moveTo(Vector2f coords)
{
}

UnitPoint::UnitPoint(UnitCircle* unitCircle, Vector2f mousePosition)
{
	auto scalar = new Scalar(atan2(mousePosition.y, mousePosition.x));
	construction = new ByCircleAndScalar(this, unitCircle, scalar);
	unitCircle->addChild(this);
	scalar->addChild(this);
	Init();
}

UnitPoint::UnitPoint(UnitCircle* unitCircle, Point* first, UnitPoint* second)
{
	construction = new CentralProjection(this, unitCircle, first, second);
	unitCircle->addChild(this);
	first->addChild(this);
	second->addChild(this);
	Init();
}

Projection::Projection(Object* object, Point* first, Line* second)
	:firstParent(first), secondParent(second)
{
	ConstructionData::object = object;
}

void Projection::recreate(Equation* equation)
{
	LineEquation* lineEquation = dynamic_cast<LineEquation*>(secondParent->getEquation());
	PointEquation* pointEquation = dynamic_cast<PointEquation*>(firstParent->getEquation());
	Vector2f position = Equation::Projection(*lineEquation, *pointEquation);
	*dynamic_cast<PointEquation*>(equation) = PointEquation(position);
}

void Projection::moveTo(Vector2f coords)
{
}

Projection::~Projection()
{
	firstParent->eraseChild(object);
	secondParent->eraseChild(object);
}
