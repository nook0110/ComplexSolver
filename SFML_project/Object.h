#pragma once
#include <SFML/Graphics.hpp>
#include <list>
#include "gui.h"
#include "Creation.h"

#include <iostream>

//The "heart" of the programm. This is file is about objects.

using namespace sf;

extern double const epsilon;
extern double const unitSeg;
extern std::thread Creator;
extern MODES Mousemode;

class LineEquation;
class PointEquation;

//Equation for all objects
struct Equation
{
	virtual ~Equation();
	Equation();
	static Vector2f Projection(LineEquation lineEquation, PointEquation pointEquation);
};

struct CircleEquation : public Equation
{
	Vector2f center;
	double radius;
	CircleEquation(Vector2f center, double radius);
};

struct LineEquation : public Equation
{
	double A, B, C;
	LineEquation(double A, double B, double C);
};

struct PointEquation : public Equation
{
	Vector2f point;
	PointEquation(Vector2f point);
};

struct ScalarEquation : public Equation
{
	double value;
	ScalarEquation(double value);
	ScalarEquation& operator+=(double deltaValue);
};

struct ComplexScalarEquation : public Equation
{
	Vector2f point;
	ComplexScalarEquation(Vector2f point);
	ComplexScalarEquation& operator+=(Vector2f delta);
};

//Objects - things that appear on the screen or help to construct them, such as UnitCircle, Circle, Points, Lines or Scalar and ComplexScalar
class Object
{
private:
	std::list<Object*> children;
protected:
	virtual void reposition();
	void reposeChildren();
	Equation* equation;
private:
	void deleteChildren();
public:
	void eraseChild(Object* child);
	void addChild(Object* child);
	void clearChildren();
	Equation* getEquation();
	virtual ~Object();
};

class VisibleObject;
class Line;
class Point;
class UnitCircle;
class Parametr;
class Scalar;
class ComplexScalar;

//Data - how object was created (By two points and so on)
class ConstructionData
{
protected:
	Object* object;
public:
	//Return equation of the objects 
	//The equation can be changed if parents were moved
	virtual void recreate(Equation* equation);
	virtual ~ConstructionData();
};

class ConstructionCircle : public ConstructionData
{
};

class ByFourPoints : public ConstructionCircle
{
	Point* firstParent, * secondParent, * thirdParent, * fourthParent;
public:
	ByFourPoints(Object* object, Point* first, Point* second, Point* third, Point* fourth);
	void recreate(Equation* equation) override;
	~ByFourPoints();
};

class ConstructionPoint : public ConstructionData
{
public:
	virtual void moveTo(Vector2f coords);
};

class ConstructionLine : public ConstructionData
{
};

class ByComplexScalar : public ConstructionPoint
{
	ComplexScalar* parent;
public:
	ByComplexScalar(Object* object, ComplexScalar* ComplexScalar);
	void recreate(Equation* equation) override;
	void moveTo(Vector2f coords) override;
	~ByComplexScalar();
};

class IntersectionOfTwoLines : public ConstructionPoint // intersect 2 lines
{
	Line* firstParent;
	Line* secondParent;
public:
	IntersectionOfTwoLines(Object* object, Line* firstParent, Line* secondParent);
	void recreate(Equation* equation) override;
	void moveTo(Vector2f coords) override;
	~IntersectionOfTwoLines() override;
};

class UnitPoint;
class CentralProjection : public ConstructionPoint
{
	UnitCircle* firstParent;
	Point* secondParent;
	UnitPoint* thirdParent;
public:
	CentralProjection(Object* object, UnitCircle* first, Point* second, UnitPoint* third);
	void recreate(Equation* equation) override;
	void moveTo(Vector2f coords) override;
	~CentralProjection() override;
};

class Projection : public ConstructionPoint
{
	Point* firstParent;
	Line* secondParent;
public:
	Projection(Object* object, Point* first, Line* second);
	void recreate(Equation* equation) override;
	void moveTo(Vector2f coords) override;
	~Projection() override;
};

class ByTwoPointsAndScalar : public ConstructionPoint
{
protected:
	Point* firstParent;
	Point* secondParent;
	Scalar* thirdParent;
public:
	ByTwoPointsAndScalar(Object* object, Point* firstParent, Point* secondParent, Scalar* thirdParent);
	~ByTwoPointsAndScalar();
	void recreate(Equation* equation) override;
};

class ByLineAndScalar : public ByTwoPointsAndScalar
{
	Line* fourthParent;
public:
	ByLineAndScalar(Object* object, Point* firstParent, Point* secondParent, Scalar* thirdParent, Line* fourthParent);
	void moveTo(Vector2f coords) override;
	~ByLineAndScalar();
};

class ByCircleAndScalar : public ConstructionPoint
{
	UnitCircle* firstParent;
	Scalar* secondParent;
public:
	ByCircleAndScalar(Object* object, UnitCircle* firstParent, Scalar* secondParent);
	void moveTo(Vector2f coords) override;
	~ByCircleAndScalar();
	void recreate(Equation* equation) override;
};

class Pole : public ConstructionPoint
{
	Line* parent;
	void recreate(Equation* equation) override;
};

class ByTwoPoints : public ConstructionLine
{
	Point* firstParent;
	Point* secondParent;
public:
	ByTwoPoints(Object* object, Point* firstParent, Point* secondParent);
	~ByTwoPoints() override;
	void recreate(Equation* equation) override;
};

class UnitPoint;
class ByTwoUnitPoints : public ByTwoPoints
{
public:
	ByTwoUnitPoints(Object* object, UnitPoint* firstParent, UnitPoint* secondParent);
};

class PerpendicularBisector : public ConstructionLine
{
	Point* firstParent;
	Point* secondParent;
	void recreate(Equation* equation) override;
};

class Perpendicular : public ConstructionLine
{
	Point* firstParent;
	Line* secondParent;
public:
	Perpendicular(Object* object, Point* firstParent, Line* secondParent);
	~Perpendicular();
	void recreate(Equation* equation) override;
};

class Polar : public ConstructionLine
{
	Point* parent;
	void recreate(Equation* equation) override;
};

class Parallel : public ConstructionLine
{
	Line* firstParent;
	Point* secondParent;
public:
	Parallel(Object* object, Line* first, Point* second);
	~Parallel();
	void recreate(Equation* equation) override;
};

class Tangent : public ConstructionLine
{
	UnitCircle* firstParent;
	Point* secondParent;
public:
	Tangent(Object* object, UnitCircle* firstParent, Point* secondParent);
	~Tangent();
	void recreate(Equation* equation) override;
};

//VisibleObject - objects that you can see on the screen.
class VisibleObject : public Object
{
	Color visibleColor = Color::Black;
	const Color unvisibleColor = Color::Blue;
	void erase();
protected:
	ConstructionData* construction;
	virtual void reposition();
	bool visible = true;
	Color getColor();
public:
	virtual bool isNearby(Vector2f position) = 0;
	virtual void draw() = 0;
	virtual void drawDescription() = 0;
	void changeVisibility(bool newVisibility);
	void changeVisibility();
	void changeColor(Color color);
	bool getVisibility();
	bool isOnCircle();
	~VisibleObject();
};

class Parametr : public Object
{
};

class Scalar : public Parametr
{
public:
	Scalar(double value);
};

class ComplexScalar : public Parametr
{
public:
	ComplexScalar(Vector2f coord);
};

class Plane : public Parametr
{
private:
	Plane();
	static Plane* plane;
public:
	static Plane* getInstance();
};

//UnitCircle - is the only circle in the programme
//To get object use UnitCircle::getInstance() method
class UnitCircle : public VisibleObject
{
private:
	const double outlineThickness = unitSeg / 100;
	//Pattern singleton
	UnitCircle();
	static UnitCircle* unitCircle;
	double getDistance(Vector2f point);
	CircleShape shape;
public:
	static UnitCircle* getInstance();
	bool isNearby(Vector2f position);
	void draw();
	void drawDescription();
};

//Circle can only be constructed to prove the problem
class Circle : public VisibleObject
{
	const double outlineThickness = unitSeg / 100;
	CircleShape shape;
	void reposition() override;
public:
	bool isNearby(Vector2f position);
	Circle(Point* first, Point* second, Point* third, Point* fourth);
	void draw();
	void drawDescription();
};

class Point;
class Line : public VisibleObject
{
protected:
	double distance(Vector2f point);
	void reposition() override;
	void Init();
public:
	Line();
	//By two points
	Line(Point* first, Point* second);
	//Perpendicular
	Line(Point* first, Line* second);
	//Tangent
	Line(UnitCircle* first, Point* second);
	//Parallel
	Line(Line* first, Point* second);
	bool isNearby(Vector2f position);
	void draw() override;
	void drawDescription();
};

class UnitPoint;
class Chord : public Line
{
public:
	Chord(UnitPoint* first, UnitPoint* second);
};

class Point : public VisibleObject
{
protected:
	Point();
	const double pointSize = unitSeg/30;
	CircleShape shape = CircleShape(pointSize);
	double distance(Vector2f point);
	void reposition() override;
	void Init();
public:
	//static Vector2f intersectLines(Line::equationLine FirstEq, Line::equationLine SecondEq);
	Vector2f getCoordinate();
	virtual void moveTo(Vector2f coords);
	// By complex scalar (Point on the plain)
	Point(Vector2f position);
	// Intersection of two lines
	Point(Line* first, Line* second);
	// Point on a line (By line and scalar)
	Point(Line* line, Point* first, Point* second, Vector2f position);
	// Projection on a line
	Point(Point* point, Line* line);
	// Point by two points and scalar
	Point(Point* first, Point* second, Scalar* scalar);
	bool isNearby(Vector2f position) override;
	void draw() override;
	void drawDescription() override;
};


class CenterPoint : public Point
{
	void reposition() override;
	CenterPoint();
	static CenterPoint* centerPoint;
public:
	void moveTo(Vector2f coords) override;
	static CenterPoint* getInstance();
	~CenterPoint();
};

class UnitPoint : public Point
{
public:
	UnitPoint(UnitCircle* unitCircle, Vector2f position);
	UnitPoint(UnitCircle* unitCircle, Point* first, UnitPoint* second);
};