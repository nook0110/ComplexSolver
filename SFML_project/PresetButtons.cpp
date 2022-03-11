#include "PresetButtons.h"
using namespace sf;


std::list<Object*> Drawer::allVisibleObjects;

Button moveButton = Button(&mainWindow,
	"Textures\\Button_textures\\Move.png", MODE_MOVE, []()->Object* {
		Waiter wait;
		InterruptionChecker interruptionChecker;
		Finder find;
		if (wait.untilClick())
		{
			return nullptr;
		}
		Clock clock;
		Vector2f startPosition = view.getCenter();
		Vector2i startMousePosition(Mouse::getPosition(mainWindow));
		Description* descr = find.nearbyDescription(mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view));
		if (descr)
		{
			Vector2f deltaDescr = descr->getDelta(mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view));
			while (Mouse::isButtonPressed(Mouse::Button::Left))
			{
				if (!interruptionChecker.checkInterruption())
					return nullptr;
				descr->moveTo(mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view) - deltaDescr);
				wait.sleep();
			}
			return nullptr;
		}
		Point* point = find.nearbyConstructedPoint(mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view));
		Object* object = find.nearbyNotUnitCircleObject(mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view));
		while (Mouse::isButtonPressed(Mouse::Button::Left))
		{
			if (!interruptionChecker.checkInterruption())
				return nullptr;
			Vector2f delta = Vector2f(Mouse::getPosition(mainWindow) - startMousePosition);
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
		if (object)
		{
			const int clickTime = 200;
			Vector2i delta = Mouse::getPosition(mainWindow) - startMousePosition;
			if (sqrt(delta.x * delta.x + delta.y * delta.y) < epsilon && clock.getElapsedTime().asMilliseconds() < clickTime)
			{
				object->switchDescription(mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view));
			}
		}
		return nullptr;
	});

Button pointButton = Button(&mainWindow,
	"Textures\\Button_textures\\Point.png",
	MODE_POINT, []()->Object* {
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

Button lineButton = Button(&mainWindow,
	"Textures\\Button_textures\\Line.png",
	MODE_LINE, []()->Object* {
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

Button pointBetweenPoints = Button(&mainWindow,
	"Textures\\Button_textures\\PointBetweenPoints.png",
	MODE_POINT_BETWEEN_POINTS, []()->Object* {
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

Button centralProjectionButton = Button(&mainWindow,
	"Textures\\Button_textures\\CentralProjection.png",
	MODE_CENTRAL_PROJECTION, []()->Object* {
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
		UnitPoint* point = new UnitPoint(UnitCircle::getInstance(), firstPoint, secondPoint);
		return point;
	});

Button perpendicularButton = Button(&mainWindow,
	"Textures\\Button_textures\\Perpendicular.png",
	MODE_PERPENDICULAR, []()->Object* {
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
		Point* projPoint = new Point(point, line);
		return new Line(point, projPoint);
	});

Button midPointButton = Button(&mainWindow,
	"Textures\\Button_textures\\Midpoint.png",
	MODE_MIDPOINT, []()->Object* {
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
		Point* point = new Point(points.first, points.second, { 1,1 });
		return point;
	});

Button symmetryButton = Button(&mainWindow,
	"Textures\\Button_textures\\Symmetry.png",
	MODE_SYMMETRY, []()->Object* {
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
		Point* point = new Point(points.first, points.second, { -2,1 });
		return point;
	});

Button rotateLeftButton = Button(&mainWindow,
	"Textures\\Button_textures\\RotateLeft.png",
	MODE_ROTATION_LEFT, []()->Object* {
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

Button rotateRightButton = Button(&mainWindow,
	"Textures\\Button_textures\\RotateRight.png",
	MODE_ROTATION_RIGHT, []()->Object* {
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

Button projectionButton = Button(&mainWindow,
	"Textures\\Button_textures\\Projection.png",
	MODE_PROJECTION, []()->Object* {
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

Button parallelButton = Button(&mainWindow,
	"Textures\\Button_textures\\Parallel.png",
	MODE_PARALLEL, []()->Object* {
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

Button scalarButton = Button(&mainWindow,
	"Textures\\Button_textures\\Scalar.png",
	MODE_MIDPOINT, []()->Object* {
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
		ScalarBox* dialogBox = new ScalarBox();
		while (!dialogBox->isFinished())
		{
			if (!checker.checkInterruption())
			{
				return nullptr;
			}
			wait.sleep();
		}
		std::pair<int, int> masses = dialogBox->getDouble();
		delete dialogBox;
		new Point(points.first, points.second, masses);
		return nullptr;
	});

Button tangentButton = Button(&mainWindow,
	"Textures\\Button_textures\\Tangent.png",
	MODE_TANGENT, []()->Object* {
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

Button deleteButton = Button(&mainWindow,
	"Textures\\Button_textures\\Delete.png",
	MODE_DELETE, []()->Object* {
		Waiter wait;
		Finder find;
		if (wait.untilClick())
		{
			return nullptr;
		}
		Vector2f mousePosition = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
		Object* object = find.nearbyNotUnitCircleObject(mousePosition);
		if (object)
		{
			Drawer::delObject(object);
		}
		return nullptr;
	});

Button hideButton = Button(&mainWindow,
	"Textures\\Button_textures\\Hide.png",
	MODE_HIDE, []()->Object* {
		Waiter wait;
		Finder find;
		if (wait.untilClick())
		{
			return nullptr;
		}
		Vector2f mousePosition = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
		Object* object = find.nearbyObject(mousePosition);
		if (object)
		{
			object->setVisibility();
		}
		return nullptr;
	});

Button clearButton = Button(&mainWindow,
	"Textures\\Button_textures\\Clear.png",
	MODE_CLEAR, []()->Object* {
		Waiter wait;
		Memento::clear();
		wait.sleep();
		return nullptr;
	});
Button switchButton = Button(&mainWindow,
	"Textures\\Button_textures\\Switch.png",
	MODE_SWITCH, []()->Object* {
		mainMenu.switchLayer(0);
		return nullptr;
	});


Button switchTriangleButton = Button(&mainWindow,
	"Textures\\Button_textures\\SwitchTriangle.png",
	MODE_SWITCH, []()->Object* {
		mainMenu.switchLayer(2);
		return nullptr;
	});

Button orthocenterButton = Button(&mainWindow,
	"Textures\\Button_textures\\Orthocenter.png",
	MODE_ORTHOCENTER, []()->Object* {
		Waiter wait;
		Finder find;
		const int size = 3;
		std::vector<UnitPoint*> points;
		while (points.size() < size)
		{
			if (wait.untilClick())
			{
				return nullptr;
			}
			Vector2f mousePosition = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
			UnitPoint* point = find.nearbyConstructedPointOnCircle(mousePosition);
			if (!point)
			{
				continue;
			}
			points.push_back(point);
		}
		return new Point(points[0], points[1], points[2]);
	});

Button barycenterButton = Button(&mainWindow,
	"Textures\\Button_textures\\Barycenter.png",
	MODE_BARYCENTER, []()->Object* {
		Waiter wait;
		Finder find;
		const int size = 3;
		std::vector<Point*> points;
		while (points.size() < size)
		{
			if (wait.untilClick())
			{
				return nullptr;
			}
			Vector2f mousePosition = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
			Point* point = find.nearbyConstructedPoint(mousePosition);
			if (!point)
			{
				continue;
			}
			points.push_back(point);
		}
		return new Point(points[0], points[1], points[2]);
	});

Button switchConstructionButton = Button(&mainWindow,
	"Textures\\Button_textures\\SwitchConstruction.png",
	MODE_SWITCH, []()->Object* {
		mainMenu.switchLayer(1);
		return nullptr;
	});

Button inscription = Button(&mainWindow,
	"Textures\\Button_textures\\Inscription.png",
	MODE_FOUR_POINTS, []()->Object* {
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
		Prover::proveInscription(points[0], points[1], points[2], points[3]);
		return nullptr;
	});

Button twoLineSegments = Button(&mainWindow,
	"Textures\\Button_textures\\Test.png",
	MODE_TWO_LINE_SEGMENTS, []()->Object* {
		Waiter wait;
		Finder find;
		const int twoTimes = 2;
		std::vector<Point*> points;
		for (int i = 0; i < twoTimes; ++i)
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
		new LineSegment(points[0], points[1]);
		for (int i = 0; i < twoTimes; ++i)
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
		new LineSegment(points[2], points[3]);
		return nullptr;
	});

Button concurrencyOfLines = Button(&mainWindow,
	"Textures\\Button_textures\\Concurrency.png",
	MODE_THREE_LINES, []()->Object* {
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
		Prover::proveConcurrency(lines[0], lines[1], lines[2]);
		return nullptr;
	});

Button ñollinearityOfPoints = Button(&mainWindow,
	"Textures\\Button_textures\\Collinearity.png",
	MODE_THREE_POINTS, []()->Object* {
		Waiter wait;
		Finder find;
		const int threeTimes = 3;
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
		auto line = new Line(points[0], points[1]);
		line->setDotted(true);
		Prover::proveCollinearity(points[0], points[1], points[2]);
		return nullptr;
	});

Button proveConstructionButton = Button(&mainWindow,
	"Textures\\Button_textures\\Test.png",
	MODE_PROVE_CONSTRUCTION, []()->Object* {
		Waiter wait;
		Finder find;
		InterruptionChecker checker;
		Object* firstObject = nullptr;
		while (!firstObject)
		{
			if (wait.untilClick())
			{
				return nullptr;
			}
			Vector2f mousePosition = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
			firstObject = find.nearbyNotUnitCircleObject(mousePosition);
		}
		mainMenu.switchLayer(0);
		Object* secondObject = nullptr;
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

Button debugButton = Button(&mainWindow,
	"Textures\\Button_textures\\Test.png",
	MODE_PROVE_CONSTRUCTION, []()->Object* {
		Waiter wait;
		Finder find;
		InterruptionChecker checker;
		Object* object = nullptr;
		while (!object)
		{
			if (wait.untilClick())
			{
				return nullptr;
			}
			Vector2f mousePosition = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
			object = find.nearbyNotUnitCircleObject(mousePosition);
		}
		object->printExpr();
	});

Description* Finder::nearbyDescription(Vector2f mousePosition)
{
	for (Description* descr : Drawer::allDescriptions)
	{
		if (descr->contains(mousePosition))
		{
			return descr;
		}
	}
	return nullptr;
}

Object* Finder::nearbyNotUnitCircleObject(Vector2f mousePosition)
{
	float lastDistancePoint = INFINITY;
	float lastDistanceLine = INFINITY;
	Object* nearestObjectPoint = nullptr;
	Object* nearestObjectLine = nullptr;
	for (Object* object : Drawer::allVisibleObjects)
	{
		if (dynamic_cast<Point*>(object) && object->isNearby(mousePosition) && object->getVisibility())
		{
			if (object->distance(mousePosition) < lastDistancePoint)
			{
				lastDistancePoint = object->distance(mousePosition);
				nearestObjectPoint = object;
			}
		}
	}
	for (Object* object : Drawer::allVisibleObjects)
	{
		if (dynamic_cast<Line*>(object) && object->isNearby(mousePosition) && object->getVisibility())
		{
			if (object->distance(mousePosition) < lastDistanceLine)
			{
				lastDistanceLine = object->distance(mousePosition);
				nearestObjectLine = object;
			}
		}
	}
	if (nearestObjectPoint && dynamic_cast<Point*>(nearestObjectPoint)->contains(mousePosition))
	{
		return nearestObjectPoint;
	}
	if (lastDistanceLine < lastDistancePoint)
	{
		return nearestObjectLine;
	}
	else
	{
		return nearestObjectPoint;
	}
}

Object* Finder::nearbyObject(Vector2f mousePosition)
{
	float lastDistancePoint = INFINITY;
	float lastDistanceLine = INFINITY;
	Object* nearestObjectPoint = nullptr;
	Object* nearestObjectLine = nullptr;
	for (Object* object : Drawer::allVisibleObjects)
	{
		if (dynamic_cast<Point*>(object) && object->isNearby(mousePosition))
		{
			if (object->distance(mousePosition) < lastDistancePoint)
			{
				lastDistancePoint = object->distance(mousePosition);
				nearestObjectPoint = object;
			}
		}
	}
	for (Object* object : Drawer::allVisibleObjects)
	{
		if (dynamic_cast<Line*>(object) && object->isNearby(mousePosition))
		{
			if (object->distance(mousePosition) < lastDistanceLine)
			{
				lastDistanceLine = object->distance(mousePosition);
				nearestObjectLine = object;
			}
		}
	}
	if (nearestObjectPoint && dynamic_cast<Point*>(nearestObjectPoint)->contains(mousePosition))
	{
		return nearestObjectPoint;
	}
	UnitCircle* unitCircle = UnitCircle::getInstance();
	if (unitCircle->isNearby(mousePosition))
	{
		if (unitCircle->distance(mousePosition) < lastDistancePoint)
		{
			return unitCircle;
		}
	}
	if (lastDistanceLine < lastDistancePoint)
	{
		return nearestObjectLine;
	}
	else
	{
		return nearestObjectPoint;
	}
}

Point* Finder::nearbyConstructedPoint(Vector2f mousePosition)
{
	float lastDistance = INFINITY;
	Point* point = nullptr;
	for (Object* object : Drawer::allVisibleObjects)
	{
		Point* anotherPoint = dynamic_cast<Point*>(object);
		if (anotherPoint && anotherPoint->isNearby(mousePosition) && anotherPoint->getVisibility())
		{
			if (anotherPoint->distance(mousePosition) < lastDistance)
			{
				lastDistance = anotherPoint->distance(mousePosition);
				point = anotherPoint;
			}
		}
	}
	return point;
}

UnitPoint* Finder::nearbyConstructedPointOnCircle(Vector2f mousePosition)
{
	float lastDistance = INFINITY;
	UnitPoint* point = nullptr;
	for (Object* object : Drawer::allVisibleObjects)
	{
		UnitPoint* anotherPoint = dynamic_cast<UnitPoint*>(object);
		if (anotherPoint && anotherPoint->isNearby(mousePosition) && anotherPoint->getVisibility())
		{
			if (anotherPoint->distance(mousePosition) < lastDistance)
			{
				lastDistance = anotherPoint->distance(mousePosition);
				point = anotherPoint;
			}
		}
	}
	return point;
}

Point* Finder::nearbyNewPoint(Vector2f mousePosition)
{
	Point* point = nearbyIntersection(mousePosition);
	if (point)
	{
		return point;
	}
	UnitCircle* unitCircle = UnitCircle::getInstance();
	if (unitCircle->isNearby(mousePosition) && unitCircle->getVisibility())
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
	float lastDistance = INFINITY;
	Line* line = nullptr;
	for (Object* object : Drawer::allVisibleObjects)
	{
		Line* anotherLine = dynamic_cast<Line*>(object);
		if (anotherLine && anotherLine->isNearby(mousePosition) && anotherLine->getVisibility())
		{
			if (anotherLine->distance(mousePosition) < lastDistance)
			{
				lastDistance = anotherLine->distance(mousePosition);
				line = anotherLine;
			}
		}
	}
	return line;
}

std::vector<Line*> Finder::nearbyLines(Vector2f mousePosition)
{
	std::vector<Line*> lines;
	for (Object* object : Drawer::allVisibleObjects)
	{
		Line* line = dynamic_cast<Line*>(object);
		if (line)
		{
			if (line->isNearby(mousePosition) && line->getVisibility())
			{
				lines.push_back(line);
			}
		}
	}
	return lines;
}



Point* Finder::nearbyIntersection(Vector2f mousePosition)
{
	std::vector<Line*> lines = nearbyLines(mousePosition);
	std::sort(lines.begin(), lines.end(),
		[mousePosition](Line* first, Line* second)
		{
			return first->distance(mousePosition) < second->distance(mousePosition);
		});
	for (auto firstLine : lines)
	{
		for (auto secondLine : lines)
		{
			if (firstLine == secondLine)
			{
				continue;
			}
			LineEquation* firstEquation = dynamic_cast<LineEquation*>(firstLine->getEquation());
			LineEquation* secondEquation = dynamic_cast<LineEquation*>(secondLine->getEquation());
			Vector2f pointCoord = Vector2f(
				(firstEquation->B * secondEquation->C - firstEquation->C * secondEquation->B)
				/ (firstEquation->A * secondEquation->B - firstEquation->B * secondEquation->A),
				(firstEquation->C * secondEquation->A - firstEquation->A * secondEquation->C)
				/ (firstEquation->A * secondEquation->B - firstEquation->B * secondEquation->A)
			);
			Vector2f mousePixelPosition = Vector2f(mainWindow.mapCoordsToPixel(mousePosition, view));
			Vector2f coord = Vector2f(mainWindow.mapCoordsToPixel(pointCoord, view));
			double distance = sqrt(pow((coord.x - mousePixelPosition.x), 2) + pow((coord.y - mousePixelPosition.y), 2));
			if (distance < epsilon)
			{
				return new Point(firstLine, secondLine);
			}
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
