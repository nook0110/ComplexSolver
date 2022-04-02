#include "Compiler.h"
#include <sstream>
std::map<std::string, Point*> Compiler::points;
std::map<std::string, Line*> Compiler::lines;

void Compiler::lineCompile(std::string line)
{
	std::stringstream sstring;
	sstring << line;
	std::string objectType;
	sstring >> objectType;
	if (objectType.empty())
	{
		throw("Compile error: null object");
	}
	std::string name;
	sstring >> name;
	if (name.empty())
	{
		throw("Compile error: no name");
	}
	std::string construction;
	sstring >> construction;
	if (construction.empty())
	{
		throw("Compile error: null construction");
	}
	NameBox nameBox;
	nameBox.setName(name);
	if (objectType == "Point")
	{
		if (points[name])
		{
			throw("Compile error: redefinition of object name");
		}
		if (construction == "OnPlane")
		{
			std::string coordX, coordY;
			sstring >> coordX;
			sstring >> coordY;
			if (coordX.empty() || coordY.empty())
			{
				throw("Compile error: not enough args");
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
				throw("Compile error: not enough args");
			}
			if (!lines[firstLine])
			{
				throw("Compile error: no line with name " + firstLine);
			}
			if (!lines[secondLine])
			{
				throw("Compile error: no line with name " + secondLine);
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
				throw("Compile error: not enough args");
			}
			Vector2f position(std::stof(coordX), std::stof(coordY));

			if (!lines[line])
			{
				throw("Compile error: no line with name " + line);
			}
			if (!points[firstPoint])
			{
				throw("Compile error: no point with name " + firstPoint);
			}
			if (!points[secondPoint])
			{
				throw("Compile error: no point with name " + secondPoint);
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
				throw("Compile error: not enough args");
			}
			if (!points[point])
			{
				throw("Compile error: no point with name " + point);
			}
			if (!lines[line])
			{
				throw("Compile error: no line with name " + line);
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
			if (line.empty() || firstPoint.empty() || secondPoint.empty() || firstMass.empty() || secondMass.empty())
			{
				throw("Compile error: not enough args");
			}
			std::pair<int, int> masses(std::stoi(firstMass), std::stoi(secondMass));

			if (!lines[line])
			{
				throw("Compile error: no line with name " + line);
			}
			if (!points[firstPoint])
			{
				throw("Compile error: no point with name " + firstPoint);
			}
			if (!points[secondPoint])
			{
				throw("Compile error: no point with name " + secondPoint);
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
				throw("Compile error: not enough args");
			}
			if (!points[center])
			{
				throw("Compile error: no point with name " + center);
			}
			if (!points[preimage])
			{
				throw("Compile error: no point with name " + preimage);
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
				throw("Compile error: not enough args");
			}
			if (!points[first])
			{
				throw("Compile error: no point with name " + first);
			}
			if (!points[second])
			{
				throw("Compile error: no point with name " + second);
			}
			if (!points[third])
			{
				throw("Compile error: no point with name " + third);
			}

			if (!dynamic_cast<UnitPoint*>(points[first]) || !dynamic_cast<UnitPoint*>(points[second]) || !dynamic_cast<UnitPoint*>(points[third]))
			{
				throw("Compile error: invalid args");
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
				throw("Compile error: not enough args");
			}
			if (!points[first])
			{
				throw("Compile error: no point with name " + first);
			}
			if (!points[second])
			{
				throw("Compile error: no point with name " + second);
			}
			if (!points[third])
			{
				throw("Compile error: no point with name " + third);
			}

			points[name] = new Point(points[first], points[second], points[third]);
			return;
		}
		else
		{
			throw("Compile error: undefined construction");
		}
	}
	else if (objectType == "UnitPoint")
	{
		if (construction == "OnCircle")
		{
			std::string coordX, coordY;
			sstring >> coordX;
			sstring >> coordY;
			if (coordX.empty() || coordY.empty())
			{
				throw("Compile error: not enough args");
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
				throw("Compile error: not enough args");
			}
			if (!points[first])
			{
				throw("Compile error: no point with name " + first);
			}
			if (!points[second])
			{
				throw("Compile error: no point with name " + second);
			}
			if (!dynamic_cast<UnitPoint*>(points[second]))
			{
				throw("Compile error: invalid args");
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
				throw("Compile error: not enough args");
			}
			if (!lines[first])
			{
				throw("Compile error: no line with name " + first);
			}
			if (!points[second])
			{
				throw("Compile error: no point with name " + second);
			}
			if (!dynamic_cast<Chord*>(lines[first]) || !dynamic_cast<UnitPoint*>(points[second]))
			{
				throw("Compile error: invalid args");
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
				throw("Compile error: not enough args");
			}
			if (!points[first])
			{
				throw("Compile error: no line with name " + first);
			}
			if (!lines[second])
			{
				throw("Compile error: no point with name " + second);
			}
			if (!dynamic_cast<UnitPoint*>(points[first]) || !dynamic_cast<Chord*>(lines[second]))
			{
				throw("Compile error: invalid args");
			}
			points[name] = new UnitPoint(UnitCircle::getInstance(), dynamic_cast<UnitPoint*>(points[first]), dynamic_cast<Chord*>(lines[second]));
			return;
		}
	}
	else if (objectType == "Line")
	{
		if (lines[name])
		{
			throw("Compile error: redefinition of object name");
		}
		if (construction == "ByTwoPoints")
		{
			std::string firstPoint, secondPoint;
			sstring >> firstPoint;
			sstring >> secondPoint;
			if (firstPoint.empty() || secondPoint.empty())
			{
				throw("Compile error: not enough args");
			}
			if (!points[firstPoint])
			{
				throw("Compile error: no point with name " + firstPoint);
			}
			if (!points[secondPoint])
			{
				throw("Compile error: no point with name " + secondPoint);
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
				throw("Compile error: not enough args");
			}
			if (!points[point])
			{
				throw("Compile error: no point with name " + point);
			}
			if (!dynamic_cast<UnitPoint*>(points[point]))
			{
				throw("Compile error: invalid args");
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
				throw("Compile error: not enough args");
			}
			if (!lines[line])
			{
				throw("Compile error: no line with name " + line);
			}
			if (!points[point])
			{
				throw("Compile error: no point with name " + point);
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
				throw("Compile error: not enough args");
			}
			if (!lines[line])
			{
				throw("Compile error: no line with name " + line);
			}
			if (!points[point])
			{
				throw("Compile error: no point with name " + point);
			}

			lines[name] = new Line(points[point], lines[line]);
			return;
		}
		else
		{
			throw("Compile error: undefined construction");
		}
	}
	else if ("Chord")
	{
		if (lines[name])
		{
			throw("Compile error: redefinition of object name");
		}
		if (construction == "ByTwoUnitPoints")
		{
			std::string firstPoint, secondPoint;
			sstring >> firstPoint;
			sstring >> secondPoint;
			if (firstPoint.empty() || secondPoint.empty())
			{
				throw("Compile error: not enough args");
			}
			if (!points[firstPoint])
			{
				throw("Compile error: no point with name " + firstPoint);
			}
			if (!points[secondPoint])
			{
				throw("Compile error: no point with name " + secondPoint);
			}
			if (!dynamic_cast<UnitPoint*>(points[firstPoint]) || !dynamic_cast<UnitPoint*>(points[secondPoint]))
			{
				throw("Compile error: invalid args");
			}
			lines[name] = new Chord(dynamic_cast<UnitPoint*>(points[firstPoint]), dynamic_cast<UnitPoint*>(points[secondPoint]));
			return;
		}
		else
		{
			throw("Compile error: undefined construction");
		}
	}
	else
	{
		throw("Compile error: uncertain object");
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
		try
		{
			lineCompile(line);
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