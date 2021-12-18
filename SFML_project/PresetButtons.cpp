
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
	MODE_NOTHING, []()->VisibleObject* {
		Waiter wait;
		InterruptionChecker interruptionChecker;
		wait.untilClick(interruptionChecker);
		Vector2f MousePosition(Vector2f(Mouse::getPosition(window)));
		while (Mouse::isButtonPressed(Mouse::Button::Left))
		{
			if (!interruptionChecker.checkInterruption())
				return nullptr;
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
			return nullptr;
		Creation::Create();
		return nullptr;
	});

Button pointButton = Button(Vector2f(120, 10), Vector2f(100, 100), &window,
	"H:\\Textures\\Test.jpg", Vector2i(0, 0), maxTextureResolution,
	MODE_POINT, []()->VisibleObject* {
		Waiter wait;
		InterruptionChecker interruptionChecker;
		wait.untilClick(interruptionChecker);
		Vector2f MousePosition = (window).mapPixelToCoords(Mouse::getPosition(window), view);
		vector<Line*> lines;
		for (VisibleObject* object : ConstructionData::allVisibleObjects)
		{
			Point* point = dynamic_cast<Point*>(object);
			if (point)
			{
				if (point->isNearby(MousePosition))
				{
					Creation::Create();
					return nullptr;
				}
			}
			Line* line = dynamic_cast<Line*>(object);
			if (line)
			{
				if (line->isNearby(MousePosition))
				{
					lines.push_back(line);
				}
			}
			UnitCircle* unitCircle = dynamic_cast<UnitCircle*>(object);

		}



		if (lines.size() == 2)
		{
			if (!interruptionChecker.checkInterruption())
				return nullptr;

			Creation::Create();
			Point* point = new Point(lines[0], lines[1]);
			ConstructionData::allVisibleObjects.push_back(point);
			return nullptr;
		}
		//Unit circle check
		if (unitCircle->isNearby(MousePosition))
		{
			if (!interruptionChecker.checkInterruption())
				return nullptr;
			Creation::Create();
			Point* point = new Point(unitCircle, MousePosition);
			ConstructionData::allVisibleObjects.push_back(point);
			return nullptr;
		}
		if (lines.size() == 1)
		{
			if (!interruptionChecker.checkInterruption())
			{
				return nullptr;
			}
			Creation::Create();
			Point* point = new Point(lines[0], MousePosition);
			ConstructionData::allVisibleObjects.push_back(point);
			return nullptr;
		}
		if (!interruptionChecker.checkInterruption())
			return nullptr;
		Creation::Create();
		Point* point = new Point(MousePosition);
		ConstructionData::allVisibleObjects.push_back(point);
		return nullptr;
	});


Button lineButton = Button(Vector2f(230, 10), Vector2f(100, 100), &window,
	"H:\\Textures\\Test.jpg", Vector2i(0, 0), maxTextureResolution,
	MODE_NOTHING, []()->VisibleObject* {
		InterruptionChecker interruptionChecker;
		const int twotimes = 2;
		vector<Point*> points;
		for (int i = 0; i < twotimes; ++i)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
			while (!Mouse::isButtonPressed(Mouse::Button::Left))
			{
				if (!interruptionChecker.checkInterruption(points))
					return nullptr;
				std::this_thread::sleep_for(std::chrono::nanoseconds(1000));
			}

			Vector2f MousePosition = (window).mapPixelToCoords(Mouse::getPosition(window), view);

			for (VisibleObject* object : ConstructionData::allVisibleObjects)
			{
				Point* point = dynamic_cast<Point*>(object);
				if (point)
				{
					if (point->isNearby(MousePosition))
					{
						points.push_back(point);
						break;
					}
				}
			}
			if (points.size() > i)
			{
				continue;
			}
			vector<Line*> lines;
			for (VisibleObject* object : ConstructionData::allVisibleObjects)
			{
				Line* line = dynamic_cast<Line*>(object);
				if (line)
				{
					if (line->isNearby(MousePosition))
					{
						lines.push_back(line);
					}
					if (lines.size() == 2)
					{
						break;
					}
				}
			}

			if (lines.size() == 2)
			{
				if (!interruptionChecker.checkInterruption())
					return nullptr;

				Point* point = new Point(lines[0], lines[1]);
				ConstructionData::allVisibleObjects.push_back(point);
				points.push_back(point);
				continue;
			}
			//Unit circle check
			if (unitCircle->isNearby(MousePosition))
			{
				if (!interruptionChecker.checkInterruption())
					return nullptr;

				Point* point = new Point(unitCircle, MousePosition);
				ConstructionData::allVisibleObjects.push_back(point);
				points.push_back(point);
				continue;
			}
			if (lines.size() == 1)
			{
				if (!interruptionChecker.checkInterruption())
				{
					return nullptr;
				}

				Point* point = new Point(lines[0], MousePosition);
				ConstructionData::allVisibleObjects.push_back(point);
				points.push_back(point);
				continue;
			}
			if (!interruptionChecker.checkInterruption())
				return nullptr;

			Point* point = new Point(MousePosition);
			points.push_back(point);
			ConstructionData::allVisibleObjects.push_back(point);

		}

		if (!interruptionChecker.checkInterruption(points))
			return nullptr;
		Creation::Create();
		ConstructionData::allVisibleObjects.push_back(new Line(points[0], points[1]));
		return new Line(points[0], points[1]);

	});
Button perpendicularButton = Button(Vector2f(340, 10), Vector2f(239, 100), &window,
	"C:\\Textures\\SFML_project\\Test.jpg", Vector2i(0, 0), maxTextureResolution,
	MODE_NOTHING, []()->VisibleObject* {
		return nullptr;
	});
Button midPointButton = Button(Vector2f(450, 10), Vector2f(100, 239), &window,
	"C:\\Textures\\SFML_project\\Test.jpg", Vector2i(0, 0), maxTextureResolution,
	MODE_NOTHING, []()->VisibleObject* {
		return nullptr;
	});
	