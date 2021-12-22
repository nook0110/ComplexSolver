
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
extern UnitCircle* unitCircle;
list<VisibleObject*> ConstructionData::allVisibleObjects;

Button moveButton = Button(Vector2f(10, 10), Vector2f(100, 100), &window,
	"H:\\Textures\\Test.jpg", Vector2i(0, 0), maxTextureResolution,
	MODE_NOTHING, []() {
		Waiter wait;
		InterruptionChecker interruptionChecker;
		wait.untilClick();
		Vector2f MousePosition(Vector2f(Mouse::getPosition(window)));
		while (Mouse::isButtonPressed(Mouse::Button::Left))
		{
			if (!interruptionChecker.checkInterruption())
				return;
			Vector2f delta = MousePosition - Vector2f(Mouse::getPosition(window));
			Vector2f Scale;
			Scale.x = view.getSize().x / window.getSize().x;
			Scale.y = view.getSize().y / window.getSize().y;
			delta.x *= Scale.x;
			delta.y *= Scale.y;
			view.move(delta);
			MousePosition = Vector2f(Mouse::getPosition(window));
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
		Creation::Create();
		return;
	});

Button pointButton = Button(Vector2f(120, 10), Vector2f(100, 100), &window,
	"C:\\Textures\\SFML_project\\Test.jpg", Vector2i(0, 0), maxTextureResolution,
	MODE_POINT, []() {
		Waiter wait;
		Finder find;
		if (wait.untilClick())
		{
			return;
		}
		Vector2f mousePosition = (window).mapPixelToCoords(Mouse::getPosition(window), view);
		Point* point = find.nearbyPoint(mousePosition);
		if (point)
		{
			Creation::Create();
			ConstructionData::allVisibleObjects.push_back(point);
			return;
		}
		point = find.nearbyIntersection(mousePosition);
		if (point)
		{
			Creation::Create();
			ConstructionData::allVisibleObjects.push_back(point);
			return;
		}
		point = new Point(mousePosition);
		Creation::Create();
		ConstructionData::allVisibleObjects.push_back(point);
		return;
	});


Button lineButton = Button(Vector2f(230, 10), Vector2f(100, 100), &window,
	"C:\\Textures\\SFML_project\\Test.jpg", Vector2i(0, 0), maxTextureResolution,
	MODE_NOTHING, []() {
		Waiter wait;
		Finder find;
		const int twotimes = 2;
		pair<Point*, Point*> points;
		for (int i = 0; i < twotimes; ++i)
		{
			//std::this_thread::sleep_for(std::chrono::seconds(1));
			if (wait.untilClick())
			{
				return;
			}
			Vector2f mousePosition = (window).mapPixelToCoords(Mouse::getPosition(window), view);
			Point* point = find.nearbyPoint(mousePosition);
			if (point)
			{
				if (get<0>(points))
				{
					if (point != get<0>(points))
					{
						points.second = point;
						break;
					}
				}
				else
				{
					points.first = point;
					break;
				}

			}
			point = find.nearbyIntersection(mousePosition);
			if (point)
			{
				if (get<0>(points))
				{
					if (point != get<0>(points))
					{
						points.second = point;
						ConstructionData::allVisibleObjects.push_back(point);
						break;
					}
				}
				else
				{
					points.first = point;
					ConstructionData::allVisibleObjects.push_back(point);
					break;
				}
			}
			point = new Point(mousePosition);
			if (get<0>(points) && point != get<0>(points))
			{
				points.second = point;
				ConstructionData::allVisibleObjects.push_back(point);
			}
			else
			{
				points.first = point;
				ConstructionData::allVisibleObjects.push_back(point);
			}
		}
		Creation::Create();
		ConstructionData::allVisibleObjects.push_back(new Line(points.first, points.second));
		return;
	});
Button perpendicularButton = Button(Vector2f(340, 10), Vector2f(239, 100), &window,
	"C:\\Textures\\SFML_project\\Test.jpg", Vector2i(0, 0), maxTextureResolution,
	MODE_NOTHING, []() {
		Waiter wait;
		InterruptionChecker interruptionChecker;
		Finder find;
		//wait.untilClick(interruptionChecker);
		Vector2f mousePosition = (window).mapPixelToCoords(Mouse::getPosition(window), view);
		Line* line = find.nearbyLine(mousePosition);
		if (line)
		{
			//wait.untilClick(interruptionChecker);
			Vector2f mousePosition = (window).mapPixelToCoords(Mouse::getPosition(window), view);
			Point* point = find.nearbyPoint(mousePosition);
			if (!interruptionChecker.checkInterruption())
				return;
			if (point)
			{
				Creation::Create();
				ConstructionData::allVisibleObjects.push_back(new Line(line, point));
				return;
			}
			point = find.nearbyIntersection(mousePosition);
			if (point)
			{
				Creation::Create();
				ConstructionData::allVisibleObjects.push_back(new Line(line, point));
				ConstructionData::allVisibleObjects.push_back(point);
				return;
			}
			point = new Point(mousePosition);
			Creation::Create();
			ConstructionData::allVisibleObjects.push_back(point);
			return;
		}
	});
Button midPointButton = Button(Vector2f(450, 10), Vector2f(100, 239), &window,
	"C:\\Textures\\SFML_project\\Test.jpg", Vector2i(0, 0), maxTextureResolution,
	MODE_NOTHING, []() {
		return nullptr;
	});

Point* Finder::nearbyPoint(Vector2f mousePosition)
{
	for (VisibleObject* object : ConstructionData::allVisibleObjects)
	{
		Point* point = dynamic_cast<Point*>(object);
		if (point)
		{
			if (point->isNearby(mousePosition))
			{
				return point;
			}
		}

	}
	return nullptr;
}

Line* Finder::nearbyLine(Vector2f mousePosition)
{
	for (VisibleObject* object : ConstructionData::allVisibleObjects)
	{
		Line* line = dynamic_cast<Line*>(object);
		if (line)
		{
			if (line->isNearby(mousePosition))
			{
				return line;
			}
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
		//return new Point(lines.first, lines.second);
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
	Vector2f rightDownCorner = (center + size/2.f);
	rightDownCorner.y += size.y * (mainWindowRect.height - mainWindowRect.top) - size.y;
	return mousePosition.x > leftUpCorner.x && mousePosition.y > leftUpCorner.y &&
		mousePosition.x < rightDownCorner.x&& mousePosition.y < rightDownCorner.y;
}

bool Waiter::untilClick()
{
	while (Mouse::isButtonPressed(Mouse::Button::Left))
	{
		if (!interruptionChecker.checkInterruption())
			return true;
		std::this_thread::sleep_for(std::chrono::nanoseconds(1000));
	}
	while (!Mouse::isButtonPressed(Mouse::Button::Left) || mainMenu.mouseOnMenu() || !mouseOnTheScreen())
	{
		if (!interruptionChecker.checkInterruption())
			return true;
		std::this_thread::sleep_for(std::chrono::nanoseconds(1000));
	}
	return false;
}
