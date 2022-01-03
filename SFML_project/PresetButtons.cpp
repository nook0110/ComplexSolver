
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <thread>
#include <functional>
#include <future>
#include "Button.h"
#include "Object.h"
#include "Creation.h"
#include <ctime>
#include "PresetButtons.h"
using namespace std;
using namespace sf;

extern RenderWindow window;//
extern View view;//
extern Vector2i maxTextureResolution;//
extern MODES Mousemode;//
extern Menu mainMenu;//
extern Plane* plane;//
list<VisibleObject*> ConstructionData::allVisibleObjects;

Button moveButton = Button(Vector2f(10, 10), Vector2f(100, 100), &window,
	"C:\\Textures\\SFML_project\\MoveButton.jpg", Vector2i(0, 0), maxTextureResolution,
	MODE_MOVE, []() {
		Waiter wait;
		InterruptionChecker interruptionChecker;
		Finder find;
		if (wait.untilClick())
		{
			return;
		}
		Vector2f mousePosition(Vector2f(Mouse::getPosition(window)));
		Point* point = find.nearbyConstructedPoint((window).mapPixelToCoords(Vector2i(mousePosition), view));
		while (Mouse::isButtonPressed(Mouse::Button::Left))
		{
			if (!interruptionChecker.checkInterruption())
				return;
			Vector2f delta = Vector2f(Mouse::getPosition(window)) - mousePosition;
			Vector2f Scale;
			Scale.x = view.getSize().x / window.getSize().x;
			Scale.y = view.getSize().y / window.getSize().y;
			delta.x *= Scale.x;
			delta.y *= Scale.y;
			if (point)
			{
				point->moveTo((window).mapPixelToCoords(Vector2i(mousePosition), view));

			}
			else
			{
				view.move(-delta);
			}
			mousePosition = Vector2f(Mouse::getPosition(window));
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		Creation::Create();
		return;
	});

Button pointButton = Button(Vector2f(120, 10), Vector2f(100, 100), &window,
	"C:\\Textures\\SFML_project\\PointButton.jpg", Vector2i(0, 0), maxTextureResolution,
	MODE_POINT, []() {
		Waiter wait;
		Finder find;
		if (wait.untilClick())
		{
			return;
		}
		Vector2f mousePosition = (window).mapPixelToCoords(Mouse::getPosition(window), view);
		Point* point = find.nearbyConstructedPoint(mousePosition);
		if (point)
		{
			Creation::Create();
			return;
		}
		point = find.nearbyNewPoint(mousePosition);
		Creation::Create();
		return;
	});


Button lineButton = Button(Vector2f(230, 10), Vector2f(100, 100), &window,
	"C:\\Textures\\SFML_project\\LineButton.jpg", Vector2i(0, 0), maxTextureResolution,
	MODE_LINE, []() {
		Waiter wait;
		Finder find;
		const int twotimes = 2;
		pair<Point*, Point*> points(nullptr, nullptr);
		for (int i = 0; i < twotimes; ++i)
		{
			if (wait.untilClick())
			{
				return;
			}
			Vector2f mousePosition = (window).mapPixelToCoords(Mouse::getPosition(window), view);
			Point* point = find.nearbyConstructedPoint(mousePosition);
			if (!point)
			{
				point = find.nearbyNewPoint(mousePosition);
			}
			if (get<0>(points))
			{
				if (point != get<0>(points))
				{
					points.second = point;
					break;
				}
				else
				{
					i--;
					continue;
				}
			}
			else
			{
				points.first = point;
				continue;
			}
		}
		ConstructionData::allVisibleObjects.push_back(new Line(points.first, points.second));
		Creation::Create();
		return;
	});
Button perpendicularButton = Button(Vector2f(340, 10), Vector2f(100, 100), &window,
	"C:\\Textures\\SFML_project\\PerpendicularButton.jpg", Vector2i(0, 0), maxTextureResolution,
	MODE_PERPENDICULAR, []() {
		Waiter wait;
		InterruptionChecker interruptionChecker;
		Finder find;
		if (wait.untilClick())
		{
			return;
		}
		Vector2f mousePosition = (window).mapPixelToCoords(Mouse::getPosition(window), view);
		Point* point = find.nearbyConstructedPoint(mousePosition);
		if (!point)
		{
			point = find.nearbyNewPoint(mousePosition);
		}
		Line* line = nullptr;
		while (!line)
		{
			if (wait.untilClick())
			{
				return;
			}
			mousePosition = (window).mapPixelToCoords(Mouse::getPosition(window), view);
			line = find.nearbyLine(mousePosition);
			wait.sleep();
		}
		Creation::Create();
		ConstructionData::allVisibleObjects.push_back(new Line(point, line));
		return;

	});
Button midPointButton = Button(Vector2f(450, 10), Vector2f(100, 100), &window,
	"C:\\Textures\\SFML_project\\Test.jpg", Vector2i(0, 0), maxTextureResolution,
	MODE_NOTHING, []() {
		return nullptr;
	});
Button tangentButton = Button(Vector2f(450, 10), Vector2f(100, 100), &window,
	"C:\\Textures\\SFML_project\\TangentButton.jpg", Vector2i(0, 0), maxTextureResolution,
	MODE_TANGENT, []() {
		Waiter wait;
		Finder find;
		if (wait.untilClick())
		{
			return;
		}
		Vector2f mousePosition = (window).mapPixelToCoords(Mouse::getPosition(window), view);
		Point* point = find.nearbyConstructedPointOnCircle(mousePosition);
		if (!point)
		{
			point = find.nearbyNewPointOnCircle(mousePosition);
		}
		if (point)
		{
			ConstructionData::allVisibleObjects.push_back(new Line(unitCircle, point));
		}
		Creation::Create();
		return;
	});

Button deleteButton = Button(Vector2f(560, 10), Vector2f(100, 100), &window,
	"C:\\Textures\\SFML_project\\DeleteButton.jpg", Vector2i(0, 0), maxTextureResolution,
	MODE_DELETE, []() {
		Waiter wait;
		Finder find;
		if (wait.untilClick())
		{
			return;
		}
		Vector2f mousePosition = (window).mapPixelToCoords(Mouse::getPosition(window), view);
		Object* object = find.nearbyObject(mousePosition);
		if (object)
		{
			delete object;
		}
		Creation::Create();
		return;
	});

Object* Finder::nearbyObject(Vector2f mousePosition)
{
	Point* point = nearbyConstructedPoint(mousePosition);
	if (point)
	{
		return point;
	}
	Line* line = nearbyLine(mousePosition);
	if (line)
	{
		return line;
	}
	for (VisibleObject* object : ConstructionData::allVisibleObjects)
	{
		if (object && object->isNearby(mousePosition))
		{
			return object;
		}
	}
	return nullptr;
}

Point* Finder::nearbyConstructedPoint(Vector2f mousePosition)
{
	for (VisibleObject* object : ConstructionData::allVisibleObjects)
	{
		Point* point = dynamic_cast<Point*>(object);
		if (point && point->isNearby(mousePosition))
		{
			return point;
		}
	}
	return nullptr;
}

Point* Finder::nearbyConstructedPointOnCircle(Vector2f mousePosition)
{
	for (VisibleObject* object : ConstructionData::allVisibleObjects)
	{
		Point* point = dynamic_cast<Point*>(object);
		if (point)
		{
			if (point->isNearby(mousePosition) && point->isOnCircle())
			{
				return point;
			}
		}
	}
	return nullptr;
}

Point* Finder::nearbyNewPoint(Vector2f mousePosition)
{
	Point* point = nearbyIntersection(mousePosition);
	if (point)
	{
		return point;
	}
	Line* line = nearbyLine(mousePosition);
	if (line)
	{
		return new Point(line, mousePosition);
	}
	UnitCircle* unitCircle = UnitCircle::getInstance();
	if (unitCircle->isNearby(mousePosition))
	{
		return new Point(unitCircle, mousePosition);
	}
	return new Point(mousePosition);
}

Point* Finder::nearbyNewPointOnCircle(Vector2f mousePosition)
{
	if (unitCircle->isNearby(mousePosition))
	{
		return new Point(unitCircle, mousePosition);
	}
}

Line* Finder::nearbyLine(Vector2f mousePosition)
{
	for (VisibleObject* object : ConstructionData::allVisibleObjects)
	{
		Line* line = dynamic_cast<Line*>(object);
		if (line && line->isNearby(mousePosition))
		{
			return line;
		}
	}
	return nullptr;
}

pair<Line*, Line*> Finder::nearbyLines(Vector2f mousePosition)
{
	pair<Line*, Line*> lines = { nullptr,nullptr };
	for (VisibleObject* object : ConstructionData::allVisibleObjects)
	{
		Line* line = dynamic_cast<Line*>(object);
		if (line)
		{
			if (line->isNearby(mousePosition))
			{
				(get<0>(lines) ? lines.second : lines.first) = line;
				if (get<1>(lines)) return lines;
			}
		}
	}
	return { nullptr,nullptr };
}



Point* Finder::nearbyIntersection(Vector2f mousePosition)
{
	pair<Line*, Line*> lines = nearbyLines(mousePosition);
	if (lines.first)
	{
		Point* point = new Point(lines.first, lines.second);
		if (point->isNearby(mousePosition))
		{
			return point;
		}
	}
	return nullptr;
}

UnitCircle* Finder::nearbyUnitCircle(Vector2f mousePosition)
{
	return nullptr;
}

InterruptionChecker::InterruptionChecker()
{
	checker.checkMode();
}

bool InterruptionChecker::checkInterruption()
{
	if (!checker.checkMode())
	{
		Creation::Create();
		return false;
	}
	return true;
}

bool InterruptionChecker::checkInterruption(VisibleObject* object)
{
	if (!checker.checkMode())
	{
		Creation::Create();
		//delete object;
		return false;
	}
	return true;
}

bool Waiter::mouseOnTheScreen()
{
	Vector2f mousePosition = (window).mapPixelToCoords(Mouse::getPosition(window));
	Vector2f center = view.getCenter();
	Vector2f size = view.getSize();
	Vector2f leftUpCorner = center - size / 2.f;
	Vector2f rightDownCorner = (center + size / 2.f);
	rightDownCorner.y += size.y * (mainWindowRect.height - mainWindowRect.top) - size.y;
	return mousePosition.x > leftUpCorner.x && mousePosition.y > leftUpCorner.y &&
		mousePosition.x < rightDownCorner.x&& mousePosition.y < rightDownCorner.y;
}

void Waiter::sleep()
{
	std::this_thread::sleep_for(std::chrono::nanoseconds(sleepingTime));
}

bool Waiter::untilClick()
{
	while (Mouse::isButtonPressed(Mouse::Button::Left))
	{
		if (!interruptionChecker.checkInterruption())
			return true;
		sleep();
	}
	while (!Mouse::isButtonPressed(Mouse::Button::Left) || mainMenu.mouseOnMenu() || !mouseOnTheScreen())
	{
		if (!interruptionChecker.checkInterruption())
			return true;
		sleep();
	}
	return false;
}
