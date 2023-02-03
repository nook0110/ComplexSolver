#include "PresetButtons.h"
#include <optional>
using namespace sf;

std::list<Object*> Drawer::all_visible_objects;

template <typename T>
bool AddNewToPair(std::pair<T, T>& pair, T& object)
{
  if (!std::get<0>(pair))
  {
    pair.first = object;
    return true;
  }

  if (object != std::get<0>(pair))
  {
    pair.second = object;
    return true;
  }

  return false;
}

std::optional<std::pair<Point*, Point*>> GetTwoPoints()
{
  Waiter wait;
  Finder find;
  const int twotimes = 2;
  std::pair<Point*, Point*> points(nullptr, nullptr);
  for (int i = 0; i < twotimes; ++i)
  {
    if (wait.untilClick())
    {
      return std::nullopt;
    }
    Vector2f mouse_position = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
    Point* point = find.nearbyConstructedPoint(mouse_position);
    if (!point)
    {
      point = find.nearbyNewPoint(mouse_position);
    }

    if (!AddNewToPair(points, point))
    {
      i--;
    }

    Highlighter::highlight(point);
  }

  return points;
}

std::optional<std::vector<Point*>> GetPoints(size_t size)
{
  Waiter wait;
  Finder find;
  const int times = size;
  std::vector<Point*> points;
  for (int i = 0; i < times; ++i)
  {
    if (wait.untilClick())
    {
      return std::nullopt;
    }
    Vector2f mouse_position = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
    Point* point = find.nearbyConstructedPoint(mouse_position);
    if (!point)
    {
      point = find.nearbyNewPoint(mouse_position);
    }

    points.push_back(point);

    Highlighter::highlight(point);
  }

  return points;
}

Button moveButton = Button(mainWindow,
                           "Textures\\Button_textures\\Move.png",
                           MODE_MOVE, []()->Object*
{
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
    Highlighter::highlight(point);
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
    object->SwitchDescription(mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view));
  }
}
wait.sleep();
Highlighter::unhighlight();
return nullptr;
});

Button pointButton = Button(mainWindow,
                            "Textures\\Button_textures\\Point.png",
                            MODE_POINT, []()->Object*
{
  Waiter wait;
Finder find;
if (wait.untilClick())
{
  return nullptr;
}
Vector2f mouse_position = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
Point* point = find.nearbyConstructedPoint(mouse_position);
if (point)
{
  return nullptr;
}
point = find.nearbyNewPoint(mouse_position);
return point;
});

Button lineButton = Button(mainWindow,
                           "Textures\\Button_textures\\Line.png",
                           MODE_LINE, []()->Object*
{
  Waiter wait;
Finder find;
auto points_op = GetTwoPoints();
if (!points_op.has_value())
return nullptr;
auto& points = points_op.value();
Line* line;
if (points.first->isOnCircle() && points.second->isOnCircle())
{
  line = new Chord(dynamic_cast<UnitPoint*>(points.first), dynamic_cast<UnitPoint*>(points.second));
}
else
{
  line = new Line(points.first, points.second);
}

Highlighter::unhighlight();
return line;
});

Button pointBetweenPoints = Button(mainWindow,
                                   "Textures\\Button_textures\\PointBetweenPoints.png",
                                   MODE_POINT_BETWEEN_POINTS, []()->Object*
{
  Waiter wait;
Finder find;
auto points_op = GetTwoPoints();
if (!points_op.has_value())
return nullptr;
auto& points = points_op.value();
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
Vector2f mouse_position = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);

Point* point = new Point(line, points.first, points.second, mouse_position);
Highlighter::unhighlight();
return point;
});

Button centralProjectionButton = Button(mainWindow,
                                        "Textures\\Button_textures\\CentralProjection.png",
                                        MODE_CENTRAL_PROJECTION, []()->Object*
{
  Waiter wait;
Finder find;
if (wait.untilClick())
{
  return nullptr;
}
Vector2f mouse_position = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
Point* firstPoint = find.nearbyConstructedPoint(mouse_position);
if (!firstPoint)
{
  firstPoint = find.nearbyNewPoint(mouse_position);
}
Highlighter::highlight(firstPoint);
UnitPoint* secondPoint = nullptr;
while (!secondPoint)
{
  if (wait.untilClick())
  {
    return nullptr;
  }
  mouse_position = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
  secondPoint = find.nearbyConstructedPointOnCircle(mouse_position);
  if (!secondPoint)
  {
    secondPoint = find.nearbyNewPointOnCircle(mouse_position);
  }
}
UnitPoint* point = new UnitPoint(UnitCircle::getInstance(), firstPoint, secondPoint);
Highlighter::unhighlight();
return point;
});

Button perpendicularButton = Button(mainWindow,
                                    "Textures\\Button_textures\\Perpendicular.png",
                                    MODE_PERPENDICULAR, []()->Object*
{
  Waiter wait;
InterruptionChecker interruptionChecker;
Finder find;
if (wait.untilClick())
{
  return nullptr;
}
Vector2f mouse_position = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
Point* point = find.nearbyConstructedPoint(mouse_position);
if (!point)
{
  point = find.nearbyNewPoint(mouse_position);
}
Highlighter::highlight(point);
Line* points_ = nullptr;
while (!points_)
{
  if (wait.untilClick())
  {
    return nullptr;
  }
  mouse_position = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
  points_ = find.nearbyLine(mouse_position);
  wait.sleep();
}
Highlighter::unhighlight();
if (dynamic_cast<UnitPoint*>(point) && dynamic_cast<Chord*>(points_))
{
  UnitPoint* unitPoint = new UnitPoint(UnitCircle::getInstance(), dynamic_cast<UnitPoint*>(point), dynamic_cast<Chord*>(points_));
  return new Chord(dynamic_cast<UnitPoint*>(point), unitPoint);
}
return new Line(point, points_);
});

Button midPointButton = Button(mainWindow,
                               "Textures\\Button_textures\\Midpoint.png",
                               MODE_MIDPOINT, []()->Object*
{
  Waiter wait;
Finder find;

auto points_op = GetTwoPoints();
if (!points_op.has_value())
return nullptr;
auto& points = points_op.value();

Point* point = new Point(points.first, points.second, { 1,1 });
return point;
});

Button symmetryButton = Button(mainWindow,
                               "Textures\\Button_textures\\Symmetry.png",
                               MODE_SYMMETRY, []()->Object*
{
  Waiter wait;
Finder find;

auto points_op = GetTwoPoints();
if (!points_op.has_value())
return nullptr;
auto& points = points_op.value();

Point* point = new Point(points.first, points.second, { -2,1 });
Highlighter::unhighlight();
return point;
});

Button rotateLeftButton = Button(mainWindow,
                                 "Textures\\Button_textures\\RotateLeft.png",
                                 MODE_ROTATION_LEFT, []()->Object*
{
  Waiter wait;
Finder find;

auto points_op = GetTwoPoints();
if (!points_op.has_value())
return nullptr;
auto& points = points_op.value();

Point* point = new Point(points.first, points.second, -1);
Highlighter::unhighlight();
return point;
});

Button rotateRightButton = Button(mainWindow,
                                  "Textures\\Button_textures\\RotateRight.png",
                                  MODE_ROTATION_RIGHT, []()->Object*
{
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
  Vector2f mouse_position = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
  Point* point = find.nearbyConstructedPoint(mouse_position);
  if (!point)
  {
    point = find.nearbyNewPoint(mouse_position);
  }
  Highlighter::highlight(point);
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
Highlighter::unhighlight();
return point;
});

Button projectionButton = Button(mainWindow,
                                 "Textures\\Button_textures\\Projection.png",
                                 MODE_PROJECTION, []()->Object*
{
  Waiter wait;
InterruptionChecker interruptionChecker;
Finder find;
if (wait.untilClick())
{
  return nullptr;
}
Vector2f mouse_position = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
Point* point = find.nearbyConstructedPoint(mouse_position);
if (!point)
{
  point = find.nearbyNewPoint(mouse_position);
}
Highlighter::highlight(point);
Line* points_ = nullptr;
while (!points_)
{
  if (wait.untilClick())
  {
    return nullptr;
  }
  mouse_position = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
  points_ = find.nearbyLine(mouse_position);
  wait.sleep();
}
Highlighter::unhighlight();
return new Point(point, points_);
});

Button parallelButton = Button(mainWindow,
                               "Textures\\Button_textures\\Parallel.png",
                               MODE_PARALLEL, []()->Object*
{
  Waiter wait;
InterruptionChecker interruptionChecker;
Finder find;
Line* points_ = nullptr;
while (!points_)
{
  if (wait.untilClick())
  {
    return nullptr;
  }
  Vector2f mouse_position = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
  points_ = find.nearbyLine(mouse_position);
}
Highlighter::highlight(points_);
if (wait.untilClick())
{
  return nullptr;
}
Vector2f mouse_position = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
Point* point = find.nearbyConstructedPoint(mouse_position);
if (!point)
{
  point = find.nearbyNewPoint(mouse_position);
}
Highlighter::unhighlight();
if (dynamic_cast<Chord*>(points_) && dynamic_cast<UnitPoint*>(point))
{
  UnitPoint* unitPoint = new UnitPoint(UnitCircle::getInstance(), dynamic_cast<Chord*>(points_), dynamic_cast<UnitPoint*>(point));
  return new Chord(dynamic_cast<UnitPoint*>(point), unitPoint);
}
return new Line(points_, point);
});

Button scalarButton = Button(mainWindow,
                             "Textures\\Button_textures\\Scalar.png",
                             MODE_MIDPOINT, []()->Object*
{
  InterruptionChecker checker;
Waiter wait;
Finder find;

auto points_op = GetTwoPoints();
if (!points_op.has_value())
return nullptr;
auto& points = points_op.value();

ScalarBox* dialogBox = new ScalarBox();
while (!dialogBox->isFinished())
{
  if (!checker.checkInterruption())
  {
    return nullptr;
  }
  wait.sleep();
}
std::pair<int, int> masses = dialogBox->getRatio();
delete dialogBox;
Highlighter::unhighlight();
return new Point(points.first, points.second, masses);
});

Button tangentButton = Button(mainWindow,
                              "Textures\\Button_textures\\Tangent.png",
                              MODE_TANGENT, []()->Object*
{
  Waiter wait;
Finder find;
if (wait.untilClick())
{
  return nullptr;
}
Vector2f mouse_position = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
Point* point = find.nearbyConstructedPointOnCircle(mouse_position);
if (!point)
{
  point = find.nearbyNewPointOnCircle(mouse_position);
}
if (point)
{
  auto unit_circle = UnitCircle::getInstance();
  new Line(unit_circle, point);
}
return nullptr;
});

Button deleteButton = Button(mainWindow,
                             "Textures\\Button_textures\\Delete.png",
                             MODE_DELETE, []()->Object*
{
  Waiter wait;
Finder find;
if (wait.untilClick())
{
  return nullptr;
}
Vector2f mouse_position = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
Object* object = find.nearbyNotUnitCircleObject(mouse_position);
if (object)
{
  Drawer::delObject(object);
}
return nullptr;
});

Button hideButton = Button(mainWindow,
                           "Textures\\Button_textures\\Hide.png",
                           MODE_HIDE, []()->Object*
{
  Waiter wait;
Finder find;
if (wait.untilClick())
{
  return nullptr;
}
Vector2f mouse_position = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
Object* object = find.nearbyObject(mouse_position);
if (object)
{
  object->ChangeVisibility();
}
return nullptr;
});

Button clearButton = Button(mainWindow,
                            "Textures\\Button_textures\\Clear.png",
                            MODE_CLEAR, []()->Object*
{
  Waiter wait;
Memento::clear();
wait.sleep();
return nullptr;
});
Button switchButton = Button(mainWindow,
                             "Textures\\Button_textures\\Switch.png",
                             MODE_SWITCH, []()->Object*
{
  mainMenu.switchLayer(0);
return nullptr;
});

Button switchTriangleButton = Button(mainWindow,
                                     "Textures\\Button_textures\\SwitchTriangle.png",
                                     MODE_SWITCH, []()->Object*
{
  mainMenu.switchLayer(3);
return nullptr;
});

Button orthocenterButton = Button(mainWindow,
                                  "Textures\\Button_textures\\Orthocenter.png",
                                  MODE_ORTHOCENTER, []()->Object*
{
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
  Vector2f mouse_position = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
  UnitPoint* point = find.nearbyConstructedPointOnCircle(mouse_position);
  if (!point)
  {
    continue;
  }
  points.push_back(point);
  Highlighter::highlight(point);
}
Highlighter::unhighlight();
return new Point(points[0], points[1], points[2], Point::ORTHOCENTER);
});

Button barycenterButton = Button(mainWindow,
                                 "Textures\\Button_textures\\Barycenter.png",
                                 MODE_BARYCENTER, []()->Object*
{
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
  Vector2f mouse_position = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
  Point* point = find.nearbyConstructedPoint(mouse_position);
  if (!point)
  {
    continue;
  }
  points.push_back(point);
  Highlighter::highlight(point);
}
Highlighter::unhighlight();
return new Point(points[0], points[1], points[2]);
});

Button switchConstructionButton = Button(mainWindow,
                                         "Textures\\Button_textures\\SwitchConstruction.png",
                                         MODE_SWITCH, []()->Object*
{
  mainMenu.switchLayer(1);
return nullptr;
});

Button inscription = Button(mainWindow,
                            "Textures\\Button_textures\\Inscription.png",
                            MODE_FOUR_POINTS, []()->Object*
{
  Waiter wait;
Finder find;

constexpr size_t kFourPoints = 4;
auto points_op = GetPoints(kFourPoints);
if (!points_op.has_value())
return nullptr;
auto& points = points_op.value();

new Circle(points[0], points[1], points[2], points[3]);
Prover::proveInscription(points[0], points[1], points[2], points[3]);
return nullptr;
});

Button twoLineSegments = Button(mainWindow,
                                "Textures\\Button_textures\\TwoLineSegments.png",
                                MODE_TWO_LINE_SEGMENTS, []()->Object*
{
  Waiter wait;
Finder find;

auto first_points_op = GetTwoPoints();
if (!first_points_op.has_value())
return nullptr;
auto& first_points = first_points_op.value();

Highlighter::highlight(new LineSegment(first_points.first, first_points.second));

auto second_points_op = GetTwoPoints();
if (!second_points_op.has_value())
return nullptr;
auto& second_points = second_points_op.value();

Highlighter::highlight(new LineSegment(second_points.first, second_points.second));
Prover::proveEquivalence(first_points.first, first_points.second, second_points.first, second_points.second);
return nullptr;
});

Button concurrencyOfLines = Button(mainWindow,
                                   "Textures\\Button_textures\\Concurrency.png",
                                   MODE_THREE_LINES, []()->Object*
{
  Waiter wait;
Finder find;
const int threeTimes = 3;
std::vector<Line*> lines;
for (int i = 0; i < threeTimes; ++i)
{
  Line* points_ = nullptr;
  while (!points_)
  {
    if (wait.untilClick())
    {
      return nullptr;
    }
    Vector2f mouse_position = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
    points_ = find.nearbyLine(mouse_position);
  }
  lines.push_back(points_);
  Highlighter::highlight(points_);
}
Prover::proveConcurrency(lines[0], lines[1], lines[2]);
return nullptr;
});

Button proveParallel = Button(mainWindow,
                              "Textures\\Button_textures\\ProveParallel.png",
                              MODE_THREE_LINES, []()->Object*
{
  Waiter wait;
Finder find;
const int twoTimes = 2;
std::vector<Line*> lines;
for (int i = 0; i < twoTimes; ++i)
{
  Line* points_ = nullptr;
  while (!points_)
  {
    if (wait.untilClick())
    {
      return nullptr;
    }
    Vector2f mouse_position = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
    points_ = find.nearbyLine(mouse_position);
  }
  lines.push_back(points_);
  Highlighter::highlight(points_);
}
Prover::proveParallel(lines[0], lines[1]);
return nullptr;
});

Button proveOrthogonality = Button(mainWindow,
                                   "Textures\\Button_textures\\ProveOrthogonality.png",
                                   MODE_THREE_LINES, []()->Object*
{
  Waiter wait;
Finder find;
const int twoTimes = 2;
std::vector<Line*> lines;
for (int i = 0; i < twoTimes; ++i)
{
  Line* points_ = nullptr;
  while (!points_)
  {
    if (wait.untilClick())
    {
      return nullptr;
    }
    Vector2f mouse_position = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
    points_ = find.nearbyLine(mouse_position);
  }
  lines.push_back(points_);
  Highlighter::highlight(points_);
}
Prover::proveOrthogonality(lines[0], lines[1]);
return nullptr;
});

Button collinearityOfPoints = Button(mainWindow,
                                     "Textures\\Button_textures\\Collinearity.png",
                                     MODE_THREE_POINTS, []()->Object*
{
  Waiter wait;
Finder find;

constexpr size_t kThreePoints = 3;
auto points_op = GetPoints(kThreePoints);
if (!points_op.has_value())
return nullptr;
auto& points = points_op.value();

auto line = new Line(points[0], points[1]);
line->SetDotted(true);
Prover::proveCollinearity(points[0], points[1], points[2]);
return nullptr;
});

Button proveConstructionButton = Button(mainWindow,
                                        "Textures\\Button_textures\\ProveConstruction.png",
                                        MODE_PROVE_CONSTRUCTION, []()->Object*
{
  Waiter wait;
Finder find;
InterruptionChecker checker;
Point* firstObject = nullptr;
while (!firstObject)
{
  if (wait.untilClick())
  {
    return nullptr;
  }
  Vector2f mouse_position = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
  firstObject = find.nearbyConstructedPoint(mouse_position);
}

mainMenu.switchLayer(2);
Object* secondObject = nullptr;
while (checker.checkInterruption())
{
  wait.sleep();
}
while (!secondObject)
{
  Highlighter::highlight(firstObject);
  if (Mousemode == MODE_SWITCH)
  {
    return nullptr;
  }
  secondObject = Creation::getInstance()->CurrentMethod();
}
Prover::proveConstruction(firstObject, dynamic_cast<Point*>(secondObject));
Highlighter::unhighlight();
wait.sleep();
});

Button debugButton = Button(mainWindow,
                            "Textures\\Button_textures\\Test.png",
                            MODE_PROVE_CONSTRUCTION, []()->Object*
{
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
  Vector2f mouse_position = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
  object = find.nearbyNotUnitCircleObject(mouse_position);
}
object->printExpr();
});

Description* Finder::nearbyDescription(Vector2f mouse_position)
{
  for (Description* descr : Drawer::allDescriptions)
  {
    if (descr->contains(mouse_position))
    {
      return descr;
    }
  }
  return nullptr;
}

Object* Finder::nearbyNotUnitCircleObject(Vector2f mouse_position)
{
  float last_distance_point = INFINITY;
  float last_distance_line = INFINITY;
  Object* nearest_object_point = nullptr;
  Object* nearest_object_line = nullptr;
  for (Object* object : Drawer::all_visible_objects)
  {
    if (dynamic_cast<Point*>(object) && object->IsNearby(mouse_position) && object->GetVisibility())
    {
      if (object->DistanceTo(mouse_position) < last_distance_point)
      {
        last_distance_point = object->DistanceTo(mouse_position);
        nearest_object_point = object;
      }
    }
  }
  for (Object* object : Drawer::all_visible_objects)
  {
    if (dynamic_cast<Line*>(object) && object->IsNearby(mouse_position) && object->GetVisibility())
    {
      if (object->DistanceTo(mouse_position) < last_distance_line)
      {
        last_distance_line = object->DistanceTo(mouse_position);
        nearest_object_line = object;
      }
    }
  }
  if (nearest_object_point && dynamic_cast<Point*>(nearest_object_point)->contains(mouse_position))
  {
    return nearest_object_point;
  }
  if (last_distance_line < last_distance_point)
  {
    return nearest_object_line;
  }
  else
  {
    return nearest_object_point;
  }
}

Object* Finder::nearbyObject(Vector2f mouse_position)
{
  float last_distance_point = INFINITY;
  float last_distance_line = INFINITY;
  Object* nearest_object_point = nullptr;
  Object* nearest_object_line = nullptr;
  for (Object* object : Drawer::all_visible_objects)
  {
    if (dynamic_cast<Point*>(object) && object->IsNearby(mouse_position))
    {
      if (object->DistanceTo(mouse_position) < last_distance_point)
      {
        last_distance_point = object->DistanceTo(mouse_position);
        nearest_object_point = object;
      }
    }
  }
  for (Object* object : Drawer::all_visible_objects)
  {
    if (dynamic_cast<Line*>(object) && object->IsNearby(mouse_position))
    {
      if (object->DistanceTo(mouse_position) < last_distance_line)
      {
        last_distance_line = object->DistanceTo(mouse_position);
        nearest_object_line = object;
      }
    }
  }
  if (nearest_object_point && dynamic_cast<Point*>(nearest_object_point)->contains(mouse_position))
  {
    return nearest_object_point;
  }
  UnitCircle* unit_circle = UnitCircle::getInstance();
  if (unit_circle->IsNearby(mouse_position))
  {
    if (unit_circle->DistanceTo(mouse_position) < last_distance_point)
    {
      return unit_circle;
    }
  }
  if (last_distance_line < last_distance_point)
  {
    return nearest_object_line;
  }
  else
  {
    return nearest_object_point;
  }
}

Point* Finder::nearbyConstructedPoint(Vector2f mouse_position)
{
  float last_distance = INFINITY;
  Point* point = nullptr;
  for (Object* object : Drawer::all_visible_objects)
  {
    Point* another_point = dynamic_cast<Point*>(object);
    if (another_point && another_point->IsNearby(mouse_position) && another_point->GetVisibility())
    {
      if (another_point->DistanceTo(mouse_position) < last_distance)
      {
        last_distance = another_point->DistanceTo(mouse_position);
        point = another_point;
      }
    }
  }
  return point;
}

UnitPoint* Finder::nearbyConstructedPointOnCircle(Vector2f mouse_position)
{
  float last_distance = INFINITY;
  UnitPoint* point = nullptr;
  for (Object* object : Drawer::all_visible_objects)
  {
    UnitPoint* another_point = dynamic_cast<UnitPoint*>(object);
    if (another_point && another_point->IsNearby(mouse_position) && another_point->GetVisibility())
    {
      if (another_point->DistanceTo(mouse_position) < last_distance)
      {
        last_distance = another_point->DistanceTo(mouse_position);
        point = another_point;
      }
    }
  }
  return point;
}

Point* Finder::nearbyNewPoint(Vector2f mouse_position)
{
  Point* point = nearbyIntersection(mouse_position);
  if (point)
  {
    return point;
  }
  UnitCircle* unit_circle = UnitCircle::getInstance();
  if (unit_circle->IsNearby(mouse_position) && unit_circle->GetVisibility())
  {
    return new UnitPoint(unit_circle, mouse_position);
  }
  return new Point(mouse_position);
}

UnitPoint* Finder::nearbyNewPointOnCircle(Vector2f mouse_position)
{
  auto unit_circle = UnitCircle::getInstance();
  if (unit_circle->IsNearby(mouse_position))
  {
    return new UnitPoint(unit_circle, mouse_position);
  }
  return nullptr;
}

Line* Finder::nearbyLine(Vector2f mouse_position)
{
  float last_distance = INFINITY;
  Line* points_ = nullptr;
  for (Object* object : Drawer::all_visible_objects)
  {
    Line* anotherLine = dynamic_cast<Line*>(object);
    if (anotherLine && anotherLine->IsNearby(mouse_position) && anotherLine->GetVisibility())
    {
      if (anotherLine->DistanceTo(mouse_position) < last_distance)
      {
        last_distance = anotherLine->DistanceTo(mouse_position);
        points_ = anotherLine;
      }
    }
  }
  return points_;
}

std::vector<Line*> Finder::nearbyLines(Vector2f mouse_position)
{
  std::vector<Line*> lines;
  for (Object* object : Drawer::all_visible_objects)
  {
    Line* points_ = dynamic_cast<Line*>(object);
    if (points_)
    {
      if (points_->IsNearby(mouse_position) && points_->GetVisibility())
      {
        lines.push_back(points_);
      }
    }
  }
  return lines;
}

Point* Finder::nearbyIntersection(Vector2f mouse_position)
{
  std::vector<Line*> lines = nearbyLines(mouse_position);
  std::sort(lines.begin(), lines.end(),
            [mouse_position](Line* first, Line* second)
  {
    return first->DistanceTo(mouse_position) < second->DistanceTo(mouse_position);
  });
  for (auto first_line : lines)
  {
    for (auto second_line : lines)
    {
      if (first_line == second_line)
      {
        continue;
      }
      LineEquation* first_equation = dynamic_cast<LineEquation*>(first_line->GetEquation());
      LineEquation* second_equation = dynamic_cast<LineEquation*>(second_line->GetEquation());
      Vector2f pointCoord = Vector2f(
        (first_equation->B * second_equation->C - first_equation->C * second_equation->B)
        / (first_equation->A * second_equation->B - first_equation->B * second_equation->A),
        (first_equation->C * second_equation->A - first_equation->A * second_equation->C)
        / (first_equation->A * second_equation->B - first_equation->B * second_equation->A)
      );
      Vector2f mouse_pixel_position = Vector2f(mainWindow.mapCoordsToPixel(mouse_position, view));
      Vector2f coord = Vector2f(mainWindow.mapCoordsToPixel(pointCoord, view));
      float DistanceTo = sqrtf(pow((coord.x - mouse_pixel_position.x), 2) + pow((coord.y - mouse_pixel_position.y), 2));
      if (DistanceTo < epsilon)
      {
        return new Point(first_line, second_line);
      }
    }
  }
  return nullptr;
}

UnitCircle* Finder::nearbyUnitCircle(Vector2f mouse_position)
{
  return nullptr;
}

InterruptionChecker::InterruptionChecker()
{
  checker.checkMode();
}

bool InterruptionChecker::checkInterruption()
{
  if (checker.checkMode())
  {
    return true;
  }
  Highlighter::unhighlight();
  return false;
}

bool Waiter::mouseOnTheScreen()
{
  Vector2f mouse_position = mainWindow.mapPixelToCoords(Mouse::getPosition(mainWindow), view);
  Vector2f center = view.getCenter();
  Vector2f size = view.getSize();
  Vector2f left_up_corner = center - size / 2.f;
  Vector2f right_down_corner = (center + size / 2.f);
  right_down_corner.y += size.y * (mainWindowRect.height - mainWindowRect.top) - size.y;
  return mouse_position.x > left_up_corner.x && mouse_position.y > left_up_corner.y &&
    mouse_position.x < right_down_corner.x&& mouse_position.y < right_down_corner.y;
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

std::list<Object*> Highlighter::highlighted;

void Highlighter::highlight(Object* object)
{
  highlighted.push_back(object);
  highlighted.unique();
  object->highlight();
}

void Highlighter::unhighlight()
{
  if (highlighted.empty())
  {
    return;
  }
  for (auto obj : highlighted)
  {
    obj->unhighlight();
  }
  highlighted.clear();
}