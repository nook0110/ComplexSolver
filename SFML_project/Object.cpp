#include "Object.h"


extern double const epsilon;
extern RenderWindow mainWindow;
extern View view;

Equation* Object::getEquation()
{
	return equation;
}

Object::~Object()
{
	erase();
	delete construction;
	if (description)
	{
		delete description;
	}
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

Color Object::getColor()
{
	return visible ? visibleColor : unvisibleColor;
}

void Object::printExpr()
{
}

double Object::distance(Vector2f point)
{
	return 0.0;
}

void Object::setVisibility(bool visibility)
{
	visible = visibility;
}

void Object::setVisibility()
{
	visible = !visible;
}

void Object::setColor(Color color)
{
	visibleColor = color;
}

bool Object::getVisibility()
{
	return visible;
}

void Object::erase()
{
	Drawer::allVisibleObjects.remove(this);
}

bool Object::isOnCircle()
{
	return dynamic_cast<OnCircle*>(construction);
}

std::string Object::makeTeX()
{
	return std::string();
}

std::string Object::getLowerCaseName()
{
	return std::to_string(int(this));
}

void Object::switchDescription(Vector2f position)
{
	if (!description)
	{
		if (equationPath.empty())
		{
			equationPath = makeTexture(makeTeX(), getLowerCaseName());
		}
		description = new Description(equationPath, getLowerCaseName());
		description->moveTo(position);
	}
	else
	{
		delete description;
		description = nullptr;
	}
}

void Object::add()
{
	if (Drawer::VisibleObjectsContains(this))
		return;
	Drawer::allVisibleObjects.push_back(this);
	for (auto child : children)
		child->add();
}

void Object::del()
{
	if (description)
	{
		delete description;
	}
	NameBox::names[getLowerCaseName()] = false;
	Drawer::allVisibleObjects.remove(this);
	for (auto child : children)
		child->del();
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

double UnitCircle::distance(Vector2f point)
{
	Vector2f center = shape.getPosition();
	double radius = shape.getRadius();
	double distanceToCenter = sqrt(
		pow(point.x - 0, 2) +
		pow(point.y - 0, 2));
	double distance = abs(distanceToCenter - radius);
	Vector2f delta = Vector2f(mainWindow.mapCoordsToPixel(Vector2f(0, distance), view) - mainWindow.mapCoordsToPixel(Vector2f(0, 0), view));
	return sqrt(delta.x * delta.x + delta.y * delta.y);
}

bool UnitCircle::isNearby(Vector2f position)
{
	return distance(position) < epsilon;
}

void UnitCircle::draw()
{
	shape.setOutlineColor(getColor());
	mainWindow.draw(shape);
}

void UnitCircle::switchDescription()
{
}

void Circle::draw()
{
	mainWindow.draw(shape);
}

void Circle::switchDescription()
{
}

void Circle::reposition()
{
	construction->recreate(equation);
	CircleEquation* circleEquation = dynamic_cast<CircleEquation*>(equation);
	Vector2f position = circleEquation->center;
	double radius = circleEquation->radius;
	shape.setRadius(radius - outlineThickness / 2);
	shape.setOrigin(radius - outlineThickness / 2, radius - outlineThickness / 2);
	shape.setPosition(position);
	shape.setOutlineThickness(outlineThickness);
}

double Circle::distance(Vector2f point)
{
	return 0.0;
}

bool Circle::isNearby(Vector2f position)
{
	return false;
}

Circle::Circle(Point* first, Point* second, Point* third, Point* fourth)
{
	first->addChild(this);
	second->addChild(this);
	third->addChild(this);
	fourth->addChild(this);
	construction = new ByFourPoints(this, first, second, third, fourth);
	equation = new CircleEquation(Vector2f(0, 0), 0);
	shape.setPointCount(100);
	shape.setOutlineColor(getColor());
	shape.setFillColor(Color(0, 0, 0, 0));
	reposition();
	Drawer::addObject(this);
}

double Line::distance(Vector2f point)
{
	LineEquation* lineEquation = dynamic_cast<LineEquation*>(getEquation());
	double A = lineEquation->A;
	double B = lineEquation->B;
	double C = lineEquation->C;
	double distance = abs((A * point.x + B * point.y + C) / sqrt(A * A + B * B));
	Vector2f delta = Vector2f(mainWindow.mapCoordsToPixel(Vector2f(0, distance), view) - mainWindow.mapCoordsToPixel(Vector2f(0, 0), view));
	return sqrt(delta.x * delta.x + delta.y * delta.y);
}

void Line::printExpr()
{
	ConstructionLine* cl = dynamic_cast<ConstructionLine*>(construction);
	(cl->z_coef).print();
	std::cout << std::endl;
	(cl->z_conj_coef).print();
	std::cout << std::endl;
	(cl->free_coef).print();
	std::cout << std::endl;
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

bool Line::isNearby(Vector2f position)
{
	return distance(position) < epsilon;
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

	if (dotted)
	{
		const int stripes = 75;
		Vertex line[stripes + 1];
		for (int i = 0; i <= stripes; i++)
		{
			line[i] = Vertex(Vector2f((x2 - x1) * i / stripes + x1, (y2 - y1) * i / stripes + y1), getColor());
		}
		mainWindow.draw(line, stripes+1, Lines);
	}
	else
	{
		Vertex line[] =
		{
			Vertex(Vector2f(x1, y1), getColor()),
			Vertex(Vector2f(x2, y2), getColor())
		};
		mainWindow.draw(line, 2, Lines);
	}

}

void Line::setDotted(bool dotted)
{
	Line::dotted = dotted;
}

std::string Line::makeTeX()
{
	ConstructionLine* constr = dynamic_cast<ConstructionLine*>(construction);
	std::string TeX;
	TeX += constr->z_coef.getTEXformat() + "$$ \n $$";
	TeX += constr->z_conj_coef.getTEXformat() + "$$ \n $$";
	TeX += constr->free_coef.getTEXformat();
	return TeX;
}

void Line::switchDescription()
{
}

void Line::reposition()
{
	construction->recreate(equation);
}

void Line::Init()
{
	Drawer::addObject(this);
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

//LineSegment::LineSegment(Point* first, Point* second)
//{
//	first->addChild(this);
//	second->addChild(this);
//	//construction = new Segment(first, second)
//	//reposition();
//}

Chord::Chord(UnitPoint* first, UnitPoint* second)
{
	first->addChild(this);
	second->addChild(this);
	construction = new ByTwoUnitPoints(this, first, second);
	equation = new LineEquation(0, 0, 0);
	reposition();
	Init();
}


double Point::distance(Vector2f point)
{
	point = Vector2f(mainWindow.mapCoordsToPixel(point, view));
	Vector2f coord = Vector2f(mainWindow.mapCoordsToPixel(shape.getPosition(), view));
	return sqrt(pow((coord.x - point.x), 2) + pow((coord.y - point.y), 2));
}

Vector2f Point::getCoordinate()
{
	return shape.getPosition();
}

Point::Point(Point* first, Point* second, std::pair<int, int> masses)
{
	setName();
	construction = new byTwoPointsFixedRatio(this, first, second, masses);
	first->addChild(this);
	second->addChild(this);
	Init();
}

Point::~Point()
{
	NameBox::names.erase(getLowerCaseName());
}

void Point::reposition()
{
	construction->recreate(equation);
	PointEquation* pointEquation = dynamic_cast<PointEquation*>(equation);
	Vector2f position = pointEquation->point;
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
	font.loadFromFile("Textures\\Font\\font.ttf");
	nameText.setFillColor(textColor);
	nameText.setScale(textScale);
	equation = new PointEquation(Vector2f(0, 0));
	shape.setOrigin(pointSize, pointSize);
	shape.setFillColor(Color::Black);
	reposition();
	Drawer::addObject(this);
}

Point::Point(Vector2f position)
{
	setName();
	construction = new OnPlane(this, position);
	Init();
}

Point::Point(Line* first, Line* second)
{
	setName();
	construction = new IntersectionOfTwoLines(this, first, second);
	first->addChild(this);
	second->addChild(this);
	Init();
}

Point::Point(Line* line, Point* first, Point* second, Vector2f position)
{
	setName();
	construction = new OnLine(this, first, second, 0, line);
	first->addChild(this);
	second->addChild(this);
	line->addChild(this);
	Init();
	moveTo(position);
}

Point::Point(Point* point, Line* line)
{
	setName();
	construction = new Projection(this, point, line);
	point->addChild(this);
	line->addChild(this);
	Init();
}

Point::Point(UnitPoint* first, UnitPoint* second, UnitPoint* third)
{
	setName();
	construction = new Orthocenter(this, first, second, third);
	first->addChild(this);
	second->addChild(this);
	third->addChild(this);
	Init();
}

Point::Point(Point* first, Point* second, Point* third)
{
	setName();
	construction = new Barycenter(this, first, second, third);
	first->addChild(this);
	second->addChild(this);
	third->addChild(this);
	Init();
}

void Point::printExpr()
{
	ConstructionPoint* cp = dynamic_cast<ConstructionPoint*>(construction);
	cp->coord.print();
	std::cout << std::endl;
}

Point::Point(Point* center, Point* preimage, int sign)
{
	setName();
	construction = new Rotation90(this, center, preimage, sign);
	center->addChild(this);
	preimage->addChild(this);
	Init();
}

bool Point::isNearby(Vector2f position)
{
	return distance(position) < epsilon;
}

bool Point::contains(Vector2f position)
{
	return shape.getGlobalBounds().contains(position);
}

void Point::setName()
{
	NameBox nameBox;
	while (!nameBox.isFinished())
	{
		std::this_thread::sleep_for(std::chrono::nanoseconds(1000));
	}
	pointName = nameBox.getName();
	nameText = Text(pointName, font, textSize);
}

void Point::draw()
{
	nameText.setPosition(shape.getPosition());
	shape.setFillColor(getColor());
	mainWindow.draw(shape);
	mainWindow.draw(nameText);
}

std::string Point::makeTeX()
{
	return dynamic_cast<ConstructionPoint*>(construction)->coord.getTEXformat();
}

std::string Point::getLowerCaseName()
{
	auto name = pointName;
	std::transform(name.begin(), name.end(), name.begin(),
		[](unsigned char c) { return std::tolower(c); });
	return name;
}

ByFourPoints::ByFourPoints(Object* object, Point* first, Point* second, Point* third, Point* fourth)
	:firstParent(first), secondParent(second), thirdParent(third), fourthParent(fourth), ConstructionCircle(object)
{
	ConstructionData::object = object;
}

OnPlane::OnPlane(Object* object) : ConstructionPoint(object)
{
}

void OnPlane::recreate(Equation* equation)
{
	*dynamic_cast<PointEquation*>(equation) = PointEquation(point);
}

void OnPlane::moveTo(Vector2f coords)
{
	point = coords;
}

OnPlane::~OnPlane()
{
}

Center::Center(Point* object) : OnPlane(object)
{
	point = Vector2f(0, 0);
	coord = expr(make_scalar(0));
}

void IntersectionOfTwoLines::recreate(Equation* equation)
{
	LineEquation* firstEquation = dynamic_cast<LineEquation*>(firstParent->getEquation());
	LineEquation* secondEquation = dynamic_cast<LineEquation*>(secondParent->getEquation());
	Vector2f pointCoord = Vector2f(
		(firstEquation->B * secondEquation->C - firstEquation->C * secondEquation->B)
		/ (firstEquation->A * secondEquation->B - firstEquation->B * secondEquation->A),
		(firstEquation->C * secondEquation->A - firstEquation->A * secondEquation->C)
		/ (firstEquation->A * secondEquation->B - firstEquation->B * secondEquation->A)
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

void CentralProjection::recreate(Equation* equation)
{
	PointEquation* firstEquation = dynamic_cast<PointEquation*>(secondParent->getEquation());
	PointEquation* secondEquation = dynamic_cast<PointEquation*>(thirdParent->getEquation());
	Vector2f firstCoord = firstEquation->point;
	Vector2f secondCoord = secondEquation->point;
	double A = -firstCoord.y + secondCoord.y;
	double B = -(-firstCoord.x + secondCoord.x);
	double C = firstCoord.x * (firstCoord.y - secondCoord.y) - firstCoord.y * (firstCoord.x - secondCoord.x);
	LineEquation lineEquation(A, B, C);
	Vector2f projection = projectionOnLine(lineEquation, PointEquation(Vector2f(0, 0)));
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

byTwoPointsFixedRatio::~byTwoPointsFixedRatio()
{
	firstParent->eraseChild(object);
	secondParent->eraseChild(object);
}


void byTwoPointsFixedRatio::recreate(Equation* equation)
{
	PointEquation* firstEquation = dynamic_cast<PointEquation*>(firstParent->getEquation());
	PointEquation* secondEquation = dynamic_cast<PointEquation*>(secondParent->getEquation());
	Vector2f firstCoord = firstEquation->point;
	Vector2f secondCoord = secondEquation->point;
	Vector2f pointCoord = (secondCoord * (float)masses.first + firstCoord * (float)masses.second) / (float)(masses.first + masses.second);
	*dynamic_cast<PointEquation*>(equation) = PointEquation(pointCoord);
}

void Pole::recreate(Equation* equation)
{
	return;
}

ByTwoPoints::ByTwoPoints(Object* object) : ConstructionLine(object)
{
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
	Vector2f secondCoord = secondEquation->point;
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

void PerpendicularBisector::recreate(Equation* equation)
{
	return;
}

void Polar::recreate(Equation* equation)
{
	return;
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
	double A = lineEquation->A;
	double B = lineEquation->B;
	double C = -lineEquation->A * coord.x - lineEquation->B * coord.y;
	*dynamic_cast<LineEquation*>(equation) = LineEquation(A, B, C);
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

ConstructionPoint::ConstructionPoint(Object* object) : ConstructionData(object)
{
}

void ConstructionPoint::moveTo(Vector2f coords)
{
}

ConstructionData::ConstructionData(Object* object)
	:object(object)
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

Vector2f projectionOnLine(LineEquation lineEquation, PointEquation pointEquation)
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

void OnLine::moveTo(Vector2f coord)
{
	Vector2f proj = projectionOnLine(*dynamic_cast<LineEquation*>(thirdParent->getEquation()), coord);
	PointEquation* firstEquation = dynamic_cast<PointEquation*>(firstParent->getEquation());
	PointEquation* secondEquation = dynamic_cast<PointEquation*>(secondParent->getEquation());
	Vector2f firstCoord = firstEquation->point;
	Vector2f secondCoord = secondEquation->point;
	ratio = -(proj - firstCoord).x / (proj - secondCoord).x;
}

void OnLine::recreate(Equation* equation)
{
	PointEquation* firstEquation = dynamic_cast<PointEquation*>(firstParent->getEquation());
	PointEquation* secondEquation = dynamic_cast<PointEquation*>(secondParent->getEquation());
	Vector2f firstCoord = firstEquation->point;
	Vector2f secondCoord = secondEquation->point;
	Vector2f pointCoord = (secondCoord * ratio + firstCoord * 1.f) / (ratio + 1.f);
	*dynamic_cast<PointEquation*>(equation) = PointEquation(pointCoord);
}

OnLine::~OnLine()
{
	firstParent->eraseChild(object);
	secondParent->eraseChild(object);
	thirdParent->eraseChild(object);
}

OnCircle::~OnCircle()
{
	firstParent->eraseChild(object);
}

void OnCircle::moveTo(Vector2f coord)
{
	angle = atan2(coord.y, coord.x);
}
void OnCircle::recreate(Equation* equation)
{
	*dynamic_cast<PointEquation*>(equation) = PointEquation(Vector2f(cos(angle) * unitSeg, sin(angle) * unitSeg));
}

CenterPoint::CenterPoint() : Point()
{
	pointName = "O";
	font.loadFromFile("Textures\\Font\\font.ttf");
	nameText = Text(pointName, font, textSize);
	nameText.setFillColor(textColor);
	construction = new Center(this);
	equation = new PointEquation(Vector2f(0, 0));
	shape.setOrigin(pointSize, pointSize);
	shape.setFillColor(Color::Black);
	Drawer::allVisibleObjects.push_front(this);
	nameText.setScale(textScale);
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

UnitPoint::UnitPoint(UnitCircle* unitCircle, Vector2f position)
{
	setName();
	float angle = atan2(position.y, position.x);
	construction = new OnCircle(this, unitCircle, angle);
	unitCircle->addChild(this);
	Init();
}

UnitPoint::UnitPoint(UnitCircle* unitCircle, Point* first, UnitPoint* second)
{
	setName();
	construction = new CentralProjection(this, unitCircle, first, second);
	unitCircle->addChild(this);
	first->addChild(this);
	second->addChild(this);
	Init();
}

UnitPoint::UnitPoint(UnitCircle* unitCircle, UnitPoint* point, Chord* chord)
{
	setName();
	construction = new IntersectionPerpendicularChord(this, unitCircle, point, chord);
	unitCircle->addChild(this);
	chord->addChild(this);
	point->addChild(this);
	Init();
}

UnitPoint::UnitPoint(UnitCircle* unitCircle, Chord* chord, UnitPoint* point)
{
	setName();
	construction = new IntersectionParallelChord(this, unitCircle, chord, point);
	unitCircle->addChild(this);
	chord->addChild(this);
	point->addChild(this);
	Init();
}

void Projection::recreate(Equation* equation)
{
	LineEquation* lineEquation = dynamic_cast<LineEquation*>(secondParent->getEquation());
	PointEquation* pointEquation = dynamic_cast<PointEquation*>(firstParent->getEquation());
	Vector2f position = projectionOnLine(*lineEquation, *pointEquation);
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

void ByFourPoints::recreate(Equation* equation)
{
	PointEquation* first = dynamic_cast<PointEquation*>(firstParent->getEquation());
	PointEquation* second = dynamic_cast<PointEquation*>(secondParent->getEquation());
	PointEquation* third = dynamic_cast<PointEquation*>(thirdParent->getEquation());
	Vector2f deltaOneTwo = first->point - second->point;
	Vector2f deltaTwoThree = second->point - third->point;
	Vector2f deltaThreeOne = third->point - first->point;
	double zFirst = (first->point.x) * (first->point.x) + (first->point.y) * (first->point.y);
	double zSecond = (second->point.x) * (second->point.x) + (second->point.y) * (second->point.y);
	double zThird = (third->point.x) * (third->point.x) + (third->point.y) * (third->point.y);
	double zX = deltaOneTwo.y * zThird + deltaTwoThree.y * zFirst + deltaThreeOne.y * zSecond;
	double zY = deltaOneTwo.x * zThird + deltaTwoThree.x * zFirst + deltaThreeOne.x * zSecond;
	double z = deltaOneTwo.x * deltaThreeOne.y - deltaOneTwo.y * deltaThreeOne.x;
	Vector2f center(-zX / (2 * z), zY / (2 * z));
	double radius = sqrt((first->point - center).x * (first->point - center).x + (first->point - center).y * (first->point - center).y);
	*dynamic_cast<CircleEquation*>(equation) = CircleEquation(center, radius);
}

ByFourPoints::~ByFourPoints()
{
	firstParent->eraseChild(object);
	secondParent->eraseChild(object);
	thirdParent->eraseChild(object);
	fourthParent->eraseChild(object);
}

CircleEquation::CircleEquation(Vector2f center, double radius)
	:center(center), radius(radius)
{
}

IntersectionParallelChord::~IntersectionParallelChord()
{
	unitCircle->eraseChild(object);
	firstParent->eraseChild(object);
	secondParent->eraseChild(object);
}

void IntersectionParallelChord::recreate(Equation* equation)
{
	LineEquation* firstEquation = dynamic_cast<LineEquation*>(firstParent->getEquation());
	PointEquation* secondEquation = dynamic_cast<PointEquation*>(secondParent->getEquation());
	Vector2f coord = secondEquation->point;
	double A = firstEquation->A;
	double B = firstEquation->B;
	double C = -(A * coord.x + B * coord.y);
	LineEquation lineEquation(A, B, C);
	Vector2f projection = projectionOnLine(lineEquation, PointEquation(Vector2f(0, 0)));
	Vector2f delta = projection - coord;
	*dynamic_cast<PointEquation*>(equation) = PointEquation(projection + delta);
}

IntersectionPerpendicularChord::~IntersectionPerpendicularChord()
{
	unitCircle->eraseChild(object);
	firstParent->eraseChild(object);
	secondParent->eraseChild(object);
}

void IntersectionPerpendicularChord::recreate(Equation* equation)
{
	PointEquation* firstEquation = dynamic_cast<PointEquation*>(firstParent->getEquation());
	LineEquation* secondEquation = dynamic_cast<LineEquation*>(secondParent->getEquation());
	Vector2f coord = firstEquation->point;
	double A = secondEquation->B;
	double B = -secondEquation->A;
	double C = -(A * coord.x + B * coord.y);
	LineEquation lineEquation(A, B, C);
	Vector2f projection = projectionOnLine(lineEquation, PointEquation(Vector2f(0, 0)));
	Vector2f delta = projection - coord;
	*dynamic_cast<PointEquation*>(equation) = PointEquation(projection + delta);
}

void Rotation90::recreate(Equation* equation)
{
	PointEquation* firstEquation = dynamic_cast<PointEquation*>(firstParent->getEquation());
	PointEquation* secondEquation = dynamic_cast<PointEquation*>(secondParent->getEquation());
	Vector2f center = firstEquation->point;
	Vector2f preimage = secondEquation->point;
	Vector2f delta = preimage - center;
	Vector2f deltaRot;
	deltaRot.x = -delta.y;
	deltaRot.y = delta.x;
	deltaRot *= (float)sign;
	*dynamic_cast<PointEquation*>(equation) = PointEquation(center + deltaRot);
}

Rotation90::~Rotation90()
{
	firstParent->eraseChild(object);
	secondParent->eraseChild(object);
}

ConstructionLine::ConstructionLine(Object* object) : ConstructionData(object)
{
}

ConstructionCircle::ConstructionCircle(Object* object) : ConstructionData(object)
{
}

Orthocenter::~Orthocenter()
{
	firstParent->eraseChild(object);
	secondParent->eraseChild(object);
	thirdParent->eraseChild(object);
}

void Orthocenter::recreate(Equation* equation)
{
	PointEquation* firstEquation = dynamic_cast<PointEquation*>(firstParent->getEquation());
	PointEquation* secondEquation = dynamic_cast<PointEquation*>(secondParent->getEquation());
	PointEquation* thirdEquation = dynamic_cast<PointEquation*>(thirdParent->getEquation());
	*dynamic_cast<PointEquation*>(equation) = PointEquation(firstEquation->point + secondEquation->point + thirdEquation->point);
}

Barycenter::~Barycenter()
{
	firstParent->eraseChild(object);
	secondParent->eraseChild(object);
	thirdParent->eraseChild(object);
}

void Barycenter::recreate(Equation* equation)
{
	PointEquation* firstEquation = dynamic_cast<PointEquation*>(firstParent->getEquation());
	PointEquation* secondEquation = dynamic_cast<PointEquation*>(secondParent->getEquation());
	PointEquation* thirdEquation = dynamic_cast<PointEquation*>(thirdParent->getEquation());
	*dynamic_cast<PointEquation*>(equation) = PointEquation((firstEquation->point + secondEquation->point + thirdEquation->point) / 3.f);
}