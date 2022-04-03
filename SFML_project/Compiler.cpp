#include "Compiler.h"

std::map<std::string, Point*> Compiler::points;
std::map<std::string, Line*> Compiler::lines;

extern void constructPreset();



void Compiler::compileSettings(std::stringstream& sstring)
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

void Compiler::compilePoint(std::stringstream& sstring, std::string name, std::string construction)
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

		points[name] = new Point(position);
		return;
	}
	else if (construction == "IntersectionOfTwoLines")
	{
		std::string firstLine, secondLine;
		sstring >> firstLine;
		sstring >> secondLine;
		if (firstLine.empty() || secondLine.empty())
		{
			throw((std::string)"Compile error: not enough args");
		}
		if (!lines[firstLine])
		{
			throw((std::string)"Compile error: no line with name " + firstLine);
		}
		if (!lines[secondLine])
		{
			throw((std::string)"Compile error: no line with name " + secondLine);
		}


		points[name] = new Point(lines[firstLine], lines[secondLine]);
		return;
	}
	else if (construction == "OnLine")
	{
		std::string line, firstPoint, secondPoint, coordX, coordY;
		sstring >> line;
		sstring >> firstPoint;
		sstring >> secondPoint;
		sstring >> coordX;
		sstring >> coordY;
		if (line.empty() || firstPoint.empty() || secondPoint.empty() || coordX.empty() || coordY.empty())
		{
			throw((std::string)"Compile error: not enough args");
		}
		Vector2f position(std::stof(coordX), std::stof(coordY));

		if (!lines[line])
		{
			throw((std::string)"Compile error: no line with name " + line);
		}
		if (!points[firstPoint])
		{
			throw((std::string)"Compile error: no point with name " + firstPoint);
		}
		if (!points[secondPoint])
		{
			throw((std::string)"Compile error: no point with name " + secondPoint);
		}

		points[name] = new Point(lines[line], points[firstPoint], points[secondPoint], position);
		return;
	}
	else if (construction == "Projection")
	{
		std::string point, line;
		sstring >> point;
		sstring >> line;
		if (point.empty() || line.empty())
		{
			throw((std::string)"Compile error: not enough args");
		}
		if (!points[point])
		{
			throw((std::string)"Compile error: no point with name " + point);
		}
		if (!lines[line])
		{
			throw((std::string)"Compile error: no line with name " + line);
		}

		points[name] = new Point(points[point], lines[line]);
		return;
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

		points[name] = new Point(points[firstPoint], points[secondPoint], masses);
		return;
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

		points[name] = new Point(points[center], points[preimage], stoi(sign));
		return;
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

		points[name] = new Point(dynamic_cast<UnitPoint*>(points[first]),
			dynamic_cast<UnitPoint*>(points[second]),
			dynamic_cast<UnitPoint*>(points[third]), Point::triangleCenter::ORTHOCENTER);
		return;
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

		points[name] = new Point(points[first], points[second], points[third]);
		return;
	}
	else
	{
		throw((std::string)"Compile error: undefined construction");
	}
}

void Compiler::compileUnitPoint(std::stringstream& sstring, std::string name, std::string construction)
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

		points[name] = new UnitPoint(UnitCircle::getInstance(), position);
		return;
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
		points[name] = new UnitPoint(UnitCircle::getInstance(), points[first], dynamic_cast<UnitPoint*>(points[second]));
		return;
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
		points[name] = new UnitPoint(UnitCircle::getInstance(), dynamic_cast<Chord*>(lines[first]), dynamic_cast<UnitPoint*>(points[second]));
		return;
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
		points[name] = new UnitPoint(UnitCircle::getInstance(), dynamic_cast<UnitPoint*>(points[first]), dynamic_cast<Chord*>(lines[second]));
		return;
	}
}

void Compiler::compileLine(std::stringstream& sstring, std::string name, std::string construction)
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

		lines[name] = new Line(points[firstPoint], points[secondPoint]);
		return;
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

		lines[name] = new Line(UnitCircle::getInstance(), dynamic_cast<UnitPoint*>(points[point]));
		return;
	}
	else if (construction == "Parallel")
	{
		std::string line, point;
		sstring >> line;
		sstring >> point;
		if (line.empty() || point.empty())
		{
			throw((std::string)"Compile error: not enough args");
		}
		if (!lines[line])
		{
			throw((std::string)"Compile error: no line with name " + line);
		}
		if (!points[point])
		{
			throw((std::string)"Compile error: no point with name " + point);
		}

		lines[name] = new Line(lines[line], points[point]);
		return;
	}
	else if (construction == "Perpendicular")
	{
		std::string line, point;
		sstring >> point;
		sstring >> line;
		if (line.empty() || point.empty())
		{
			throw((std::string)"Compile error: not enough args");
		}
		if (!lines[line])
		{
			throw((std::string)"Compile error: no line with name " + line);
		}
		if (!points[point])
		{
			throw((std::string)"Compile error: no point with name " + point);
		}

		lines[name] = new Line(points[point], lines[line]);
		return;
	}
	else
	{
		throw((std::string)"Compile error: undefined construction");
	}
}

void Compiler::compileChord(std::stringstream& sstring, std::string name, std::string construction)
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
		lines[name] = new Chord(dynamic_cast<UnitPoint*>(points[firstPoint]), dynamic_cast<UnitPoint*>(points[secondPoint]));
		return;
	}
	else
	{
		throw((std::string)"Compile error: undefined construction");
		return;
	}
}

void Compiler::codeLineCompile(std::string line)
{
	std::stringstream sstring;
	sstring << line;
	std::string objectType;
	sstring >> objectType;
	if (objectType == "Settings")
	{
		compileSettings(sstring);
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
	if (objectType == "Point")
	{
		compilePoint(sstring, name, construction);
		return;
	}
	else if (objectType == "UnitPoint")
	{
		compileUnitPoint(sstring, name, construction);
		return;
	}
	else if (objectType == "Line")
	{
		compileLine(sstring, name, construction);
		return;
	}
	else if ("Chord")
	{
		compileChord(sstring, name, construction);
	}
	else
	{
		throw((std::string)"Compile error: uncertain object");
		return;
	}
}

void Compiler::compile(std::string filePath)
{
	std::ifstream file;
	file.open(filePath);
	int lineNumber = 0;
	while (!file.eof())
	{
		std::string line;
		lineNumber++;
		std::getline(file, line);
		if (line.empty() || line[0] == '#')
		{
			continue;
		}
		try
		{
			codeLineCompile(line);
		}
		catch (std::string error)
		{
			std::cout << error << " in line " << lineNumber << std::endl;
			mainWindow.close();
			system("pause");
			exit(0);
		}
	}
}