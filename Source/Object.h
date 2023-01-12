#pragma once
#include <list>
#include <SFML/Graphics.hpp>
#include "gui.h"
#include "algebraic.h"
#include "Creation.h"
#include "Printer.h"
#define _USE_MATH_DEFINES
#include <math.h>

#include <iostream>

//The "heart" of the programm. This file is about objects.


extern float const epsilon;
extern float const unitSeg;
extern std::thread Creator;
extern MODES Mousemode;
extern RenderWindow mainWindow;
extern View view;
struct LineEquation;
struct PointEquation;

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
	float radius;
	CircleEquation(Vector2f center, float radius);
};

struct LineEquation : public Equation
{
	float A, B, C;
	LineEquation(float A, float B, float C);
};

struct SegmentEquation : public Equation
{
	Vector2f pointFirst, pointSecond;
	SegmentEquation(Vector2f pointFirst, Vector2f pointSecond);
};

struct PointEquation : public Equation
{
	Vector2f point;
	PointEquation(Vector2f point);
};

class Description;
class ConstructionData;
//Objects - things that appear on the screen, such as UnitCircle, Circle, Points, Lines
class Object
{
private:
	std::list<Object*> children;
protected:
	//Update children position
	void reposeChildren();
	Equation* equation;
private:
	void deleteChildren();
public:
	void eraseChild(Object* child);
	void addChild(Object* child);
	//Delete all nullptr-s in Children list
	void clearChildren();
	Equation* getEquation();
	virtual ~Object();

private:
	Color visibleColor = Color::Black;
	const Color unvisibleColor = Color(0, 0, 255, 128);
	void erase();
public:
	ConstructionData* construction = nullptr;
protected:
	virtual void Reposition() = 0;
	bool visible = true;
public:
	Color getColor();
public:
	//Debug
	virtual void printExpr();
	//
	virtual float DistanceTo(Vector2f point) = 0;
	virtual bool IsNearby(Vector2f position) = 0;
	virtual void Draw() = 0;
	void SetVisibility(bool newVisibility);
	void SetVisibility();
	void setColor(Color color);
	bool getVisibility();
	bool isOnCircle();
protected:
	Description* description_ = nullptr;
	//If empty - No File created
	std::string equation_path_ = "";
	std::string TeX_;
	virtual std::string MakeTeX();
public:
	void updateTeX();
	virtual std::string getLowerCaseName();
	void SwitchDescription(Vector2f position);

public:
	void add();
	void del();

protected:
	bool highlighted = false;
public:
	virtual void highlight() { highlighted = true; };
	virtual void unhighlight() { highlighted = false; };
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
	std::string TeXFormula;
public:
	//Return equation of the objects 
	//The equation can be changed if parents were moved
	virtual void recreate(Equation* equation) = 0;
	virtual ~ConstructionData() {};
};

class ConstructionSegment : public ConstructionData
{
protected:
	ConstructionSegment(Object* object);
};

class ByTwoEnds : public ConstructionSegment
{
	Point* firstParent, * secondParent;
public:
	ByTwoEnds(Object* object, Point* first, Point* second);
	void recreate(Equation* equation) override;
	~ByTwoEnds();
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
	virtual void moveTo(Vector2f coords) = 0;
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
protected:
	Vector2f point;
	OnPlane(Object* object);
public:
	OnPlane(Point* object, Vector2f point);
	void recreate(Equation* equation) override;
	void moveTo(Vector2f coords) override;
	~OnPlane();
};

class Center : public OnPlane
{
public:
	Center(Point* object);
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
	void moveTo(Vector2f coords) {};
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
	void moveTo(Vector2f coords) {};
	~CentralProjection() override;
};

class Projection : public ConstructionPoint
{
	Point* firstParent;
	Line* secondParent;
public:
	Projection(Point* object, Point* first, Line* second);
	void recreate(Equation* equation) override;
	void moveTo(Vector2f coords) {};
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
	void moveTo(Vector2f coords) {};
	void recreate(Equation* equation) override;
};

class OnLine : public ConstructionPoint
{
	Point* firstParent;
	Point* secondParent;
	Line* thirdParent;
	float ratio;
public:
	OnLine(Point* object, Point* firstParent, Point* secondParent, float ratio, Line* fourthParent);
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
	UnitCircle* unit_circle;
	Chord* firstParent;
	UnitPoint* secondParent;
public:
	IntersectionParallelChord(Object* object, UnitCircle* unit_circle, Chord* firstParent, UnitPoint* secondParent);
	~IntersectionParallelChord();
	void moveTo(Vector2f coords) {};
	void recreate(Equation* equation) override;
};

class IntersectionPerpendicularChord : public ConstructionPoint
{
	UnitCircle* unit_circle;
	UnitPoint* firstParent;
	Chord* secondParent;
public:
	IntersectionPerpendicularChord(Object* object, UnitCircle* unit_circle, UnitPoint* firstParent, Chord* secondParent);
	~IntersectionPerpendicularChord();
	void moveTo(Vector2f coords) {};
	void recreate(Equation* equation) override;
};

class Pole : public ConstructionPoint
{
	Line* parent;
	void recreate(Equation* equation) override;
	void moveTo(Vector2f coords) {};
};

class Orthocenter : public ConstructionPoint
{
	UnitPoint* firstParent;
	UnitPoint* secondParent;
	UnitPoint* thirdParent;
public:
	Orthocenter(Object* object, UnitPoint* firstParent, UnitPoint* secondParent, UnitPoint* thirdParent);
	~Orthocenter();
	void moveTo(Vector2f coords) {};
	void recreate(Equation* equation) override;
};

class Incenter : public ConstructionPoint
{
	UnitPoint* firstParent;
	UnitPoint* secondParent;
	UnitPoint* thirdParent;
public:
	Incenter(Object* object, UnitPoint* firstParent, UnitPoint* secondParent, UnitPoint* thirdParent);
	~Incenter();
	void moveTo(Vector2f coords) {};
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
	void moveTo(Vector2f coords) {};
	void recreate(Equation* equation) override;
};

class ByTwoPoints : public ConstructionLine
{
protected:
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
public:
	UnitCircle* firstParent;
	Point* secondParent;
	Tangent(Object* object, UnitCircle* firstParent, Point* secondParent);
	~Tangent();
	void recreate(Equation* equation) override;
};

//UnitCircle - is the only circle in the programme
//To get object use UnitCircle::getInstance() method
class UnitCircle : public Object
{
private:
	const float outlineThickness = unitSeg / 100;
	//Pattern singleton
	UnitCircle();
	static UnitCircle* unit_circle;
	CircleShape shape;
public:
	float DistanceTo(Vector2f point);
	static UnitCircle* getInstance();
	void Reposition() {};
	bool IsNearby(Vector2f position);
	void Draw();
	void SwitchDescription();
};

//Circle can only be constructed to prove the problem
class Circle : public Object
{
	const float outlineThickness = unitSeg / 100;
	CircleShape shape;
	void Reposition() override;
public:
	float DistanceTo(Vector2f point);
	bool IsNearby(Vector2f position);
	Circle(Point* first, Point* second, Point* third, Point* fourth);
	void Draw();
	void SwitchDescription();
};

class LineSegment : public Object
{
	Vertex points_[2];
	void Reposition() override;
public:
	LineSegment(Point* first, Point* second);
	float DistanceTo(Vector2f point) { return 0; };
	bool IsNearby(Vector2f position) { return false; };
	void Draw();
};

class Point;
class Line : public Object
{
public:
	//DEBUG
	void printExpr();
	//
	Line() {};
	//By two points
	Line(Point* first, Point* second);
	//Tangent
	Line(UnitCircle* first, Point* second);
	//Parallel
	Line(Line* first, Point* second);
	// Perpendicular
	Line(Point* first, Line* second);

	float DistanceTo(Vector2f point);
	bool IsNearby(Vector2f position);
	void Draw() override;
	void SwitchDescription();
	void SetDotted(bool dotted_);
	std::string MakeTeX();
protected:
	void Reposition() override;
	void Init();
	bool dotted_ = false;
private:
	Vertex points_[2];
	float outline_thickness_ = 5;
	Color highlighted_color_ = Color(64, 64, 64, 64);
public:
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
	Color movableColor = Color(0, 128, 255);
	std::string pointName;
	Font font;
	unsigned int textSize = 90;
	Vector2f textScale = Vector2f(0.33f, 0.33f);
	Color textColor = Color::Black;
	Text nameText;
	Point();
	const float pointSize = unitSeg / 30;
	CircleShape shape = CircleShape(pointSize);
	void Reposition() override;
	void setName();
	void Init();
public:
	//DEBUG
	void printExpr();
	//
	Vector2f getCoordinate();
	virtual void moveTo(Vector2f coords);
	~Point();
	// On a plane
	Point(Vector2f position);
	// Intersection of two lines
	Point(Line* first, Line* second);
	// Point on a line
	Point(Line* points_, Point* first, Point* second, Vector2f position);
	// Projection on a line
	Point(Point* point, Line* points_);
	// Point by two points and scalar
	Point(Point* first, Point* second, std::pair<int, int> masses);
	// Rotation on 90 degrees
	Point(Point* center, Point* preimage, int sign);
	enum triangleCenter
	{
		ORTHOCENTER,
		INCENTER
	};
	// Orthocenter and Incenter
	Point(UnitPoint* first, UnitPoint* second, UnitPoint* third, triangleCenter center);
	// Barycenter
	Point(Point* first, Point* second, Point* third);
	float DistanceTo(Vector2f point);
	bool IsNearby(Vector2f position) override;
	bool contains(Vector2f position);
	void Draw() override;
	std::string getLowerCaseName() override;
	std::string MakeTeX() override;
private:
	float outline_thickness_ = 5;
	Color highlighted_color_ = Color(64, 64, 64, 64);
public:
	void highlight();
	void unhighlight();
};


class CenterPoint : public Point
{
	void Reposition() override;
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
	UnitPoint(UnitCircle* unit_circle, Vector2f position);
	UnitPoint(UnitCircle* unit_circle, Point* first, UnitPoint* second);
	// Intersection of parallel chord and Unit Circle (though a UnitPoint)
	UnitPoint(UnitCircle* unit_circle, Chord* first, UnitPoint* second);
	// Intersection of perpendicular chord and Unit Circle (though a UnitPoint)
	UnitPoint(UnitCircle* unit_circle, UnitPoint* first, Chord* second);
};