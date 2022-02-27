#pragma once
#include "Object.h"
#include <fstream>
class Prover
{
	static bool started;
	static bool finished;
	static bool theorem;
	static std::thread provingThread;
	static std::ofstream fileTEX;
	static std::ofstream fileDVI;
public:
	static void makeFiles(std::string);
	static void proveCollinearity(Point* first, Point* second, Point* third);
	static void proveConcurrency(Line* first, Line* second, Line* third);
	static bool getStarted();
	static bool getFinished();
	static bool getTheorem();
};