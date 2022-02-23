#pragma once
#include "Object.h"

class Prover
{
	static bool started;
	static bool theorem;
	static std::thread provingThread;
public:
	static void proveCollinearity(Point* first, Point* second, Point* third);
	static void proveConcurrency(Line* first, Line* second, Line* third);

};