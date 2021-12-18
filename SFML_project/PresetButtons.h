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

class InterruptionChecker
{
private:
	Checker checker;
public:
	InterruptionChecker()
	{
		checker.checkMode();
	}
	bool checkInterruption()
	{
		if (!checker.checkMode())
		{
			Creation::Create();
			return false;
		}
		return true;
	}
	bool checkInterruption(VisibleObject* object)
	{
		if (!checker.checkMode())
		{
			Creation::Create();
			//delete object;
			return false;
		}
		return true;
	}
	bool checkInterruption(vector<Point*> objects)
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
	}
};

class Waiter
{
public:
	bool untilClick(InterruptionChecker interruptionChecker)
	{
		while (!Mouse::isButtonPressed(Mouse::Button::Left))
		{
			if (!interruptionChecker.checkInterruption())
				return 1;
			std::this_thread::sleep_for(std::chrono::nanoseconds(1000));
		}
		return 0;
	}
	void untilUnclick()
	{
		while (Mouse::isButtonPressed(Mouse::Button::Left))
		{
			std::this_thread::sleep_for(std::chrono::nanoseconds(1000));
		}
	}
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