
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <thread>
#include <functional>
#include <future>
#include "gui.h"
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
extern Plane* plane;//
list<VisibleObject*> Drawer::allVisibleObjects;

Button moveButton = Button(Vector2f(10, 10), Vector2f(100, 100), &window,
	"Textures\\SFML_project\\MoveButton.jpg", Vector2i(0, 0), maxTextureResolution,
	MODE_MOVE, [&]() {
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
	"Textures\\SFML_project\\PointButton.jpg", Vector2i(0, 0), maxTextureResolution,
	MODE_POINT, [&]() {
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
	"Textures\\SFML_project\\LineButton.jpg", Vector2i(0, 0), maxTextureResolution,
	MODE_LINE, [&]() {
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
		Drawer::allVisibleObjects.push_back(new Line(points.first, points.second));
		Creation::Create();
		return;
	});
Button perpendicularButton = Button(Vector2f(340, 10), Vector2f(100, 100), &window,
	"Textures\\SFML_project\\PerpendicularButton.jpg", Vector2i(0, 0), maxTextureResolution,
	MODE_PERPENDICULAR, [&]() {
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
		Drawer::allVisibleObjects.push_back(new Line(point, line));
		return;

	});
Button midPointButton = Button(Vector2f(670, 10), Vector2f(100, 100), &window,
	"Textures\\SFML_project\\MidpointButton.jpg", Vector2i(0, 0), maxTextureResolution,
	MODE_MIDPOINT, [&]() {
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
		new Point(points.first, points.second, new Scalar(1));
		Creation::Create();
		return;
	});

Button scalarButton = Button(Vector2f(670, 10), Vector2f(100, 100), &window,
	"Textures\\SFML_project\\MidpointButton.jpg", Vector2i(0, 0), maxTextureResolution,
	MODE_MIDPOINT, [&]() {
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
		DialogBox dialogBox(&window);
		while (!dialogBox.isFinished())
		{
			wait.sleep();
		}
		new Point(points.first, points.second, new Scalar(dialogBox.getDouble()));
		Creation::Create();
		return;
	});

Button tangentButton = Button(Vector2f(450, 10), Vector2f(100, 100), &window,
	"Textures\\SFML_project\\TangentButton.jpg", Vector2i(0, 0), maxTextureResolution,
	MODE_TANGENT, [&]() {
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
			auto unitCircle = UnitCircle::getInstance();
			Drawer::allVisibleObjects.push_back(new Line(unitCircle, point));
		}
		Creation::Create();
		return;
	});

Button deleteButton = Button(Vector2f(560, 10), Vector2f(100, 100), &window,
	"Textures\\SFML_project\\DeleteButton.jpg", Vector2i(0, 0), maxTextureResolution,
	MODE_DELETE, [&]() {
		Waiter wait;
		Finder find;
		if (wait.untilClick())
		{
			return;
		}
		Vector2f mousePosition = (window).mapPixelToCoords(Mouse::getPosition(window), view);
		Object* object = find.nearbyVisibleObject(mousePosition);
		if (object)
		{
			delete object;
			object = nullptr;
		}
		Creation::Create();
		return;
	});

Button hideButton = Button(Vector2f(780, 10), Vector2f(100, 100), &window,
	"Textures\\SFML_project\\HideButton.jpg", Vector2i(0, 0), maxTextureResolution,
	MODE_HIDE, [&]() {
		Waiter wait;
		Finder find;
		if (wait.untilClick())
		{
			return;
		}
		Vector2f mousePosition = (window).mapPixelToCoords(Mouse::getPosition(window), view);
		VisibleObject* object = find.nearbyObject(mousePosition);
		if (object)
		{
			object->changeVisibility();
		}
		Creation::Create();
		return;
	});

Button clearButton = Button(Vector2f(890, 10), Vector2f(100, 100), &window,
	"Textures\\SFML_project\\ClearButton.jpg", Vector2i(0, 0), maxTextureResolution,
	MODE_CLEAR, [&]() {
		Waiter wait;
		while (Drawer::allVisibleObjects.size() > 1)
		{
			auto object = *prev(Drawer::allVisibleObjects.end());
			delete object;
			object = nullptr;
		}
		wait.sleep();
		Creation::Create();
		return;
	});

VisibleObject* Finder::nearbyVisibleObject(Vector2f mousePosition)
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
	for (VisibleObject* object : Drawer::allVisibleObjects)
	{
		if (object && object->isNearby(mousePosition))
		{
			return object;
		}
	}
	return nullptr;
}

VisibleObject* Finder::nearbyObject(Vector2f mousePosition)
{
	for (VisibleObject* object : Drawer::allVisibleObjects)
	{
		if (object && object->isNearby(mousePosition))
		{
			return object;
		}
	}
	if (UnitCircle::getInstance()->isNearby(mousePosition))
	{
		return UnitCircle::getInstance();
	}
	return nullptr;
}

Point* Finder::nearbyConstructedPoint(Vector2f mousePosition)
{
	for (VisibleObject* object : Drawer::allVisibleObjects)
	{
		Point* point = dynamic_cast<Point*>(object);
		if (point && point->isNearby(mousePosition) && point->getVisibility())
		{
			return point;
		}
	}
	return nullptr;
}

Point* Finder::nearbyConstructedPointOnCircle(Vector2f mousePosition)
{
	for (VisibleObject* object : Drawer::allVisibleObjects)
	{
		Point* point = dynamic_cast<Point*>(object);
		if (point)
		{
			if (point->isNearby(mousePosition) && point->isOnCircle() && point->getVisibility())
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
	auto unitCircle = UnitCircle::getInstance();
	if (unitCircle->isNearby(mousePosition))
	{
		return new Point(unitCircle, mousePosition);
	}
}

Line* Finder::nearbyLine(Vector2f mousePosition)
{
	for (VisibleObject* object : Drawer::allVisibleObjects)
	{
		Line* line = dynamic_cast<Line*>(object);
		if (line && line->isNearby(mousePosition) && line->getVisibility())
		{
			return line;
		}
	}
	return nullptr;
}

pair<Line*, Line*> Finder::nearbyLines(Vector2f mousePosition)
{
	pair<Line*, Line*> lines = { nullptr,nullptr };
	for (VisibleObject* object : Drawer::allVisibleObjects)
	{
		Line* line = dynamic_cast<Line*>(object);
		if (line)
		{
			if (line->isNearby(mousePosition) && line->getVisibility())
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
		if (point->isNearby(mousePosition) && point->getVisibility())
		{
			return point;
		}
		else
		{
			delete point;
			point = nullptr;
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
