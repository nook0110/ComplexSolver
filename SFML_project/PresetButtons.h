#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <thread>
#include <functional>
#include <future>
#include <iostream>
#include "Button.h"
#include "Creation.h"
#include <ctime>

extern Menu mainMenu;
extern FloatRect mainWindowRect;
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
	InterruptionChecker interruptionChecker;
	bool mouseOnTheScreen();
public:
	bool untilClick();
};

class Finder
{
public:
	Point* nearbyConstructedPoint(Vector2f mousePosition);
	Point* nearbyConstructedPointOnCircle(Vector2f mousePosition);
	Point* nearbyNewPoint(Vector2f mousePosition);
	Line* nearbyLine(Vector2f mousePosition);
	pair<Line*, Line*> nearbyLines(Vector2f mousePosition);
	Point* nearbyIntersection(Vector2f mousePosition);
	UnitCircle* nearbyUnitCircle(Vector2f mousePosition);
};