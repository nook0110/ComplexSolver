#pragma once
#include "Button.h"
#include "Creation.h"

//Preset of Buttons with functions

extern Menu mainMenu;
extern FloatRect mainWindowRect;
class UnitCircle;
class InterruptionChecker
{
private:
	Checker checker;
public:
	InterruptionChecker();
	bool checkInterruption();
	bool checkInterruption(VisibleObject* object);
};

class Waiter
{
private:
	const int sleepingTime = 100;
	InterruptionChecker interruptionChecker;
	bool mouseOnTheScreen();
public:
	void sleep();
	bool untilClick();
};

class Finder
{
public:
	VisibleObject* nearbyVisibleObject(Vector2f mousePosition);
	VisibleObject* nearbyObject(Vector2f mousePosition);
	Point* nearbyConstructedPoint(Vector2f mousePosition);
	Point* nearbyConstructedPointOnCircle(Vector2f mousePosition);
	Point* nearbyNewPoint(Vector2f mousePosition);
	Point* nearbyNewPointOnCircle(Vector2f mousePosition);
	Line* nearbyLine(Vector2f mousePosition);
	pair<Line*, Line*> nearbyLines(Vector2f mousePosition);
	Point* nearbyIntersection(Vector2f mousePosition);
	UnitCircle* nearbyUnitCircle(Vector2f mousePosition);
};