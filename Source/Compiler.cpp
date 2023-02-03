#include "Compiler.h"

std::map<std::string, Point*, std::less<>> Compiler::points;
std::map<std::string, Line*, std::less<>> Compiler::lines;

extern void constructPreset();

void Compiler::CompileProperties(std::stringstream& sstring, Object* object)
{
  std::string visibility;
  sstring >> visibility;
  if (visibility == "")
  {
    return;
  }
  if (visibility == "invisible")
  {
    object->SetVisibility(false);
  }
  else
  {
    object->SetVisibility(true);
  }
}

void Compiler::CompileSettings(std::stringstream& sstring)
{
  std::string fpsstr;
  sstring >> fpsstr;
  if (fpsstr == "Unlimited")
  {
    fps = presets::FPS::UNLIMITED;
  }
  else if (fpsstr == "VS")
  {
    fps = presets::FPS::VS;
  }
  else
  {
    fps = presets::FPS::FRAMES_60;
  }
  std::string presetstr;
  sstring >> presetstr;
  if (presetstr == "Triangle")
  {
    preset = presets::Construction::TRIANGLE;
  }
  else if (presetstr == "Orthocenter")
  {
    preset = presets::Construction::ORTHOCENTER;
  }
  else if (presetstr == "Incenter")
  {
    preset = presets::Construction::INCENTER;
  }
  else
  {
    preset = presets::Construction::EMPTY;
  }
  constructPreset();
  return;
}

Object* Compiler::CompilePoint(std::stringstream& sstring, std::string name, std::string construction)
{
  if (points[name])
  {
    throw((std::string)"Compile error: redefinition of object name");
  }
  if (construction == "OnPlane")
  {
    std::string coordX, coordY;
    sstring >> coordX;
    sstring >> coordY;
    if (coordX.empty() || coordY.empty())
    {
      throw((std::string)"Compile error: not enough args");
    }
    Vector2f position(std::stof(coordX), std::stof(coordY));

    return points[name] = new Point(position);
  }
  else if (construction == "IntersectionOfTwoLines")
  {
    std::string first_line, second_line;
    sstring >> first_line;
    sstring >> second_line;
    if (first_line.empty() || second_line.empty())
    {
      throw((std::string)"Compile error: not enough args");
    }
    if (!lines[first_line])
    {
      throw((std::string)"Compile error: no line with name " + first_line);
    }
    if (!lines[second_line])
    {
      throw((std::string)"Compile error: no line with name " + second_line);
    }

    return points[name] = new Point(lines[first_line], lines[second_line]);
  }
  else if (construction == "OnLine")
  {
    std::string points_, firstPoint, secondPoint, coordX, coordY;
    sstring >> points_;
    sstring >> firstPoint;
    sstring >> secondPoint;
    sstring >> coordX;
    sstring >> coordY;
    if (points_.empty() || firstPoint.empty() || secondPoint.empty() || coordX.empty() || coordY.empty())
    {
      throw((std::string)"Compile error: not enough args");
    }
    Vector2f position(std::stof(coordX), std::stof(coordY));

    if (!lines[points_])
    {
      throw((std::string)"Compile error: no line with name " + points_);
    }
    if (!points[firstPoint])
    {
      throw((std::string)"Compile error: no point with name " + firstPoint);
    }
    if (!points[secondPoint])
    {
      throw((std::string)"Compile error: no point with name " + secondPoint);
    }

    return points[name] = new Point(lines[points_], points[firstPoint], points[secondPoint], position);
  }
  else if (construction == "Projection")
  {
    std::string point, points_;
    sstring >> point;
    sstring >> points_;
    if (point.empty() || points_.empty())
    {
      throw((std::string)"Compile error: not enough args");
    }
    if (!points[point])
    {
      throw((std::string)"Compile error: no point with name " + point);
    }
    if (!lines[points_])
    {
      throw((std::string)"Compile error: no line with name " + points_);
    }

    return points[name] = new Point(points[point], lines[points_]);
  }
  else if (construction == "byTwoPointsFixedRatio")
  {
    std::string firstPoint, secondPoint, firstMass, secondMass;
    sstring >> firstPoint;
    sstring >> secondPoint;
    sstring >> firstMass;
    sstring >> secondMass;
    if (firstPoint.empty() || secondPoint.empty() || firstMass.empty() || secondMass.empty())
    {
      throw((std::string)"Compile error: not enough args");
    }
    std::pair<int, int> masses(std::stoi(firstMass), std::stoi(secondMass));

    if (!points[firstPoint])
    {
      throw((std::string)"Compile error: no point with name " + firstPoint);
    }
    if (!points[secondPoint])
    {
      throw((std::string)"Compile error: no point with name " + secondPoint);
    }

    return points[name] = new Point(points[firstPoint], points[secondPoint], masses);
  }
  else if (construction == "Rotation90")
  {
    std::string center, preimage, sign;
    sstring >> center;
    sstring >> preimage;
    sstring >> sign;
    if (center.empty() || preimage.empty() || sign.empty())
    {
      throw((std::string)"Compile error: not enough args");
    }
    if (!points[center])
    {
      throw((std::string)"Compile error: no point with name " + center);
    }
    if (!points[preimage])
    {
      throw((std::string)"Compile error: no point with name " + preimage);
    }

    return points[name] = new Point(points[center], points[preimage], stoi(sign));
  }
  else if (construction == "Orthocenter")
  {
    std::string first, second, third;
    sstring >> first;
    sstring >> second;
    sstring >> third;
    if (first.empty() || second.empty() || third.empty())
    {
      throw((std::string)"Compile error: not enough args");
    }
    if (!points[first])
    {
      throw((std::string)"Compile error: no point with name " + first);
    }
    if (!points[second])
    {
      throw((std::string)"Compile error: no point with name " + second);
    }
    if (!points[third])
    {
      throw((std::string)"Compile error: no point with name " + third);
    }

    if (!dynamic_cast<UnitPoint*>(points[first]) || !dynamic_cast<UnitPoint*>(points[second]) || !dynamic_cast<UnitPoint*>(points[third]))
    {
      throw((std::string)"Compile error: invalid args");
    }

    return points[name] = new Point(dynamic_cast<UnitPoint*>(points[first]),
                                    dynamic_cast<UnitPoint*>(points[second]),
                                    dynamic_cast<UnitPoint*>(points[third]), Point::triangleCenter::ORTHOCENTER);
  }
  else if (construction == "Barycenter")
  {
    std::string first, second, third;
    sstring >> first;
    sstring >> second;
    sstring >> third;
    if (first.empty() || second.empty() || third.empty())
    {
      throw((std::string)"Compile error: not enough args");
    }
    if (!points[first])
    {
      throw((std::string)"Compile error: no point with name " + first);
    }
    if (!points[second])
    {
      throw((std::string)"Compile error: no point with name " + second);
    }
    if (!points[third])
    {
      throw((std::string)"Compile error: no point with name " + third);
    }

    return points[name] = new Point(points[first], points[second], points[third]);
  }
  else
  {
    throw((std::string)"Compile error: undefined construction");
  }
}

Object* Compiler::CompileUnitPoint(std::stringstream& sstring, std::string name, std::string construction)
{
  if (construction == "OnCircle")
  {
    std::string coordX, coordY;
    sstring >> coordX;
    sstring >> coordY;
    if (coordX.empty() || coordY.empty())
    {
      throw((std::string)"Compile error: not enough args");
    }
    Vector2f position(std::stof(coordX), std::stof(coordY));

    return points[name] = new UnitPoint(UnitCircle::getInstance(), position);
  }
  else if (construction == "CentralProjection")
  {
    std::string first, second;
    sstring >> first;
    sstring >> second;
    if (first.empty() || second.empty())
    {
      throw((std::string)"Compile error: not enough args");
    }
    if (!points[first])
    {
      throw((std::string)"Compile error: no point with name " + first);
    }
    if (!points[second])
    {
      throw((std::string)"Compile error: no point with name " + second);
    }
    if (!dynamic_cast<UnitPoint*>(points[second]))
    {
      throw((std::string)"Compile error: invalid args");
    }
    return points[name] = new UnitPoint(UnitCircle::getInstance(), points[first], dynamic_cast<UnitPoint*>(points[second]));
  }
  else if (construction == "IntersectionPerpendicularChord")
  {
    std::string first, second;
    sstring >> first;
    sstring >> second;
    if (first.empty() || second.empty())
    {
      throw((std::string)"Compile error: not enough args");
    }
    if (!lines[first])
    {
      throw((std::string)"Compile error: no line with name " + first);
    }
    if (!points[second])
    {
      throw((std::string)"Compile error: no point with name " + second);
    }
    if (!dynamic_cast<Chord*>(lines[first]) || !dynamic_cast<UnitPoint*>(points[second]))
    {
      throw((std::string)"Compile error: invalid args");
    }
    return points[name] = new UnitPoint(UnitCircle::getInstance(), dynamic_cast<Chord*>(lines[first]), dynamic_cast<UnitPoint*>(points[second]));
  }
  else if (construction == "IntersectionParallelChord")
  {
    std::string first, second;
    sstring >> first;
    sstring >> second;
    if (first.empty() || second.empty())
    {
      throw((std::string)"Compile error: not enough args");
    }
    if (!points[first])
    {
      throw((std::string)"Compile error: no line with name " + first);
    }
    if (!lines[second])
    {
      throw((std::string)"Compile error: no point with name " + second);
    }
    if (!dynamic_cast<UnitPoint*>(points[first]) || !dynamic_cast<Chord*>(lines[second]))
    {
      throw((std::string)"Compile error: invalid args");
    }
    return points[name] = new UnitPoint(UnitCircle::getInstance(), dynamic_cast<UnitPoint*>(points[first]), dynamic_cast<Chord*>(lines[second]));
  }
}

Object* Compiler::CompileLine(std::stringstream& sstring, std::string name, std::string construction)
{
  if (lines[name])
  {
    throw((std::string)"Compile error: redefinition of object name");
  }
  if (construction == "ByTwoPoints")
  {
    std::string firstPoint, secondPoint;
    sstring >> firstPoint;
    sstring >> secondPoint;
    if (firstPoint.empty() || secondPoint.empty())
    {
      throw((std::string)"Compile error: not enough args");
    }
    if (!points[firstPoint])
    {
      throw((std::string)"Compile error: no point with name " + firstPoint);
    }
    if (!points[secondPoint])
    {
      throw((std::string)"Compile error: no point with name " + secondPoint);
    }

    return lines[name] = new Line(points[firstPoint], points[secondPoint]);
  }
  else if (construction == "Tangent")
  {
    std::string point;
    sstring >> point;
    if (point.empty())
    {
      throw((std::string)"Compile error: not enough args");
    }
    if (!points[point])
    {
      throw((std::string)"Compile error: no point with name " + point);
    }
    if (!dynamic_cast<UnitPoint*>(points[point]))
    {
      throw((std::string)"Compile error: invalid args");
    }

    return lines[name] = new Line(UnitCircle::getInstance(), dynamic_cast<UnitPoint*>(points[point]));
  }
  else if (construction == "Parallel")
  {
    std::string points_, point;
    sstring >> points_;
    sstring >> point;
    if (points_.empty() || point.empty())
    {
      throw((std::string)"Compile error: not enough args");
    }
    if (!lines[points_])
    {
      throw((std::string)"Compile error: no line with name " + points_);
    }
    if (!points[point])
    {
      throw((std::string)"Compile error: no point with name " + point);
    }

    return lines[name] = new Line(lines[points_], points[point]);
  }
  else if (construction == "Perpendicular")
  {
    std::string points_, point;
    sstring >> point;
    sstring >> points_;
    if (points_.empty() || point.empty())
    {
      throw((std::string)"Compile error: not enough args");
    }
    if (!lines[points_])
    {
      throw((std::string)"Compile error: no line with name " + points_);
    }
    if (!points[point])
    {
      throw((std::string)"Compile error: no point with name " + point);
    }

    return lines[name] = new Line(points[point], lines[points_]);
  }
  else
  {
    throw((std::string)"Compile error: undefined construction");
    return nullptr;
  }
}

Object* Compiler::CompileChord(std::stringstream& sstring, std::string name, std::string construction)
{
  if (lines[name])
  {
    throw((std::string)"Compile error: redefinition of object name");
  }
  if (construction == "ByTwoUnitPoints")
  {
    std::string firstPoint, secondPoint;
    sstring >> firstPoint;
    sstring >> secondPoint;
    if (firstPoint.empty() || secondPoint.empty())
    {
      throw((std::string)"Compile error: not enough args");
    }
    if (!points[firstPoint])
    {
      throw((std::string)"Compile error: no point with name " + firstPoint);
    }
    if (!points[secondPoint])
    {
      throw((std::string)"Compile error: no point with name " + secondPoint);
    }
    if (!dynamic_cast<UnitPoint*>(points[firstPoint]) || !dynamic_cast<UnitPoint*>(points[secondPoint]))
    {
      throw((std::string)"Compile error: invalid args");
    }
    return lines[name] = new Chord(dynamic_cast<UnitPoint*>(points[firstPoint]), dynamic_cast<UnitPoint*>(points[secondPoint]));
  }
  else
  {
    throw((std::string)"Compile error: undefined construction");
    return nullptr;
  }
}

void Compiler::CodeLineCompile(std::string points_)
{
  std::stringstream sstring;
  sstring << points_;
  std::string objectType;
  sstring >> objectType;
  if (objectType == "Settings")
  {
    CompileSettings(sstring);
    return;
  }
  std::string name;
  sstring >> name;
  if (name.empty())
  {
    throw((std::string)"Compile error: no name");
  }
  std::string construction;
  sstring >> construction;
  if (construction.empty())
  {
    throw((std::string)"Compile error: null construction");
  }
  NameBox nameBox;
  nameBox.setName(name);
  Object* object;
  if (objectType == "Point")
  {
    object = CompilePoint(sstring, name, construction);
  }
  else if (objectType == "UnitPoint")
  {
    object = CompileUnitPoint(sstring, name, construction);
  }
  else if (objectType == "Line")
  {
    object = CompileLine(sstring, name, construction);
  }
  else if ("Chord")
  {
    object = CompileChord(sstring, name, construction);
  }
  else
  {
    throw((std::string)"Compile error: uncertain object");
    return;
  }
  if (object)
  {
    CompileProperties(sstring, object);
  }
}

void Compiler::Compile(std::string filePath)
{
  points["O"] = CenterPoint::getInstance();
  std::ifstream file;
  file.open(filePath);
  int lineNumber = 0;
  while (!file.eof())
  {
    std::string points_;
    lineNumber++;
    std::getline(file, points_);
    if (points_.empty() || points_[0] == '#')
    {
      continue;
    }
    try
    {
      CodeLineCompile(points_);
    }
    catch (std::string error)
    {
      std::cout << error << " in line " << lineNumber << std::endl << points_ << std::endl;
      mainWindow.close();
      system("pause");
      exit(0);
    }
  }
}