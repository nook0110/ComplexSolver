#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <future>
#include <functional>
#include <list>
#include <thread>
#include "gui.h"
#include "Creation.h"
using namespace std;
using namespace sf;

extern double const epsilon;
extern double const unitSeg;
extern thread Creator;
extern MODES Mousemode;

class LineEquation;
class PointEquation;
struct Equation
{
	virtual ~Equation();
public:
	static Vector2f Projection(LineEquation lineEquation, PointEquation pointEquation);
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
};

struct ComplexScalarEquation : public Equation
{
	Vector2f point;
	ComplexScalarEquation(Vector2f point);
};


class Object
{
public:
	list<Object*> children;
protected:
	Equation* equation;
private:
	void deleteChildren();
public:
	void eraseChild(Object* child);
	void addChild(Object* child);
	Equation* getEquation();
};

class VisibleObject;
class Line;
class Point;
class UnitCircle;
class ParametrObject;
class Scalar;
class ComplexScalar;

class ConstructionData
{
public:
	static list<VisibleObject*> allVisibleObjects;
	static list<ParametrObject*> allParametrs;
	virtual Equation* recreate();
};

class ConstructionPoint : public ConstructionData
{
	Equation* recreate() override;
};

class ConstructionLine : public ConstructionData
{
	Equation* recreate() override;
};

class ByComplexScalar : public ConstructionPoint
{
	ComplexScalar* parent;
public:
	ByComplexScalar(ComplexScalar* ComplexScalar);
	Equation* recreate() override;
};

class IntersectionOfTwoLines : public ConstructionPoint // intersect 2 lines
{
	Line* firstParent;
	Line* secondParent;
public:
	IntersectionOfTwoLines(Line* firstParent, Line* secondParent);
	Equation* recreate() override;
};

class ByTwoPointsAndScalar : public ConstructionPoint
{
	Point* firstParent;
	Point* secondParent;
	Scalar* thirdParent;
public:
	ByTwoPointsAndScalar(Point* firstParent, Point* secondParent, Scalar* thirdParent);
	Equation* recreate() override;
};

class ByLineAndScalar : public ConstructionPoint
{
	Line* firstParent;
	Scalar* secondParent;
public:
	ByLineAndScalar(Line* firstParent, Scalar* secondParent);
	Equation* recreate() override;
};

class ByCircleAndScalar : public ConstructionPoint
{
	UnitCircle* firstParent;
	Scalar* secondParent;
public:
	ByCircleAndScalar(UnitCircle* firstParent, Scalar* secondParent);
	//Equation* recreate() override;
};

class Pole : public ConstructionPoint
{
	Line* parent;
	Equation* recreate() override;
};

class ByTwoPoints : public ConstructionLine
{
	Point* firstParent;
	Point* secondParent;
public:
	ByTwoPoints(Point* firstParent, Point* secondParent);
	Equation* recreate() override;
};

class PerpendicularBisector : public ConstructionLine
{
	Point* firstParent;
	Point* secondParent;
	Equation* recreate() override;
};

class Perpendicular : public ConstructionLine
{
	Point* firstParent;
	Line* secondParent;
public:
	Perpendicular(Point* firstParent, Line* secondParent);
	Equation* recreate() override;
};

class Polar : public ConstructionLine
{
	Point* parent;
	Equation* recreate() override;
};

class Parallel : public ConstructionLine
{
	Point* firstParent;
	Line* secondParent;
	Equation* recreate() override;
};

class Tangent : public ConstructionLine
{
	Point* firstParent;
	UnitCircle* secondParent;
	Equation* recreate() override;
};


class VisibleObject : public Object
{
	list<VisibleObject*>::iterator it;
	void insert();
	void erase();
protected:
	ConstructionData* construction;

public:
	virtual bool isNearby(Vector2f mousePosition) = 0;
	virtual void draw() = 0;
	virtual void drawDescription() = 0;
	//virtual ~VisibleObject();
};

class ParametrObject : public Object
{
};


class Scalar : public ParametrObject
{
	double value;
public:
	Scalar(double value);
};

class ComplexScalar : public ParametrObject
{
public:
	ComplexScalar(Vector2f coord);
};

class Plane : public ParametrObject
{
private:
	Plane();
	static Plane* plane;
public:
	static Plane* getInstance();
};


class UnitCircle : public VisibleObject
{
private:
	UnitCircle();
	static UnitCircle* unitCircle;
	double getDistance(Vector2f point);
	CircleShape Shape;
	double const OutlineThickness = unitSeg / 100;
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
public:
	Line(Point* first, Point* second);
	Line(Point* first, Line* second);
	bool isNearby(Vector2f mousePosition);
	void draw() override;
	void drawDescription();
};

class Point : public VisibleObject
{
private:
	const double pointSize = 5;
	/*
	class equation
	*/
	CircleShape shape = CircleShape(pointSize);
	double distance(Vector2f Point);
	void Init();
public:
	//static Vector2f intersectLines(Line::equationLine FirstEq, Line::equationLine SecondEq);
	Vector2f getCoordinate();
	Point(Vector2f mousePosition);
	Point(Line* first, Line* second);
	Point(Line* line, Vector2f mousePosition);
	Point(Line* line, Point* point);
	Point(UnitCircle* unitCircle, Vector2f mousePosition);
	bool isNearby(Vector2f mousePosition) override;
	void draw() override;
	void drawDescription() override;
};