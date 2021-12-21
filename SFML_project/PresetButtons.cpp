
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


//



Button moveButton = Button(Vector2f(10, 10), Vector2f(100, 100), &window,
	"H:\\Textures\\Test.jpg", Vector2i(0, 0), maxTextureResolution,
	MODE_NOTHING, []() {
		Waiter wait;
		InterruptionChecker interruptionChecker;
		wait.untilClick(interruptionChecker);
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
		if (!interruptionChecker.checkInterruption())
			return;
		Creation::Create();
		return ;
	});

Button pointButton = Button(Vector2f(120, 10), Vector2f(100, 100), &window,
	"C:\\Textures\\SFML_project\\Test.jpg", Vector2i(0, 0), maxTextureResolution,
	MODE_POINT, []() {
		Waiter wait;
		InterruptionChecker interruptionChecker;
		Finder find;
		wait.untilClick(interruptionChecker);
		Vector2f mousePosition = (window).mapPixelToCoords(Mouse::getPosition(window), view);
		Point* point = find.nearbyPoint(mousePosition);
		if (!interruptionChecker.checkInterruption())
			return;
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
		InterruptionChecker interruptionChecker;
		Finder find;
		const int twotimes = 2;
		pair<Point*, Point*> points;
		for (int i = 0; i < twotimes; ++i)
		{
			wait.untilClick(interruptionChecker);
			Vector2f mousePosition = (window).mapPixelToCoords(Mouse::getPosition(window), view);
			Point* point = find.nearbyPoint(mousePosition);
			if (!interruptionChecker.checkInterruption())
				return;
			if (point)
			{
				if (get<0>(points) && point != get<0>(points))
				{
					points.second = point;
					Creation::Create();
					ConstructionData::allVisibleObjects.push_back(point);
				}
				else
				{
					points.first = point;
					ConstructionData::allVisibleObjects.push_back(point);
				}
			}
			point = find.nearbyIntersection(mousePosition);
			if (point)
			{
				if (get<0>(points) && point != get<0>(points))
				{
					points.second = point;
					Creation::Create();
					ConstructionData::allVisibleObjects.push_back(point);
				}
				else
				{
					points.first = point;
					ConstructionData::allVisibleObjects.push_back(point);
				}
			}

			point = new Point(mousePosition);
			if (get<0>(points) && point != get<0>(points))
			{
				points.second = point;
				Creation::Create();
				ConstructionData::allVisibleObjects.push_back(point);
			}
			else
			{
				points.first = point;
				ConstructionData::allVisibleObjects.push_back(point);
			}

		}

		Creation::Create();
		//ConstructionData::allVisibleObjects.push_back(new Line(points[0], points[1]));
		//return new Line(points[0], points[1]);

	});
Button perpendicularButton = Button(Vector2f(340, 10), Vector2f(239, 100), &window,
	"C:\\Textures\\SFML_project\\Test.jpg", Vector2i(0, 0), maxTextureResolution,
	MODE_NOTHING, []() {
		return nullptr;
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
