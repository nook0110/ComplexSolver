#include "PresetButtons.h"
using namespace sf;

extern RenderWindow mainWindow;//
extern View view;//
extern Vector2i maxTextureResolution;//
extern MODES Mousemode;//
extern Menu mainMenu;

std::list<VisibleObject*> Drawer::allVisibleObjects;

Button moveButton = Button(Vector2f(10, 10), Vector2f(100, 100), &mainWindow,
	"Textures\\SFML_project\\MoveButton.jpg", Vector2i(0, 0), maxTextureResolution,
	MODE_MOVE, []()->VisibleObject* {
		Waiter wait;
		InterruptionChecker interruptionChecker;
		Finder find;
		if (wait.untilClick())
		{
			return nullptr;
		}
		Vector2f startPosition = view.getCenter();
		Vector2f startMousePosition(Vector2f(Mouse::getPosition(mainWindow)));
		Point* point = find.nearbyConstructedPoint(mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view));
		while (Mouse::isButtonPressed(Mouse::Button::Left))
		{
			if (!interruptionChecker.checkInterruption())
				return nullptr;
			Vector2f delta = Vector2f(Mouse::getPosition(mainWindow)) - startMousePosition;
			Vector2f Scale;
			Scale.x = view.getSize().x / mainWindow.getSize().x;
			Scale.y = view.getSize().y / mainWindow.getSize().y;
			delta.x *= Scale.x;
			delta.y *= Scale.y;
			if (point)
			{
				point->moveTo(mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view));
			}
			else
			{
				view.setCenter(startPosition - delta);
			}
			wait.sleep();
		}
		return nullptr;
	});

Button pointButton = Button(Vector2f(120, 10), Vector2f(100, 100), &mainWindow,
	"Textures\\SFML_project\\Point.png", Vector2i(0, 0), maxTextureResolution,
	MODE_POINT, []()->VisibleObject* {
		Waiter wait;
		Finder find;
		if (wait.untilClick())
		{
			return nullptr;
		}
		Vector2f mousePosition = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
		Point* point = find.nearbyConstructedPoint(mousePosition);
		if (point)
		{
			return nullptr;
		}
		point = find.nearbyNewPoint(mousePosition);
		return point;
	});

Button lineButton = Button(Vector2f(230, 10), Vector2f(100, 100), &mainWindow,
	"Textures\\SFML_project\\Line.png", Vector2i(0, 0), maxTextureResolution,
	MODE_LINE, []()->VisibleObject* {
		Waiter wait;
		Finder find;
		const int twotimes = 2;
		std::pair<Point*, Point*> points(nullptr, nullptr);
		for (int i = 0; i < twotimes; ++i)
		{
			if (wait.untilClick())
			{
				return nullptr;
			}
			Vector2f mousePosition = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
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
		return line;
	});

Button pointBetweenPoints = Button(Vector2f(230, 10), Vector2f(100, 100), &mainWindow,
	"Textures\\SFML_project\\PointBetweenPoints.png", Vector2i(0, 0), maxTextureResolution,
	MODE_POINT_BETWEEN_POINTS, []()->VisibleObject* {
		Waiter wait;
		Finder find;
		const int twotimes = 2;
		std::pair<Point*, Point*> points(nullptr, nullptr);
		for (int i = 0; i < twotimes; ++i)
		{
			if (wait.untilClick())
			{
				return nullptr;
			}
			Vector2f mousePosition = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
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
			return nullptr;
		}
		Vector2f mousePosition = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);

		Point* point = new Point(line, points.first, points.second, mousePosition);
		return point;
	});

Button centralProjectionButton = Button(Vector2f(340, 10), Vector2f(100, 100), &mainWindow,
	"Textures\\SFML_project\\CentralProjection.png", Vector2i(0, 0), maxTextureResolution,
	MODE_CENTRAL_PROJECTION, []()->VisibleObject* {
		Waiter wait;
		Finder find;
		if (wait.untilClick())
		{
			return nullptr;
		}
		Vector2f mousePosition = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
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
				return nullptr;
			}
			mousePosition = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
			secondPoint = find.nearbyConstructedPointOnCircle(mousePosition);
			if (!secondPoint)
			{
				secondPoint = find.nearbyNewPointOnCircle(mousePosition);
			}
		}
		Point* point = new UnitPoint(UnitCircle::getInstance(), firstPoint, secondPoint);
		return point;
	});

Button perpendicularButton = Button(Vector2f(340, 10), Vector2f(100, 100), &mainWindow,
	"Textures\\SFML_project\\Perpendicular.png", Vector2i(0, 0), maxTextureResolution,
	MODE_PERPENDICULAR, []()->VisibleObject* {
		Waiter wait;
		InterruptionChecker interruptionChecker;
		Finder find;
		if (wait.untilClick())
		{
			return nullptr;
		}
		Vector2f mousePosition = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
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
				return nullptr;
			}
			mousePosition = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
			line = find.nearbyLine(mousePosition);
			wait.sleep();
		}
		if (dynamic_cast<UnitPoint*>(point) && dynamic_cast<Chord*>(line))
		{
			UnitPoint* unitPoint = new UnitPoint(UnitCircle::getInstance(), dynamic_cast<UnitPoint*>(point), dynamic_cast<Chord*>(line));
			return new Chord(dynamic_cast<UnitPoint*>(point), unitPoint);
		}
		return new Line(point, line);
	});
Button midPointButton = Button(Vector2f(670, 10), Vector2f(100, 100), &mainWindow,
	"Textures\\SFML_project\\Midpoint.png", Vector2i(0, 0), maxTextureResolution,
	MODE_MIDPOINT, []()->VisibleObject* {
		Waiter wait;
		Finder find;
		const int twotimes = 2;
		std::pair<Point*, Point*> points(nullptr, nullptr);
		for (int i = 0; i < twotimes; ++i)
		{
			if (wait.untilClick())
			{
				return nullptr;
			}
			Vector2f mousePosition = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
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
		Point* point = new Point(points.first, points.second, 1);
		return point;
	});

Button symmetryButton = Button(Vector2f(670, 10), Vector2f(100, 100), &mainWindow,
	"Textures\\SFML_project\\Symmetry.png", Vector2i(0, 0), maxTextureResolution,
	MODE_SYMMETRY, []()->VisibleObject* {
		Waiter wait;
		Finder find;
		std::pair<Point*, Point*> points(nullptr, nullptr);
		const int twotimes = 2;
		for (int i = 0; i < twotimes; ++i)
		{
			if (wait.untilClick())
			{
				return nullptr;
			}
			Vector2f mousePosition = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
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
		Point* point = new Point(points.first, points.second, (float)-2);
		return point;
	});

Button rotateLeftButton = Button(Vector2f(450, 10), Vector2f(100, 100), &mainWindow,
	"Textures\\SFML_project\\Test.jpg", Vector2i(0, 0), maxTextureResolution,
	MODE_ROTATION_LEFT, []()->VisibleObject* {
		Waiter wait;
		Finder find;
		std::pair<Point*, Point*> points(nullptr, nullptr);
		const int twotimes = 2;
		for (int i = 0; i < twotimes; ++i)
		{
			if (wait.untilClick())
			{
				return nullptr;
			}
			Vector2f mousePosition = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
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
		Point* point = new Point(points.first, points.second, -1);
		return point;
	});

Button rotateRightButton = Button(Vector2f(450, 10), Vector2f(100, 100), &mainWindow,
	"Textures\\SFML_project\\Test.jpg", Vector2i(0, 0), maxTextureResolution,
	MODE_ROTATION_RIGHT, []()->VisibleObject* {
		Waiter wait;
		Finder find;
		std::pair<Point*, Point*> points(nullptr, nullptr);
		const int twotimes = 2;
		for (int i = 0; i < twotimes; ++i)
		{
			if (wait.untilClick())
			{
				return nullptr;
			}
			Vector2f mousePosition = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
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
		Point* point = new Point(points.first, points.second, 1);
		return point;
	});

Button projectionButton = Button(Vector2f(450, 10), Vector2f(100, 100), &mainWindow,
	"Textures\\SFML_project\\Projection.png", Vector2i(0, 0), maxTextureResolution,
	MODE_PROJECTION, []()->VisibleObject* {
		Waiter wait;
		InterruptionChecker interruptionChecker;
		Finder find;
		if (wait.untilClick())
		{
			return nullptr;
		}
		Vector2f mousePosition = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
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
				return nullptr;
			}
			mousePosition = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
			line = find.nearbyLine(mousePosition);
			wait.sleep();
		}
		return new Point(point, line);
	});

Button parallelButton = Button(Vector2f(560, 10), Vector2f(120, 120), &mainWindow,
	"Textures\\SFML_project\\Parallel.png", Vector2i(0, 0), maxTextureResolution,
	MODE_PARALLEL, []()->VisibleObject* {
		Waiter wait;
		InterruptionChecker interruptionChecker;
		Finder find;
		Line* line = nullptr;
		while (!line)
		{
			if (wait.untilClick())
			{
				return nullptr;
			}
			Vector2f mousePosition = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
			line = find.nearbyLine(mousePosition);
		}
		if (wait.untilClick())
		{
			return nullptr;
		}
		Vector2f mousePosition = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
		Point* point = find.nearbyConstructedPoint(mousePosition);
		if (!point)
		{
			point = find.nearbyNewPoint(mousePosition);
		}
		if (dynamic_cast<Chord*>(line) && dynamic_cast<UnitPoint*>(point))
		{
			UnitPoint* unitPoint = new UnitPoint(UnitCircle::getInstance(), dynamic_cast<Chord*>(line), dynamic_cast<UnitPoint*>(point));
			return new Chord(dynamic_cast<UnitPoint*>(point), unitPoint);
		}
		return new Line(line, point);
	});

Button scalarButton = Button(Vector2f(670, 10), Vector2f(100, 100), &mainWindow,
	"Textures\\SFML_project\\Scalar.png", Vector2i(0, 0), maxTextureResolution,
	MODE_MIDPOINT, []()->VisibleObject* {
		InterruptionChecker checker;
		Waiter wait;
		Finder find;
		const int twotimes = 2;
		std::pair<Point*, Point*> points(nullptr, nullptr);
		for (int i = 0; i < twotimes; ++i)
		{
			if (wait.untilClick())
			{
				return nullptr;
			}
			Vector2f mousePosition = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
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
		ScalarBox* dialogBox = new ScalarBox(&mainWindow);
		while (!dialogBox->isFinished())
		{
			if (!checker.checkInterruption())
			{
				return nullptr;
			}
			wait.sleep();
		}
		float ratio = dialogBox->getDouble();
		delete dialogBox;
		new Point(points.first, points.second, ratio);
		return nullptr;
	});

Button tangentButton = Button(Vector2f(450, 10), Vector2f(100, 100), &mainWindow,
	"Textures\\SFML_project\\TangentButton.jpg", Vector2i(0, 0), maxTextureResolution,
	MODE_TANGENT, []()->VisibleObject* {
		Waiter wait;
		Finder find;
		if (wait.untilClick())
		{
			return nullptr;
		}
		Vector2f mousePosition = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
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
		return nullptr;
	});

Button deleteButton = Button(Vector2f(560, 10), Vector2f(100, 100), &mainWindow,
	"Textures\\SFML_project\\DeleteButton.jpg", Vector2i(0, 0), maxTextureResolution,
	MODE_DELETE, []()->VisibleObject* {
		Waiter wait;
		Finder find;
		if (wait.untilClick())
		{
			return nullptr;
		}
		Vector2f mousePosition = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
		Object* object = find.nearbyVisibleObject(mousePosition);
		if (object)
		{
			delete object;
			object = nullptr;
		}
		return nullptr;
	});

Button hideButton = Button(Vector2f(780, 10), Vector2f(100, 100), &mainWindow,
	"Textures\\SFML_project\\HideButton.jpg", Vector2i(0, 0), maxTextureResolution,
	MODE_HIDE, []()->VisibleObject* {
		Waiter wait;
		Finder find;
		if (wait.untilClick())
		{
			return nullptr;
		}
		Vector2f mousePosition = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
		VisibleObject* object = find.nearbyObject(mousePosition);
		if (object)
		{
			object->changeVisibility();
		}
		return nullptr;
	});

Button clearButton = Button(Vector2f(890, 10), Vector2f(100, 100), &mainWindow,
	"Textures\\SFML_project\\ClearButton.jpg", Vector2i(0, 0), maxTextureResolution,
	MODE_CLEAR, []()->VisibleObject* {
		Waiter wait;
		while (Drawer::allVisibleObjects.size() > 1)
		{
			auto object = *prev(Drawer::allVisibleObjects.end());
			delete object;
			object = nullptr;
		}
		wait.sleep();
		return nullptr;
	});

Button switchButton = Button(Vector2f(890, 10), Vector2f(100, 100), &mainWindow,
	"Textures\\SFML_project\\Test.jpg", Vector2i(0, 0), maxTextureResolution,
	MODE_SWITCH, []()->VisibleObject* {
		mainMenu.switchLayer();
		return nullptr;
	});

Button fourPointsOnACircle = Button(Vector2f(890, 10), Vector2f(100, 100), &mainWindow,
	"Textures\\SFML_project\\Test.jpg", Vector2i(0, 0), maxTextureResolution,
	MODE_FOUR_POINTS, []()->VisibleObject* {
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
					return nullptr;
				}
				Vector2f mousePosition = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
				point = find.nearbyConstructedPoint(mousePosition);
			}
			points.push_back(point);
		}
		new Circle(points[0], points[1], points[2], points[3]);
		return nullptr;
	});

Button ñollinearityOfLines = Button(Vector2f(890, 10), Vector2f(100, 100), &mainWindow,
	"Textures\\SFML_project\\Test.jpg", Vector2i(0, 0), maxTextureResolution,
	MODE_HIDE, []()->VisibleObject* {
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
					return nullptr;
				}
				Vector2f mousePosition = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
				line = find.nearbyLine(mousePosition);
			}
			lines.push_back(line);
		}
		return nullptr;
	});

Button ñollinearityOfPoints = Button(Vector2f(890, 10), Vector2f(100, 100), &mainWindow,
	"Textures\\SFML_project\\Test.jpg", Vector2i(0, 0), maxTextureResolution,
	MODE_HIDE, []()->VisibleObject* {
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
					return nullptr;
				}
				Vector2f mousePosition = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
				point = find.nearbyConstructedPoint(mousePosition);
			}
			points.push_back(point);
		}
		return nullptr;
	});

Button ProveConstructionButton = Button(Vector2f(890, 10), Vector2f(100, 100), &mainWindow,
	"Textures\\SFML_project\\Test.jpg", Vector2i(0, 0), maxTextureResolution,
	MODE_PROVE_CONSTRUCTION, []()->VisibleObject* {
		Waiter wait;
		Finder find;
		InterruptionChecker checker;
		VisibleObject* firstObject = nullptr;
		while (!firstObject)
		{
			if (wait.untilClick())
			{
				return nullptr;
			}
			Vector2f mousePosition = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
			firstObject = find.nearbyVisibleObject(mousePosition);
		}
		mainMenu.switchLayer();
		VisibleObject* secondObject = nullptr;
		while (checker.checkInterruption())
		{
			wait.sleep();
		}
		while (!secondObject)
		{
			if (Mousemode == MODE_NOTHING || Mousemode == MODE_PROVE_CONSTRUCTION)
			{
				return nullptr;
			}
			secondObject = Creation::getInstance()->CurrentMethod();
		}
		wait.sleep();
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
		if (object && object->isNearby(mousePosition) && object->getVisibility())
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
	return checker.checkMode();
}

bool Waiter::mouseOnTheScreen()
{
	Vector2f mousePosition = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
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
