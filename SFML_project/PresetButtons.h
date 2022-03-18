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
	Description* nearbyDescription(Vector2f mousePosition);
	Object* nearbyNotUnitCircleObject(Vector2f mousePosition);
	Object* nearbyObject(Vector2f mousePosition);
	Point* nearbyConstructedPoint(Vector2f mousePosition);
	UnitPoint* nearbyConstructedPointOnCircle(Vector2f mousePosition);
	Point* nearbyNewPoint(Vector2f mousePosition);
	UnitPoint* nearbyNewPointOnCircle(Vector2f mousePosition);
	Line* nearbyLine(Vector2f mousePosition);
	std::vector<Line*> nearbyLines(Vector2f mousePosition);
	Point* nearbyIntersection(Vector2f mousePosition);
	UnitCircle* nearbyUnitCircle(Vector2f mousePosition);
};

class Highlighter
{
	static std::list<Object*> highlighted;
public:
	static void highlight(Object* object);
	static void unhighlight();
};