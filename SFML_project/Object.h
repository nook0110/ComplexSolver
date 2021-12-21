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

struct Equation
{
	virtual ~Equation();
};


struct LineEquation : public Equation
{
	double A, B, C;
	LineEquation(double _A, double _B, double _C);
};

struct PointEquation : public Equation
{
	Vector2f point;
	PointEquation(Vector2f _point);
};

struct ScalarEquation : public Equation
{
	double ratio;
	ScalarEquation(double _ratio);
};

struct ComplexScalarEquation : public Equation
{
	Vector2f point;
	ComplexScalarEquation(Vector2f _point);
};


class Object
{
public:
	list<Object*> children;
protected:
	Equation* equation;
private:
	void deleteChildren();
	void eraseChild(Object* child);
public:
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
	Equation* recreate() override;
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
	ByTwoPoints(Point* _firstParent, Point* _secondParent);
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
	Line* firstParent;
	Point* secondParent;
public:
	Perpendicular(Line* _firstParent, Point* _secondParent);
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
	virtual bool isNearby(Vector2f mouseCoord) = 0;
	virtual void draw() = 0;
	virtual void drawDescription() = 0;
	//virtual ~VisibleObject();
};

class ParametrObject : public Object
{

};

class Scalar : public ParametrObject
{
};

class ComplexScalar : public Scalar
{
public:
	ComplexScalar(Vector2f coord);
};

class Plane : public ParametrObject
{
private:
	Plane();
	static Plane* plane;
	//static Color color;
	//double getDistance(Vector2f point);
	//RectangleShape Shape;
	//double const OutlineThickness = unitSeg / 10;
public:
	static Plane* getInstance();
	//bool isNearby(Vector2f mouseCoord);
	//void draw();
	//void drawDescription() {}
	//static Color getColor();
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
	bool isNearby(Vector2f mouseCoord);
	void draw();
	void drawDescription() {}
};



class Point;
class Line : public VisibleObject
{
private:


private:


public:
	//double distance(Vector2f point);
public:
	Line(Point* first, Point* second);
	Line(Line* first, Point* second);
	bool isNearby(Vector2f mouseCoord);
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
public:
	//static Vector2f intersectLines(Line::equationLine FirstEq, Line::equationLine SecondEq);
	Vector2f getCoordinate();
	Point(Vector2f mouseCoord);
	//Point(Line* first, Line* second);
	//Point(Line* line, Vector2f mouseCoord);
	Point(Line* line, Point* point);
	Point(UnitCircle* unitCircle, Vector2f mouseCoord);
	bool isNearby(Vector2f mouseCoord) override;
	void draw() override;
	void drawDescription() override;
};


/*
Point:
by parametrObject
by 2 lines;
by 2 points and parametrObject;
by line and unit circle(pole)
Line:
by 2 points;
by 2 points;(perp bisector)
by point and line(perp);
by point and unit circle(polar);
by point and line(||);
by point and circle;

*/




