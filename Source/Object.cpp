#include "Object.h"


extern float const epsilon;
extern RenderWindow mainWindow;
extern View view;

Equation* Object::GetEquation()
{
	return equation_;
}

ConstructionData* Object::GetConstruction()
{
	return construction;
}

Object::~Object()
{
	erase();
	delete construction;
	if (description_)
	{
		delete description_;
	}
	deleteChildren();
	deleteFiles(GetLowerCaseName());
}

void Object::ReposeChildren()
{
	Reposition();
	for (auto child : children_)
	{
		child->ReposeChildren();
	}
}

void Object::deleteChildren()
{
	while (!children_.empty())
	{
		auto child = *children_.begin();
		delete (child);
		child = nullptr;
	}
}

void Object::eraseChild(Object* child)
{
	children_.remove(child);
}

void Object::addChild(Object* child)
{
	children_.push_back(child);
}

void Object::clearChildren()
{
	children_.remove(nullptr);
}

Color Object::GetColor()
{
	return visible_ ? visible_color_ : unvisibleColor;
}

void Object::printExpr()
{
}

void Object::SetVisibility(bool visibility)
{
	visible_ = visibility;
}

void Object::ChangeVisibility()
{
	visible_ = !visible_;
}

void Object::SetColor(Color color)
{
	visible_color_ = color;
}

bool Object::GetVisibility()
{
	return visible_;
}

void Object::erase()
{
	Drawer::all_visible_objects.remove(this);
}

bool Object::isOnCircle()
{
	return dynamic_cast<UnitPoint*>(this);
}

std::string Object::MakeTeX()
{
	return std::string();
}

void Object::UpdateTex()
{
	TeX_ = MakeTeX();
}

std::string Object::GetLowerCaseName()
{
	return std::string();
}

void Object::SwitchDescription(Vector2f position)
{
	if (!description_)
	{
		if (equation_path_.empty())
		{
			equation_path_ = makeTexture(TeX_, GetLowerCaseName() + std::to_string((uintptr_t)this));
		}
		description_ = new Description(equation_path_, GetLowerCaseName());
		description_->moveTo(position);
	}
	else
	{
		delete description_;
		description_ = nullptr;
	}
}

void Object::AddToDrawer()
{
	if (Drawer::VisibleObjectsContains(this))
		return;
	Drawer::all_visible_objects.push_back(this);
	for (auto child : children_)
		child->AddToDrawer();
}

void Object::DeleteFromDrawer()
{
	if (description_)
	{
		delete description_;
		description_ = nullptr;
	}
	NameBox::names[GetLowerCaseName()] = false;
	Drawer::all_visible_objects.remove(this);
	for (auto child : children_)
		child->DeleteFromDrawer();
}

UnitCircle* UnitCircle::unit_circle = nullptr;
UnitCircle::UnitCircle()
{
	shape.setPointCount(100);
	shape.setPosition(Vector2f(0, 0));
	shape.setOrigin(unitSeg - outlineThickness / 2, unitSeg - outlineThickness / 2);
	shape.setOutlineThickness(outlineThickness);
	shape.setRadius(unitSeg - outlineThickness / 2);
	shape.setOutlineColor(GetColor());
	shape.setFillColor(Color(0, 0, 0, 0));
}

UnitCircle* UnitCircle::getInstance()
{
	if (unit_circle == nullptr) {
		unit_circle = new UnitCircle();
	}
	return unit_circle;
}

float UnitCircle::DistanceTo(Vector2f point)
{
	Vector2f center = shape.getPosition();
	float radius = shape.getRadius();
	float distanceToCenter = sqrtf(
		pow(point.x - 0, 2) +
		pow(point.y - 0, 2));
	float DistanceTo = abs(distanceToCenter - radius);
	Vector2f delta = Vector2f(mainWindow.mapCoordsToPixel(Vector2f(0, DistanceTo), view) - mainWindow.mapCoordsToPixel(Vector2f(0, 0), view));
	return sqrt(delta.x * delta.x + delta.y * delta.y);
}

bool UnitCircle::IsNearby(Vector2f position)
{
	return DistanceTo(position) < epsilon;
}

void UnitCircle::Draw()
{
	shape.setOutlineColor(GetColor());
	mainWindow.draw(shape);
}

void UnitCircle::SwitchDescription()
{
}

void Circle::Draw()
{
	mainWindow.draw(shape);
}

void Circle::SwitchDescription()
{
}

void Circle::Reposition()
{
	construction->recreate(equation_);
	CircleEquation* circleEquation = dynamic_cast<CircleEquation*>(equation_);
	Vector2f position = circleEquation->center;
	float radius = circleEquation->radius;
	shape.setRadius(radius - outlineThickness / 2);
	shape.setOrigin(radius - outlineThickness / 2, radius - outlineThickness / 2);
	shape.setPosition(position);
	shape.setOutlineThickness(outlineThickness);
}

float Circle::DistanceTo(Vector2f point)
{
	return 0.0f;
}

bool Circle::IsNearby(Vector2f position)
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
	equation_ = new CircleEquation(Vector2f(0, 0), 0);
	shape.setPointCount(100);
	shape.setOutlineColor(GetColor());
	shape.setFillColor(Color(0, 0, 0, 0));
	Reposition();
	Drawer::addObject(this);
}

float Line::DistanceTo(Vector2f point)
{
	LineEquation* lineEquation = dynamic_cast<LineEquation*>(GetEquation());
	float A = lineEquation->A;
	float B = lineEquation->B;
	float C = lineEquation->C;
	float DistanceTo = abs((A * point.x + B * point.y + C) / sqrt(A * A + B * B));
	Vector2f delta = Vector2f(mainWindow.mapCoordsToPixel(Vector2f(0, DistanceTo), view) - mainWindow.mapCoordsToPixel(Vector2f(0, 0), view));
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
	equation_ = new LineEquation(0, 0, 0);
	Reposition();
	Init();
}

Line::Line(Point* first, Line* second)
{
	first->addChild(this);
	second->addChild(this);
	construction = new Perpendicular(this, first, second);
	equation_ = new LineEquation(0, 0, 0);
	Reposition();
	Init();
}

bool Line::IsNearby(Vector2f position)
{
	return DistanceTo(position) < epsilon;
}

void Line::Draw()
{
	LineEquation lineEq = *(dynamic_cast<LineEquation*>(GetEquation()));
	float x1, x2, y1, y2;
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

	if (dotted_)
	{
		const int stripes = 75;
		Vertex lines[stripes + 1];
		for (int i = 0; i <= stripes; i++)
		{
			lines[i] = Vertex(Vector2f((x2 - x1) * i / stripes + x1, (y2 - y1) * i / stripes + y1), GetColor());
		}
		mainWindow.draw(lines, stripes + 1, Lines);
	}
	else
	{
		points_[0] = Vertex(Vector2f(x1, y1), GetColor());
		points_[1] = Vertex(Vector2f(x2, y2), GetColor());
		mainWindow.draw(points_, 2, Lines);
	}

	if (highlighted_)
	{
		Vector2f delta = Vector2f(x2 - x1, y2 - y1);
		float lineLength = sqrt(delta.x * delta.x + delta.y * delta.y);
		RectangleShape outline(Vector2f(lineLength, outline_thickness_ * 2));
		outline.setOrigin(Vector2f(0, outline_thickness_));
		float angle = atan2(delta.y, delta.x);
		const float radToDegRatio = 180 / (float)M_PI;
		outline.setRotation(angle * radToDegRatio);
		outline.move(Vector2f(x1, y1));
		outline.setFillColor(highlighted_color_);
		mainWindow.draw(outline);
	}

}



void Line::SetDotted(bool dotted_)
{
	Line::dotted_ = dotted_;
}

std::string Line::MakeTeX()
{
	ConstructionLine* constr = dynamic_cast<ConstructionLine*>(construction);
	std::string TeX_;
	TeX_ += constr->z_coef.getTEXformat() + "$$ \n $$";
	TeX_ += constr->z_conj_coef.getTEXformat() + "$$ \n $$";
	TeX_ += constr->free_coef.getTEXformat();
	return TeX_;
}

void Line::SwitchDescription()
{
}

void Line::Reposition()
{
	construction->recreate(equation_);
}

void Line::Init()
{
	TeX_ = MakeTeX();
	Drawer::addObject(this);
	points_[0].color = GetColor();
	points_[1].color = GetColor();
}


Line::Line(UnitCircle* first, Point* second)
{
	first->addChild(this);
	second->addChild(this);
	construction = new Tangent(this, first, second);
	equation_ = new LineEquation(0, 0, 0);
	Reposition();
	Init();
}

Line::Line(Point* first, Point* second)
{
	first->addChild(this);
	second->addChild(this);
	construction = new ByTwoPoints(this, first, second);
	equation_ = new LineEquation(0, 0, 0);
	Reposition();
	Init();
}

LineSegment::LineSegment(Point* first, Point* second)
{
	first->addChild(this);
	second->addChild(this);
	equation_ = new SegmentEquation(Vector2f(), Vector2f());
	construction = new ByTwoEnds(this, first, second);
	points_[0].color = first->GetColor();
	points_[1].color = second->GetColor();
	Reposition();
	Drawer::addObject(this);
}

void LineSegment::Reposition()
{
	construction->recreate(equation_);
	SegmentEquation* segmentEquation = dynamic_cast<SegmentEquation*>(equation_);
	points_[0].position = segmentEquation->pointFirst;
	points_[1].position = segmentEquation->pointSecond;
}

void LineSegment::Draw()
{
	mainWindow.draw(points_, 2, Lines);
}

Chord::Chord(UnitPoint* first, UnitPoint* second)
{
	first->addChild(this);
	second->addChild(this);
	construction = new ByTwoUnitPoints(this, first, second);
	equation_ = new LineEquation(0, 0, 0);
	Reposition();
	Init();
}


float Point::DistanceTo(Vector2f point)
{
	point = Vector2f(mainWindow.mapCoordsToPixel(point, view));
	Vector2f coord = Vector2f(mainWindow.mapCoordsToPixel(shape_.getPosition(), view));
	return sqrt(pow((coord.x - point.x), 2) + pow((coord.y - point.y), 2));
}

Vector2f Point::getCoordinate()
{
	return shape_.getPosition();
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
	NameBox::names.erase(GetLowerCaseName());
	deleteFiles(GetLowerCaseName());
}

void Point::Reposition()
{
	construction->recreate(equation_);
	PointEquation* pointEquation = dynamic_cast<PointEquation*>(equation_);
	Vector2f position = pointEquation->point;
	shape_.setPosition(position);
}

Point::Point()
{
}

void Point::moveTo(Vector2f coords)
{
	dynamic_cast<ConstructionPoint*>(construction)->moveTo(coords);
	construction->recreate(equation_);
	ReposeChildren();
}

void Point::Init()
{
	TeX_ = MakeTeX();
	font_.loadFromFile("Textures\\Font\\font.ttf");
	name_.setFillColor(text_color_);
	name_.setScale(text_scale_);
	equation_ = new PointEquation(Vector2f(0, 0));
	shape_.setOrigin(kPointSize, kPointSize);
	Reposition();
	shape_.setFillColor(GetColor());
	Drawer::addObject(this);
}

Point::Point(Vector2f position)
{
	SetColor(movable_color_);
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

Point::Point(Line* points_, Point* first, Point* second, Vector2f position)
{
	SetColor(movable_color_);
	setName();
	construction = new OnLine(this, first, second, 0, points_);
	first->addChild(this);
	second->addChild(this);
	points_->addChild(this);
	Init();
	moveTo(position);
}

Point::Point(Point* point, Line* points_)
{
	setName();
	construction = new Projection(this, point, points_);
	point->addChild(this);
	points_->addChild(this);
	Init();
}

Point::Point(UnitPoint* first, UnitPoint* second, UnitPoint* third, triangleCenter center)
{
	setName();
	switch (center)
	{
	case ORTHOCENTER:
		construction = new Orthocenter(this, first, second, third);
		break;
	case INCENTER:
		construction = new Incenter(this, first, second, third);
		break;
	}
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

bool Point::IsNearby(Vector2f position)
{
	return DistanceTo(position) < epsilon;
}

bool Point::contains(Vector2f position)
{
	return shape_.getGlobalBounds().contains(position);
}

void Point::setName()
{
	auto current = Mousemode;
	Mousemode = MODE_NAMING;
	NameBox* namebox = nullptr;
	if (!Drawer::dialogBox)
	{
		namebox = new NameBox;
	}
	while (!Drawer::dialogBox->isFinished())
	{
		std::this_thread::sleep_for(std::chrono::nanoseconds(1000));
	}
	point_name_ = dynamic_cast<NameBox*>(Drawer::dialogBox)->getName();
	name_ = Text(point_name_, font_, text_size_);
	Mousemode = current;
	if (namebox)
	{
		delete namebox;
	}
}

void Point::Draw()
{
	shape_.setFillColor(GetColor());
	name_.setPosition(shape_.getPosition());
	mainWindow.draw(shape_);
	mainWindow.draw(name_);
}

std::string Point::MakeTeX()
{
	return dynamic_cast<ConstructionPoint*>(construction)->coord.getTEXformat();
}

std::string Point::GetLowerCaseName()
{
	auto name = point_name_;
	std::transform(name.begin(), name.end(), name.begin(),
		[](unsigned char c) { return std::tolower(c); });
	return name;
}

void Point::highlight()
{
	shape_.setOutlineThickness(outline_thickness_);
	shape_.setOutlineColor(highlighted_color_);
}

void Point::unhighlight()
{
	shape_.setOutlineThickness(0);
	shape_.setOutlineColor(Color());
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
	LineEquation* first_equation = dynamic_cast<LineEquation*>(firstParent->GetEquation());
	LineEquation* second_equation = dynamic_cast<LineEquation*>(secondParent->GetEquation());
	Vector2f pointCoord = Vector2f(
		(first_equation->B * second_equation->C - first_equation->C * second_equation->B)
		/ (first_equation->A * second_equation->B - first_equation->B * second_equation->A),
		(first_equation->C * second_equation->A - first_equation->A * second_equation->C)
		/ (first_equation->A * second_equation->B - first_equation->B * second_equation->A)
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
	PointEquation* first_equation = dynamic_cast<PointEquation*>(secondParent->GetEquation());
	PointEquation* second_equation = dynamic_cast<PointEquation*>(thirdParent->GetEquation());
	Vector2f firstCoord = first_equation->point;
	Vector2f secondCoord = second_equation->point;
	float A = -firstCoord.y + secondCoord.y;
	float B = -(-firstCoord.x + secondCoord.x);
	float C = firstCoord.x * (firstCoord.y - secondCoord.y) - firstCoord.y * (firstCoord.x - secondCoord.x);
	LineEquation lineEquation(A, B, C);
	Vector2f projection = projectionOnLine(lineEquation, PointEquation(Vector2f(0, 0)));
	Vector2f delta = projection - thirdParent->getCoordinate();
	*dynamic_cast<PointEquation*>(equation) = PointEquation(projection + delta);
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
	PointEquation* first_equation = dynamic_cast<PointEquation*>(firstParent->GetEquation());
	PointEquation* second_equation = dynamic_cast<PointEquation*>(secondParent->GetEquation());
	Vector2f firstCoord = first_equation->point;
	Vector2f secondCoord = second_equation->point;
	Vector2f pointCoord = (secondCoord * (float)masses.first + firstCoord * (float)masses.second) / (float)(masses.first + masses.second);
	*dynamic_cast<PointEquation*>(equation) = PointEquation(pointCoord);
}

void Pole::recreate(Equation* equation)
{
	return;
}

void ByTwoPoints::recreate(Equation* equation)
{
	if (!equation)
	{
		equation = new LineEquation(0, 0, 0);
	}
	PointEquation* first_equation = dynamic_cast<PointEquation*>(firstParent->GetEquation());
	PointEquation* second_equation = dynamic_cast<PointEquation*>(secondParent->GetEquation());
	Vector2f firstCoord = first_equation->point;
	Vector2f secondCoord = second_equation->point;
	float A = -firstCoord.y + secondCoord.y;
	float B = -(-firstCoord.x + secondCoord.x);
	float C = firstCoord.x * (firstCoord.y - secondCoord.y) - firstCoord.y * (firstCoord.x - secondCoord.x);
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


Perpendicular::~Perpendicular()
{
	firstParent->eraseChild(object);
	secondParent->eraseChild(object);
}

void Perpendicular::recreate(Equation* equation)
{
	PointEquation* first_equation = dynamic_cast<PointEquation*>(firstParent->GetEquation());
	LineEquation* second_equation = dynamic_cast<LineEquation*>(secondParent->GetEquation());
	Vector2f coord = first_equation->point;
	float A = -(*second_equation).B;
	float B = (*second_equation).A;
	float C = (*second_equation).B * coord.x - (*second_equation).A * coord.y;
	*dynamic_cast<LineEquation*>(equation) = LineEquation(A, B, C);
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
	LineEquation* lineEquation = dynamic_cast<LineEquation*>(firstParent->GetEquation());
	PointEquation* pointEquation = dynamic_cast<PointEquation*>(secondParent->GetEquation());
	Vector2f coord = pointEquation->point;
	float A = lineEquation->A;
	float B = lineEquation->B;
	float C = -lineEquation->A * coord.x - lineEquation->B * coord.y;
	*dynamic_cast<LineEquation*>(equation) = LineEquation(A, B, C);
}

Tangent::~Tangent()
{
	firstParent->eraseChild(object);
	secondParent->eraseChild(object);
}

void Tangent::recreate(Equation* equation)
{
	PointEquation* second_equation = dynamic_cast<PointEquation*>(secondParent->GetEquation());
	Vector2f coord = second_equation->point;
	float A = coord.x;
	float B = coord.y;
	float C = -(coord.x * coord.x + coord.y * coord.y);
	*dynamic_cast<LineEquation*>(equation) = LineEquation(A, B, C);
}

ConstructionPoint::ConstructionPoint(Object* object) : ConstructionData(object)
{
}

ConstructionData::ConstructionData(Object* object)
	: object(object)
{
}


LineEquation::LineEquation(float A, float B, float C)
	: A(A), B(B), C(C)
{
}

SegmentEquation::SegmentEquation(Vector2f pointFirst, Vector2f pointSecond)
	: pointFirst(pointFirst), pointSecond(pointSecond)
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
	float A = lineEquation.A;
	float B = lineEquation.B;
	float C = lineEquation.C;
	float x = pointEquation.point.x;
	float y = pointEquation.point.y;
	Vector2f point(
		(B * B * x - A * (B * y + C)) / (A * A + B * B),
		(A * A * y - B * (A * x + C)) / (A * A + B * B));
	return point;
}

void OnLine::moveTo(Vector2f coord)
{
	Vector2f proj = projectionOnLine(*dynamic_cast<LineEquation*>(thirdParent->GetEquation()), coord);
	PointEquation* first_equation = dynamic_cast<PointEquation*>(firstParent->GetEquation());
	PointEquation* second_equation = dynamic_cast<PointEquation*>(secondParent->GetEquation());
	Vector2f firstCoord = first_equation->point;
	Vector2f secondCoord = second_equation->point;
	ratio = -(proj - firstCoord).x / (proj - secondCoord).x;
}

void OnLine::recreate(Equation* equation)
{
	PointEquation* first_equation = dynamic_cast<PointEquation*>(firstParent->GetEquation());
	PointEquation* second_equation = dynamic_cast<PointEquation*>(secondParent->GetEquation());
	Vector2f firstCoord = first_equation->point;
	Vector2f secondCoord = second_equation->point;
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
	point_name_ = "O";
	font_.loadFromFile("Textures\\Font\\font.ttf");
	name_ = Text(point_name_, font_, text_size_);
	name_.setFillColor(text_color_);
	construction = new Center(this);
	equation_ = new PointEquation(Vector2f(0, 0));
	shape_.setOrigin(kPointSize, kPointSize);
	shape_.setFillColor(Color::Black);
	Drawer::all_visible_objects.push_front(this);
	name_.setScale(text_scale_);
	UpdateTex();
}

void CenterPoint::Reposition()
{
	shape_.setPosition(Vector2f(0, 0));
}

CenterPoint* CenterPoint::center_point_;
CenterPoint* CenterPoint::getInstance()
{
	if (center_point_ == nullptr) {
		center_point_ = new CenterPoint();
	}
	return center_point_;
}

CenterPoint::~CenterPoint()
{
	center_point_ = new CenterPoint();
}

void CenterPoint::moveTo(Vector2f coords)
{
}

UnitPoint::UnitPoint(UnitCircle* unit_circle, Vector2f position)
{
	SetColor(movable_color_);
	setName();
	float angle = atan2(position.y, position.x);
	construction = new OnCircle(this, unit_circle, angle);
	unit_circle->addChild(this);
	Init();
}

UnitPoint::UnitPoint(UnitCircle* unit_circle, Point* first, UnitPoint* second)
{
	setName();
	construction = new CentralProjection(this, unit_circle, first, second);
	unit_circle->addChild(this);
	first->addChild(this);
	second->addChild(this);
	Init();
}

UnitPoint::UnitPoint(UnitCircle* unit_circle, UnitPoint* point, Chord* chord)
{
	setName();
	construction = new IntersectionPerpendicularChord(this, unit_circle, point, chord);
	unit_circle->addChild(this);
	chord->addChild(this);
	point->addChild(this);
	Init();
}

UnitPoint::UnitPoint(UnitCircle* unit_circle, Chord* chord, UnitPoint* point)
{
	setName();
	construction = new IntersectionParallelChord(this, unit_circle, chord, point);
	unit_circle->addChild(this);
	chord->addChild(this);
	point->addChild(this);
	Init();
}

void Projection::recreate(Equation* equation)
{
	LineEquation* lineEquation = dynamic_cast<LineEquation*>(secondParent->GetEquation());
	PointEquation* pointEquation = dynamic_cast<PointEquation*>(firstParent->GetEquation());
	Vector2f position = projectionOnLine(*lineEquation, *pointEquation);
	*dynamic_cast<PointEquation*>(equation) = PointEquation(position);
}

Projection::~Projection()
{
	firstParent->eraseChild(object);
	secondParent->eraseChild(object);
}

void ByFourPoints::recreate(Equation* equation)
{
	PointEquation* first = dynamic_cast<PointEquation*>(firstParent->GetEquation());
	PointEquation* second = dynamic_cast<PointEquation*>(secondParent->GetEquation());
	PointEquation* third = dynamic_cast<PointEquation*>(thirdParent->GetEquation());
	Vector2f deltaOneTwo = first->point - second->point;
	Vector2f deltaTwoThree = second->point - third->point;
	Vector2f deltaThreeOne = third->point - first->point;
	float zFirst = (first->point.x) * (first->point.x) + (first->point.y) * (first->point.y);
	float zSecond = (second->point.x) * (second->point.x) + (second->point.y) * (second->point.y);
	float zThird = (third->point.x) * (third->point.x) + (third->point.y) * (third->point.y);
	float zX = deltaOneTwo.y * zThird + deltaTwoThree.y * zFirst + deltaThreeOne.y * zSecond;
	float zY = deltaOneTwo.x * zThird + deltaTwoThree.x * zFirst + deltaThreeOne.x * zSecond;
	float z = deltaOneTwo.x * deltaThreeOne.y - deltaOneTwo.y * deltaThreeOne.x;
	Vector2f center(-zX / (2 * z), zY / (2 * z));
	float radius = sqrt((first->point - center).x * (first->point - center).x + (first->point - center).y * (first->point - center).y);
	*dynamic_cast<CircleEquation*>(equation) = CircleEquation(center, radius);
}

ByFourPoints::~ByFourPoints()
{
	firstParent->eraseChild(object);
	secondParent->eraseChild(object);
	thirdParent->eraseChild(object);
	fourthParent->eraseChild(object);
}

CircleEquation::CircleEquation(Vector2f center, float radius)
	:center(center), radius(radius)
{
}

IntersectionParallelChord::~IntersectionParallelChord()
{
	unit_circle->eraseChild(object);
	firstParent->eraseChild(object);
	secondParent->eraseChild(object);
}

void IntersectionParallelChord::recreate(Equation* equation)
{
	LineEquation* first_equation = dynamic_cast<LineEquation*>(firstParent->GetEquation());
	PointEquation* second_equation = dynamic_cast<PointEquation*>(secondParent->GetEquation());
	Vector2f coord = second_equation->point;
	float A = first_equation->A;
	float B = first_equation->B;
	float C = -(A * coord.x + B * coord.y);
	LineEquation lineEquation(A, B, C);
	Vector2f projection = projectionOnLine(lineEquation, PointEquation(Vector2f(0, 0)));
	Vector2f delta = projection - coord;
	*dynamic_cast<PointEquation*>(equation) = PointEquation(projection + delta);
}

IntersectionPerpendicularChord::~IntersectionPerpendicularChord()
{
	unit_circle->eraseChild(object);
	firstParent->eraseChild(object);
	secondParent->eraseChild(object);
}

void IntersectionPerpendicularChord::recreate(Equation* equation)
{
	PointEquation* first_equation = dynamic_cast<PointEquation*>(firstParent->GetEquation());
	LineEquation* second_equation = dynamic_cast<LineEquation*>(secondParent->GetEquation());
	Vector2f coord = first_equation->point;
	float A = second_equation->B;
	float B = -second_equation->A;
	float C = -(A * coord.x + B * coord.y);
	LineEquation lineEquation(A, B, C);
	Vector2f projection = projectionOnLine(lineEquation, PointEquation(Vector2f(0, 0)));
	Vector2f delta = projection - coord;
	*dynamic_cast<PointEquation*>(equation) = PointEquation(projection + delta);
}

void Rotation90::recreate(Equation* equation)
{
	PointEquation* first_equation = dynamic_cast<PointEquation*>(firstParent->GetEquation());
	PointEquation* second_equation = dynamic_cast<PointEquation*>(secondParent->GetEquation());
	Vector2f center = first_equation->point;
	Vector2f preimage = second_equation->point;
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

ConstructionSegment::ConstructionSegment(Object* object) : ConstructionData(object)
{
}

ByTwoEnds::ByTwoEnds(Object* object, Point* first, Point* second)
	: ConstructionSegment(object), firstParent(first), secondParent(second)
{
}

void ByTwoEnds::recreate(Equation* equation)
{
	PointEquation* first_equation = dynamic_cast<PointEquation*>(firstParent->GetEquation());
	PointEquation* second_equation = dynamic_cast<PointEquation*>(secondParent->GetEquation());
	*dynamic_cast<SegmentEquation*>(equation) = SegmentEquation(first_equation->point, second_equation->point);
}

ByTwoEnds::~ByTwoEnds()
{
	firstParent->eraseChild(object);
	secondParent->eraseChild(object);
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
	PointEquation* first_equation = dynamic_cast<PointEquation*>(firstParent->GetEquation());
	PointEquation* second_equation = dynamic_cast<PointEquation*>(secondParent->GetEquation());
	PointEquation* thirdEquation = dynamic_cast<PointEquation*>(thirdParent->GetEquation());
	*dynamic_cast<PointEquation*>(equation) = PointEquation(first_equation->point + second_equation->point + thirdEquation->point);
}

Incenter::~Incenter()
{
	firstParent->eraseChild(object);
	secondParent->eraseChild(object);
	thirdParent->eraseChild(object);
}

void Incenter::recreate(Equation* equation)
{
	PointEquation* first_equation = dynamic_cast<PointEquation*>(firstParent->GetEquation());
	PointEquation* second_equation = dynamic_cast<PointEquation*>(secondParent->GetEquation());
	PointEquation* thirdEquation = dynamic_cast<PointEquation*>(thirdParent->GetEquation());
	Vector2f firstPoint = first_equation->point;
	Vector2f secondPoint = second_equation->point;
	Vector2f thirdPoint = thirdEquation->point;
	float sideA = sqrt((secondPoint - thirdPoint).x * (secondPoint - thirdPoint).x + (secondPoint - thirdPoint).y * (secondPoint - thirdPoint).y);
	float sideB = sqrt((firstPoint - thirdPoint).x * (firstPoint - thirdPoint).x + (firstPoint - thirdPoint).y * (firstPoint - thirdPoint).y);
	float sideC = sqrt((secondPoint - firstPoint).x * (secondPoint - firstPoint).x + (secondPoint - firstPoint).y * (secondPoint - firstPoint).y);

	*dynamic_cast<PointEquation*>(equation) = PointEquation((first_equation->point * sideA + second_equation->point * sideB + thirdEquation->point * sideC) / (sideA + sideB + sideC));

}

Barycenter::~Barycenter()
{
	firstParent->eraseChild(object);
	secondParent->eraseChild(object);
	thirdParent->eraseChild(object);
}

void Barycenter::recreate(Equation* equation)
{
	PointEquation* first_equation = dynamic_cast<PointEquation*>(firstParent->GetEquation());
	PointEquation* second_equation = dynamic_cast<PointEquation*>(secondParent->GetEquation());
	PointEquation* thirdEquation = dynamic_cast<PointEquation*>(thirdParent->GetEquation());
	*dynamic_cast<PointEquation*>(equation) = PointEquation((first_equation->point + second_equation->point + thirdEquation->point) / 3.f);
}