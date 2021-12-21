#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <thread>
#include <functional>
#include <future>
#include "Button.h"
#include "Creation.h"
#include <ctime>

extern Menu mainMenu;

class InterruptionChecker
{
private:
	Checker checker;
public:
	InterruptionChecker();
	bool checkInterruption();
	bool checkInterruption(VisibleObject* object);
	/*bool checkInterruption(vector<Point*> objects)
	{
		if (!checker.checkMode())
		{
			Creation::Create();
			for (auto object : objects)
			{
				//delete object;
			}
			return false;
		}
		return true;
	}*/
};

class Waiter
{
private:
	InterruptionChecker interruptionChecker;
public:
	bool untilClick();
};

class Finder
{
public:
	Point* nearbyPoint(Vector2f mousePosition);
	Line* nearbyLine(Vector2f mousePosition);
	pair<Line*, Line*> nearbyLines(Vector2f mousePosition);
	Point* nearbyIntersection(Vector2f mousePosition);
	UnitCircle* nearbyUnitCircle(Vector2f mousePosition);
};