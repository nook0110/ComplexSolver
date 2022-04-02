#pragma once
#include "Object.h"
#include "gui.h"

#include <fstream>
#include <map>


class Compiler
{
	static std::map<std::string, Point*> points;
	static std::map<std::string, Line*> lines;
public:
	static void lineCompile(std::string line);
	static void compile(std::string filePath);
};