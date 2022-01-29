
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
using namespace sf;

extern RenderWindow mainWindow;//
extern View view;//
extern Vector2i maxTextureResolution;//
extern MODES Mousemode;//
extern Plane* plane;//
std::list<VisibleObject*> Drawer::allVisibleObjects;

Button moveButton = Button(Vector2f(10, 10), Vector2f(100, 100), &mainWindow,
	"Textures\\SFML_project\\MoveButton.jpg", Vector2i(0, 0), maxTextureResolution,
	MODE_MOVE, []() {
		Waiter wait;
		InterruptionChecker interruptionChecker;
		Finder find;
		if (wait.untilClick())
		{
			return;
		}
		Vector2f startPosition = view.getCenter();
		Vector2f startMousePosition(Vector2f(Mouse::getPosition(mainWindow)));
		Point* point = find.nearbyConstructedPoint((mainWindow).mapPixelToCoords(Mouse::getPosition(mainWindow), view));
		while (Mouse::isButtonPressed(Mouse::Button::Left))
		{
			if (!interruptionChecker.checkInterruption())
				return;
			Vector2f delta = Vector2f(Mouse::getPosition(mainWindow)) - startMousePosition;
			Vector2f Scale;
			Scale.x = view.getSize().x / mainWindow.getSize().x;
			Scale.y = view.getSize().y / mainWindow.getSize().y;
			delta.x *= Scale.x;
			delta.y *= Scale.y;
			if (point)
			{
				point->moveTo((mainWindow).mapPixelToCoords(Mouse::getPosition(mainWindow), view));
			}
			else
			{
				view.setCenter(startPosition - delta);
			}
			wait.sleep();
		}
		Creation::Create();
		return;
	});

Button pointButton = Button(Vector2f(120, 10), Vector2f(100, 100), &mainWindow,
	"Textures\\SFML_project\\PointButton.jpg", Vector2i(0, 0), maxTextureResolution,
	MODE_POINT, []() {
		Waiter wait;
		Finder find;
		if (wait.untilClick())
		{
			return;
		}
		Vector2f mousePosition = (mainWindow).mapPixelToCoords(Mouse::getPosition(mainWindow), view);


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


Button lineButton = Button(Vector2f(230, 10), Vector2f(100, 100), &mainWindow,
	"Textures\\SFML_project\\LineButton.jpg", Vector2i(0, 0), maxTextureResolution,
	MODE_LINE, []() {
		Waiter wait;
		Finder find;
		const int twotimes = 2;
		std::pair<Point*, Point*> points(nullptr, nullptr);
		for (int i = 0; i < twotimes; ++i)
		{
			if (wait.untilClick())
			{
				return;
			}
			Vector2f mousePosition = (mainWindow).mapPixelToCoords(Mouse::getPosition(mainWindow), view);
			Point* point = find.nearbyConstructedPoint(mousePosition);
			if (!point)
			{
				point = find.nearbyNewPoint(mousePosition);
			}
			if (std::get<0>(points))
			{
				if (point != std::get<0>(points))
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
		if (points.first->isOnCircle() && points.second->isOnCircle())
		{
			new Chord(dynamic_cast<UnitPoint*>(points.first), dynamic_cast<UnitPoint*>(points.second));
		}
		else
		{
			new Line(points.first, points.second);
		}

		Creation::Create();
		return;
	});

Button pointBetweenPoints = Button(Vector2f(230, 10), Vector2f(100, 100), &mainWindow,
	"Textures\\SFML_project\\LineButton.jpg", Vector2i(0, 0), maxTextureResolution,
	MODE_POINT_BETWEEN_POINTS, []() {
		Waiter wait;
		Finder find;
		const int twotimes = 2;
		std::pair<Point*, Point*> points(nullptr, nullptr);
		for (int i = 0; i < twotimes; ++i)
		{
			if (wait.untilClick())
			{
				return;
			}
			Vector2f mousePosition = (mainWindow).mapPixelToCoords(Mouse::getPosition(mainWindow), view);
			Point* point = find.nearbyConstructedPoint(mousePosition);
			if (!point)
			{
				point = find.nearbyNewPoint(mousePosition);
			}
			if (std::get<0>(points))
			{
				if (point != std::get<0>(points))
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
		Line* line;
		if (points.first->isOnCircle() && points.second->isOnCircle())
		{
			line = new Chord(dynamic_cast<UnitPoint*>(points.first), dynamic_cast<UnitPoint*>(points.second));
		}
		else
		{
			line = new Line(points.first, points.second);
		}
		if (wait.untilClick())
		{
			return;
		}
		Vector2f mousePosition = (mainWindow).mapPixelToCoords(Mouse::getPosition(mainWindow), view);

		new Point(line, points.first, points.second, mousePosition);
		Creation::Create();
		return;
	});

Button centralProjectionButton = Button(Vector2f(340, 10), Vector2f(100, 100), &mainWindow,
	"Textures\\SFML_project\\Test.jpg", Vector2i(0, 0), maxTextureResolution,
	MODE_CENTRAL_PROJECTION, []() {
		Waiter wait;
		Finder find;
		if (wait.untilClick())
		{
			return;
		}
		Vector2f mousePosition = (mainWindow).mapPixelToCoords(Mouse::getPosition(mainWindow), view);
		Point* firstPoint = find.nearbyConstructedPoint(mousePosition);
		if (!firstPoint)
		{
			firstPoint = find.nearbyNewPoint(mousePosition);
		}
		UnitPoint* secondPoint = nullptr;
		while (!secondPoint)
		{
			if (wait.untilClick())
			{
				return;
			}
			mousePosition = (mainWindow).mapPixelToCoords(Mouse::getPosition(mainWindow), view);
			secondPoint = find.nearbyConstructedPointOnCircle(mousePosition);
			if (!secondPoint)
			{
				secondPoint = find.nearbyNewPointOnCircle(mousePosition);
			}
		}
		new UnitPoint(UnitCircle::getInstance(), firstPoint, secondPoint);
		Creation::Create();
		return;
	});

Button perpendicularButton = Button(Vector2f(340, 10), Vector2f(100, 100), &mainWindow,
	"Textures\\SFML_project\\PerpendicularButton.jpg", Vector2i(0, 0), maxTextureResolution,
	MODE_PERPENDICULAR, []() {
		Waiter wait;
		InterruptionChecker interruptionChecker;
		Finder find;
		if (wait.untilClick())
		{
			return;
		}
		Vector2f mousePosition = (mainWindow).mapPixelToCoords(Mouse::getPosition(mainWindow), view);
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
			mousePosition = (mainWindow).mapPixelToCoords(Mouse::getPosition(mainWindow), view);
			line = find.nearbyLine(mousePosition);
			wait.sleep();
		}
		Creation::Create();
		new Line(point, line);
		return;

	});
Button midPointButton = Button(Vector2f(670, 10), Vector2f(100, 100), &mainWindow,
	"Textures\\SFML_project\\MidpointButton.jpg", Vector2i(0, 0), maxTextureResolution,
	MODE_MIDPOINT, []() {
		Waiter wait;
		Finder find;
		const int twotimes = 2;
		std::pair<Point*, Point*> points(nullptr, nullptr);
		for (int i = 0; i < twotimes; ++i)
		{
			if (wait.untilClick())
			{
				return;
			}
			Vector2f mousePosition = (mainWindow).mapPixelToCoords(Mouse::getPosition(mainWindow), view);
			Point* point = find.nearbyConstructedPoint(mousePosition);
			if (!point)
			{
				point = find.nearbyNewPoint(mousePosition);
			}
			if (std::get<0>(points))
			{
				if (point != std::get<0>(points))
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

Button symmetryButton = Button(Vector2f(670, 10), Vector2f(100, 100), &mainWindow,
	"Textures\\SFML_project\\Test.jpg", Vector2i(0, 0), maxTextureResolution,
	MODE_SYMMETRY, []() {
		Waiter wait;
		Finder find;
		const int twotimes = 2;
		std::pair<Point*, Point*> points(nullptr, nullptr);
		for (int i = 0; i < twotimes; ++i)
		{
			if (wait.untilClick())
			{
				return;
			}
			Vector2f mousePosition = (mainWindow).mapPixelToCoords(Mouse::getPosition(mainWindow), view);
			Point* point = find.nearbyConstructedPoint(mousePosition);
			if (!point)
			{
				point = find.nearbyNewPoint(mousePosition);
			}
			if (std::get<0>(points))
			{
				if (point != std::get<0>(points))
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
		new Point(points.first, points.second, new Scalar(-2));
		Creation::Create();
		return;
	});

Button projectionButton = Button(Vector2f(450, 10), Vector2f(100, 100), &mainWindow,
	"Textures\\SFML_project\\Test.jpg", Vector2i(0, 0), maxTextureResolution,
	MODE_PROJECTION, []() {
		Waiter wait;
		InterruptionChecker interruptionChecker;
		Finder find;
		if (wait.untilClick())
		{
			return;
		}
		Vector2f mousePosition = (mainWindow).mapPixelToCoords(Mouse::getPosition(mainWindow), view);
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
			mousePosition = (mainWindow).mapPixelToCoords(Mouse::getPosition(mainWindow), view);
			line = find.nearbyLine(mousePosition);
			wait.sleep();
		}
		Creation::Create();
		new Point(point, line);
		return;
	});

Button parallelButton = Button(Vector2f(560, 10), Vector2f(120, 120), &mainWindow,
	"Textures\\SFML_project\\Test.jpg", Vector2i(0, 0), maxTextureResolution,
	MODE_PARALLEL, []() {
		Waiter wait;
		InterruptionChecker interruptionChecker;
		Finder find;
		Line* line = nullptr;
		while (!line)
		{
			if (wait.untilClick())
			{
				return;
			}
			Vector2f mousePosition = (mainWindow).mapPixelToCoords(Mouse::getPosition(mainWindow), view);
			line = find.nearbyLine(mousePosition);
		}
		if (wait.untilClick())
		{
			return;
		}
		Vector2f mousePosition = (mainWindow).mapPixelToCoords(Mouse::getPosition(mainWindow), view);
		Point* point = find.nearbyConstructedPoint(mousePosition);
		if (!point)
		{
			point = find.nearbyNewPoint(mousePosition);
		}
		new Line(line, point);
		Creation::Create();
		return;
	});

Button scalarButton = Button(Vector2f(670, 10), Vector2f(100, 100), &mainWindow,
	"Textures\\SFML_project\\MidpointButton.jpg", Vector2i(0, 0), maxTextureResolution,
	MODE_MIDPOINT, []() {
		InterruptionChecker checker;
		Waiter wait;
		Finder find;
		const int twotimes = 2;
		std::pair<Point*, Point*> points(nullptr, nullptr);
		for (int i = 0; i < twotimes; ++i)
		{
			if (wait.untilClick())
			{
				return;
			}
			Vector2f mousePosition = (mainWindow).mapPixelToCoords(Mouse::getPosition(mainWindow), view);
			Point* point = find.nearbyConstructedPoint(mousePosition);
			if (!point)
			{
				point = find.nearbyNewPoint(mousePosition);
			}
			if (std::get<0>(points))
			{
				if (point != std::get<0>(points))
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
		ScalarBox dialogBox(&mainWindow);
		while (!dialogBox.isFinished())
		{
			if (!checker.checkInterruption())
			{
				return;
			}
			wait.sleep();
		}
		new Point(points.first, points.second, new Scalar(dialogBox.getDouble()));
		Creation::Create();
		return;
	});

Button tangentButton = Button(Vector2f(450, 10), Vector2f(100, 100), &mainWindow,
	"Textures\\SFML_project\\TangentButton.jpg", Vector2i(0, 0), maxTextureResolution,
	MODE_TANGENT, []() {
		Waiter wait;
		Finder find;
		if (wait.untilClick())
		{
			return;
		}
		Vector2f mousePosition = (mainWindow).mapPixelToCoords(Mouse::getPosition(mainWindow), view);
		Point* point = find.nearbyConstructedPointOnCircle(mousePosition);
		if (!point)
		{
			point = find.nearbyNewPointOnCircle(mousePosition);
		}
		if (point)
		{
			auto unitCircle = UnitCircle::getInstance();
			new Line(unitCircle, point);
		}
		Creation::Create();
		return;
	});

Button deleteButton = Button(Vector2f(560, 10), Vector2f(100, 100), &mainWindow,
	"Textures\\SFML_project\\DeleteButton.jpg", Vector2i(0, 0), maxTextureResolution,
	MODE_DELETE, []() {
		Waiter wait;
		Finder find;
		if (wait.untilClick())
		{
			return;
		}
		Vector2f mousePosition = (mainWindow).mapPixelToCoords(Mouse::getPosition(mainWindow), view);
		Object* object = find.nearbyVisibleObject(mousePosition);
		if (object)
		{
			delete object;
			object = nullptr;
		}
		Creation::Create();
		return;
	});

Button hideButton = Button(Vector2f(780, 10), Vector2f(100, 100), &mainWindow,
	"Textures\\SFML_project\\HideButton.jpg", Vector2i(0, 0), maxTextureResolution,
	MODE_HIDE, []() {
		Waiter wait;
		Finder find;
		if (wait.untilClick())
		{
			return;
		}
		Vector2f mousePosition = (mainWindow).mapPixelToCoords(Mouse::getPosition(mainWindow), view);
		VisibleObject* object = find.nearbyObject(mousePosition);
		if (object)
		{
			object->changeVisibility();
		}
		Creation::Create();
		return;
	});

Button clearButton = Button(Vector2f(890, 10), Vector2f(100, 100), &mainWindow,
	"Textures\\SFML_project\\ClearButton.jpg", Vector2i(0, 0), maxTextureResolution,
	MODE_CLEAR, []() {
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


Button fourPointsOnACircle = Button(Vector2f(890, 10), Vector2f(100, 100), &mainWindow,
	"Textures\\SFML_project\\ClearButton.jpg", Vector2i(0, 0), maxTextureResolution,
	MODE_HIDE, []() {
		Waiter wait;
		Finder find;
		const int fourTimes = 4;
		std::vector<Point*> points;
		for (int i = 0; i < fourTimes; ++i)
		{
			Point* point = nullptr;
			while (!point)
			{
				if (wait.untilClick())
				{
					return;
				}
				Vector2f mousePosition = (mainWindow).mapPixelToCoords(Mouse::getPosition(mainWindow), view);
				point = find.nearbyConstructedPoint(mousePosition);
			}
			points.push_back(point);
		}
		/*
		PointEquation* first = dynamic_cast<PointEquation*>(points[0]->getEquation());
		PointEquation* second = dynamic_cast<PointEquation*>(points[1]->getEquation());
		PointEquation* third = dynamic_cast<PointEquation*>(points[2]->getEquation());
		Vector2f deltaOneTwo = first->point - second->point;
		Vector2f deltaTwoThree = second->point - third->point;
		Vector2f deltaThreeOne = third->point - first->point;
		double zFirst = (first->point.x) * (first->point.x) + (first->point.y) * (first->point.y);
		double zSecond = (second->point.x) * (second->point.x) + (second->point.y) * (second->point.y);
		double zThird = (third->point.x) * (third->point.x) + (third->point.y) * (third->point.y);
		double zX = deltaOneTwo.y * zThird + deltaTwoThree.y * zFirst + deltaThreeOne.y * zSecond;
		double zY = deltaOneTwo.x * zThird + deltaTwoThree.x * zFirst + deltaThreeOne.x * zSecond;
		double z = deltaOneTwo.x * deltaThreeOne.y - deltaOneTwo.y * deltaThreeOne.x;
		Vector2f center(-zX / (2 * z), zY / (2 * z));
		double r = sqrt((first->point - center).x * (first->point - center).x + (first->point - center).y * (first->point - center).y);
		Drawer::proveShape = new CircleShape(r, 100);
		Drawer::proveShape->setOrigin(Vector2f(r, r));
		Drawer::proveShape->setPosition(center);
		Drawer::proveShape->setFillColor(Color(0, 0, 0, 0));
		Drawer::proveShape->setOutlineColor(Color::Red);
		Drawer::proveShape->setOutlineThickness(5);
		*/
		Creation::Create();
		return;
	});

Button ñollinearityOfLines = Button(Vector2f(890, 10), Vector2f(100, 100), &mainWindow,
	"Textures\\SFML_project\\ClearButton.jpg", Vector2i(0, 0), maxTextureResolution,
	MODE_HIDE, []() {
		Waiter wait;
		Finder find;
		const int threeTimes = 3;
		std::vector<Line*> lines;
		for (int i = 0; i < threeTimes; ++i)
		{
			Line* line = nullptr;
			while (!line)
			{
				if (wait.untilClick())
				{
					return;
				}
				Vector2f mousePosition = (mainWindow).mapPixelToCoords(Mouse::getPosition(mainWindow), view);
				line = find.nearbyLine(mousePosition);
			}
			lines.push_back(line);
		}
		Creation::Create();
		return;
	});

Button ñollinearityOfPoints = Button(Vector2f(890, 10), Vector2f(100, 100), &mainWindow,
	"Textures\\SFML_project\\ClearButton.jpg", Vector2i(0, 0), maxTextureResolution,
	MODE_HIDE, []() {
		Waiter wait;
		Finder find;
		const int threeTimes = 4;
		std::vector<Point*> points;
		for (int i = 0; i < threeTimes; ++i)
		{
			Point* point = nullptr;
			while (!point)
			{
				if (wait.untilClick())
				{
					return;
				}
				Vector2f mousePosition = (mainWindow).mapPixelToCoords(Mouse::getPosition(mainWindow), view);
				point = find.nearbyConstructedPoint(mousePosition);
			}
			points.push_back(point);
		}

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

UnitPoint* Finder::nearbyConstructedPointOnCircle(Vector2f mousePosition)
{
	for (VisibleObject* object : Drawer::allVisibleObjects)
	{
		UnitPoint* point = dynamic_cast<UnitPoint*>(object);
		if (point && point->isNearby(mousePosition) && point->getVisibility())
		{
			return point;
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
		//return new Point(line, mousePosition);
	}
	UnitCircle* unitCircle = UnitCircle::getInstance();
	if (unitCircle->isNearby(mousePosition))
	{
		return new UnitPoint(unitCircle, mousePosition);
	}
	return new Point(mousePosition);
}

UnitPoint* Finder::nearbyNewPointOnCircle(Vector2f mousePosition)
{
	auto unitCircle = UnitCircle::getInstance();
	if (unitCircle->isNearby(mousePosition))
	{
		return new UnitPoint(unitCircle, mousePosition);
	}
	return nullptr;
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

std::pair<Line*, Line*> Finder::nearbyLines(Vector2f mousePosition)
{
	std::pair<Line*, Line*> lines = { nullptr,nullptr };
	for (VisibleObject* object : Drawer::allVisibleObjects)
	{
		Line* line = dynamic_cast<Line*>(object);
		if (line)
		{
			if (line->isNearby(mousePosition) && line->getVisibility())
			{
				(std::get<0>(lines) ? lines.second : lines.first) = line;
				if (std::get<1>(lines)) return lines;
			}
		}
	}
	return { nullptr,nullptr };
}



Point* Finder::nearbyIntersection(Vector2f mousePosition)
{
	std::pair<Line*, Line*> lines = nearbyLines(mousePosition);
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
	Vector2f mousePosition = (mainWindow).mapPixelToCoords(Mouse::getPosition(mainWindow),view);
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
