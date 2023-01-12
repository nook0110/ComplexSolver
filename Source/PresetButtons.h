#pragma once
#include "gui.h"
#include "Creation.h"
#include "Prover.h"
//Preset of Buttons with functions
extern FloatRect mainWindowRect;


class UnitCircle;
class InterruptionChecker
{
private:
	Checker checker;
public:
	InterruptionChecker();
	bool checkInterruption();
};

class Waiter
{
private:
	const int sleepingTime = 1000;
	InterruptionChecker interruptionChecker;
	bool mouseOnTheScreen();
public:
	void sleep();
	bool untilClick();
};

class Finder
{
public:
	Description* nearbyDescription(Vector2f mouse_position);
	Object* nearbyNotUnitCircleObject(Vector2f mouse_position);
	Object* nearbyObject(Vector2f mouse_position);
	Point* nearbyConstructedPoint(Vector2f mouse_position);
	UnitPoint* nearbyConstructedPointOnCircle(Vector2f mouse_position);
	Point* nearbyNewPoint(Vector2f mouse_position);
	UnitPoint* nearbyNewPointOnCircle(Vector2f mouse_position);
	Line* nearbyLine(Vector2f mouse_position);
	std::vector<Line*> nearbyLines(Vector2f mouse_position);
	Point* nearbyIntersection(Vector2f mouse_position);
	UnitCircle* nearbyUnitCircle(Vector2f mouse_position);
};

class Highlighter
{
	static std::list<Object*> highlighted;
public:
	static void highlight(Object* object);
	static void unhighlight();
};