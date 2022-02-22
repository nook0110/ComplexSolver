#pragma once
#include <list>
#include <SFML/Graphics.hpp>
#include "gui.h"
#include "algebraic.h"
#include "Creation.h"

#include <iostream>
//The "heart" of the programm. This is file is about objects.

using namespace sf;

extern double const epsilon;
extern double const unitSeg;
extern std::thread Creator;
extern MODES Mousemode;
extern RenderWindow mainWindow;
class LineEquation;
class PointEquation;

Vector2f projectionOnLine(LineEquation lineEquation, PointEquation pointEquation);

//Equation for all objects
struct Equation
{
	virtual ~Equation();
	Equation();
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

class ConstructionData;
//Objects - things that appear on the screen, such as UnitCircle, Circle, Points, Lines
class Object
{
private:
	std::list<Object*> children;
protected:
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

private:
	Color visibleColor = Color::Black;
	const Color unvisibleColor = Color::Blue;
	void erase();
public:
	ConstructionData* construction;
protected:
	virtual void reposition();
	bool visible = true;
	Color getColor();
public:
	//Debug
	virtual void printExpr();
	//
	virtual double distance(Vector2f point);
	virtual bool isNearby(Vector2f position) = 0;
	virtual void draw() = 0;
	virtual void drawDescription() = 0;
	void changeVisibility(bool newVisibility);
	void changeVisibility();
	void changeColor(Color color);
	bool getVisibility();
	bool isOnCircle();
};

class Object;
class Line;
class Point;
class UnitCircle;

//Data - how object was created (By two points and so on)
class ConstructionData
{
protected:
	Object* object;
	ConstructionData(Object* object);
public:
	//Return equation of the objects 
	//The equation can be changed if parents were moved
	virtual void recreate(Equation* equation);
	virtual ~ConstructionData();
};

class ConstructionCircle : public ConstructionData
{
protected:
	ConstructionCircle(Object* object);
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
protected:
	ConstructionPoint(Object* object);
public:
	expr coord;
	virtual void moveTo(Vector2f coords);
};

class ConstructionLine : public ConstructionData
{
protected:
	ConstructionLine(Object* object);
public:
	expr z_coef, z_conj_coef, free_coef;
};

class OnPlane : public ConstructionPoint
{
	Vector2f point;
public:
	OnPlane(Point* object, Vector2f point);
	void recreate(Equation* equation) override;
	void moveTo(Vector2f coords) override;
	~OnPlane();
};

class IntersectionOfTwoLines : public ConstructionPoint // intersect 2 lines
{
	Line* firstParent;
	Line* secondParent;
public:
	IntersectionOfTwoLines(Point* object, Line* firstParent, Line* secondParent);
	void recreate(Equation* equation) override;
	void moveTo(Vector2f coords) override;
	~IntersectionOfTwoLines() override;
};

class Rotation90 : public ConstructionPoint
{
	Point* firstParent;
	Point* secondParent;
	int sign;
public:
	Rotation90(Object* object, Point* firstParent, Point* secondParent, int sign);
	void recreate(Equation* equation) override;
	~Rotation90();
};

class UnitPoint;
class CentralProjection : public ConstructionPoint
{
	UnitCircle* firstParent;
	Point* secondParent;
	UnitPoint* thirdParent;
public:
	CentralProjection(UnitPoint* object, UnitCircle* first, Point* second, UnitPoint* third);
	void recreate(Equation* equation) override;
	void moveTo(Vector2f coords) override;
	~CentralProjection() override;
};

class Projection : public ConstructionPoint
{
	Point* firstParent;
	Line* secondParent;
public:
	Projection(Point* object, Point* first, Line* second);
	void recreate(Equation* equation) override;
	void moveTo(Vector2f coords) override;
	~Projection() override;
};

class byTwoPointsFixedRatio : public ConstructionPoint
{
private:
	Point* firstParent;
	Point* secondParent;
	const std::pair<int, int> masses;
public:
	byTwoPointsFixedRatio(Object* object, Point* firstParent, Point* secondParent, std::pair<int, int> masses);
	~byTwoPointsFixedRatio();
	void recreate(Equation* equation) override;
};

class OnLine : public ConstructionPoint
{
	Point* firstParent;
	Point* secondParent;
	Line* thirdParent;
	float ratio;
public:
	OnLine(Object* object, Point* firstParent, Point* secondParent, float ratio, Line* fourthParent);
	void moveTo(Vector2f coords) override;
	void recreate(Equation* equation) override;
	~OnLine();
};

class OnCircle : public ConstructionPoint
{
	UnitCircle* firstParent;
	float angle;
public:
	OnCircle(Point* object, UnitCircle* firstParent, float angle);
	void moveTo(Vector2f coords) override;
	~OnCircle();
	void recreate(Equation* equation) override;
};

class Chord;
class IntersectionParallelChord : public ConstructionPoint
{
	UnitCircle* unitCircle;
	Chord* firstParent;
	UnitPoint* secondParent;
public:
	IntersectionParallelChord(Object* object, UnitCircle* unitCircle, Chord* firstParent, UnitPoint* secondParent);
	~IntersectionParallelChord();
	void recreate(Equation* equation) override;
};

class IntersectionPerpendicularChord : public ConstructionPoint
{
	UnitCircle* unitCircle;
	UnitPoint* firstParent;
	Chord* secondParent;
public:
	IntersectionPerpendicularChord(Object* object, UnitCircle* unitCircle, UnitPoint* firstParent, Chord* secondParent);
	~IntersectionPerpendicularChord();
	void recreate(Equation* equation) override;
};

class Pole : public ConstructionPoint
{
	Line* parent;
	void recreate(Equation* equation) override;
};

class Orthocenter : public ConstructionPoint
{
	UnitPoint* firstParent;
	UnitPoint* secondParent;
	UnitPoint* thirdParent;
public:
	Orthocenter(Object* object, UnitPoint* firstParent, UnitPoint* secondParent, UnitPoint* thirdParent);
	~Orthocenter();
	void recreate(Equation* equation) override;
};

class Barycenter : public ConstructionPoint
{
	Point* firstParent;
	Point* secondParent;
	Point* thirdParent;
public:
	Barycenter(Object* object, Point* firstParent, Point* secondParent, Point* thirdParent);
	~Barycenter();
	void recreate(Equation* equation) override;
};

class ByTwoPoints : public ConstructionLine
{
protected:
	Point* firstParent;
	Point* secondParent;
	ByTwoPoints(Object* object);
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

//UnitCircle - is the only circle in the programme
//To get object use UnitCircle::getInstance() method
class UnitCircle : public Object
{
private:
	const double outlineThickness = unitSeg / 100;
	//Pattern singleton
	UnitCircle();
	static UnitCircle* unitCircle;
	CircleShape shape;
public:
	double distance(Vector2f point);
	static UnitCircle* getInstance();
	bool isNearby(Vector2f position);
	void draw();
	void drawDescription();
};

//Circle can only be constructed to prove the problem
class Circle : public Object
{
	const double outlineThickness = unitSeg / 100;
	CircleShape shape;
	void reposition() override;
public:
	double distance(Vector2f point);
	bool isNearby(Vector2f position);
	Circle(Point* first, Point* second, Point* third, Point* fourth);
	void draw();
	void drawDescription();
};

class LineSegment : public Object
{
	Vertex line[2];
	void reposition() override;
public:
	double distance(Vector2f point);
	bool isNearby(Vector2f position);
	LineSegment(Point* first, Point* second);
	void draw();
};

class Point;
class Line : public Object
{
protected:
	void reposition() override;
	void Init();
public:
	//DEBUG
	void printExpr();
	//
	Line();
	//By two points
	Line(Point* first, Point* second);
	//Tangent
	Line(UnitCircle* first, Point* second);
	//Parallel
	Line(Line* first, Point* second);
	double distance(Vector2f point);
	bool isNearby(Vector2f position);
	void draw() override;
	void drawDescription();
};

class UnitPoint;
class Chord : public Line
{
public:
	//By two UnitPoints
	Chord(UnitPoint* first, UnitPoint* second);
};

class Point : public Object
{
protected:
	std::string pointName;
	Font font;
	unsigned int textSize = 30;
	Text nameText;
	Point();
	const double pointSize = unitSeg / 30;
	CircleShape shape = CircleShape(pointSize);
	void reposition() override;
	void setName();
	void Init();
public:
	//DEBUG
	void printExpr();
	//
	Vector2f getCoordinate();
	virtual void moveTo(Vector2f coords);

	// On a plane
	Point(Vector2f position);
	// Intersection of two lines
	Point(Line* first, Line* second);
	// Point on a line
	Point(Line* line, Point* first, Point* second, Vector2f position);
	// Projection on a line
	Point(Point* point, Line* line);
	// Point by two points and scalar
	Point(Point* first, Point* second, std::pair<int,int> masses);
	// Rotation on 90 degrees
	Point(Point* center, Point* preimage, int sign);
	// Orthocenter
	Point(UnitPoint* first, UnitPoint* second, UnitPoint* third);
	// Barycenter
	Point(Point* first, Point* second, Point* third);
	double distance(Vector2f point);
	bool isNearby(Vector2f position) override;
	void draw() override;
	std::string getLowerCaseName();
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
	// Intersection of parallel chord and Unit Circle (though a UnitPoint)
	UnitPoint(UnitCircle* unitCircle, Chord* first, UnitPoint* second);
	// Intersection of perpendicular chord and Unit Circle (though a UnitPoint)
	UnitPoint(UnitCircle* unitCircle, UnitPoint* first, Chord* second);
};