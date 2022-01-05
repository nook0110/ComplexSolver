#pragma once
#include <SFML/Graphics.hpp>
#include <list>
#include "gui.h"
#include "Creation.h"

#include <iostream>

//The "heart" of the programme. This is file is about objects.

using namespace std;
using namespace sf;

extern double const epsilon;
extern double const unitSeg;
extern thread Creator;
extern MODES Mousemode;

class LineEquation;
class PointEquation;

const Vector2f projPoint = Vector2f(100, 100);
//equation for all objects
struct Equation
{
	virtual ~Equation();
	Equation();
	static Vector2f Projection(LineEquation lineEquation, PointEquation pointEquation);
	static Vector2f Projection(LineEquation lineEquation);
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

//objects - geomtrical shapes on the screen
class Object
{
private:
	list<Object*> children;
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

class ConstructionPoint : public ConstructionData
{
public:
	virtual void recreate(Equation* equation);
	virtual void move(Vector2f delta);
	virtual void moveTo(Vector2f coords);
};

class ConstructionLine : public ConstructionData
{
public:
	virtual void recreate(Equation* equation);
	virtual ~ConstructionLine();
};

class ByComplexScalar : public ConstructionPoint
{
	ComplexScalar* parent;
public:
	ByComplexScalar(Object* object, ComplexScalar* ComplexScalar);
	void recreate(Equation* equation) override;
	void move(Vector2f delta) override;
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
	void move(Vector2f delta) override;
	void moveTo(Vector2f coords) override;
	~IntersectionOfTwoLines() override;
};

class ByTwoPointsAndScalar : public ConstructionPoint
{
	Point* firstParent;
	Point* secondParent;
	Scalar* thirdParent;
public:
	ByTwoPointsAndScalar(Object* object, Point* firstParent, Point* secondParent, Scalar* thirdParent);
	~ByTwoPointsAndScalar();
	void recreate(Equation* equation) override;
};

class ByLineAndScalar : public ConstructionPoint
{
	Line* firstParent;
	Scalar* secondParent;
public:
	ByLineAndScalar(Object* object, Line* firstParent, Scalar* secondParent);
	void move(Vector2f delta) override;
	void moveTo(Vector2f coords) override;
	~ByLineAndScalar();
	void recreate(Equation* equation) override;
};

class ByCircleAndScalar : public ConstructionPoint
{
	UnitCircle* firstParent;
	Scalar* secondParent;
public:
	ByCircleAndScalar(Object* object, UnitCircle* firstParent, Scalar* secondParent);
	void move(Vector2f delta) override;
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
	Point* firstParent;
	Line* secondParent;
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


class VisibleObject : public Object
{
	const Color visibleColor = Color::Black;
	const Color unvisibleColor = Color::Blue;
	void erase();
protected:
	ConstructionData* construction;
	virtual void reposition();
	bool visible = true;
	Color getColor();
public:
	virtual bool isNearby(Vector2f mousePosition) = 0;
	virtual void draw() = 0;
	virtual void drawDescription() = 0;
	void changeVisibility(bool newVisibility);
	void changeVisibility();
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
	Scalar& operator+=(double deltaValue);
};

class ComplexScalar : public Parametr
{
public:
	ComplexScalar(Vector2f coord);
	ComplexScalar& operator += (Vector2f delta);
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
	bool isNearby(Vector2f mousePosition);
	void draw();
	void drawDescription() {}
};

class Point;
class Line : public VisibleObject
{
private:
	double distance(Vector2f point);
	void reposition() override;
public:
	Line(Point* first, Point* second);
	Line(Point* first, Line* second);
	Line(UnitCircle* first, Point* second);
	bool isNearby(Vector2f mousePosition);
	void draw() override;
	void drawDescription();
};

class Point : public VisibleObject
{
protected:
	Point();
	const double pointSize = 5;
	CircleShape shape = CircleShape(pointSize);
	double distance(Vector2f Point);
	void reposition() override;
	void Init();
public:
	//static Vector2f intersectLines(Line::equationLine FirstEq, Line::equationLine SecondEq);
	Vector2f getCoordinate();
	virtual void move(Vector2f delta);
	virtual void moveTo(Vector2f coords);
	Point(Vector2f mousePosition);
	Point(Line* first, Line* second);
	Point(Line* line, Vector2f mousePosition);
	Point(Line* line, Point* point);
	Point(UnitCircle* unitCircle, Vector2f mousePosition);
	Point(Point* first, Point* second, Scalar* scalar);
	bool isNearby(Vector2f mousePosition) override;
	void draw() override;
	void drawDescription() override;
};


class CenterPoint : public Point
{
	void reposition() override;
	CenterPoint();
	static CenterPoint* centerPoint;
public:
	void move(Vector2f delta) override;
	void moveTo(Vector2f coords) override;
	static CenterPoint* getInstance();
	~CenterPoint();
};