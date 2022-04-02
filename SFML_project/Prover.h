#pragma once
#include "Object.h"
#include "Creation.h"
class Prover
{
	static bool started;
	static std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
	static std::chrono::time_point<std::chrono::high_resolution_clock> finishTime;
	static bool finished;
	static bool theorem;
	static std::thread* provingThread;
	static void start();
	static void finish();
public:
	static void updateTextBox();
	static void proveCollinearity(Point* first, Point* second, Point* third);
	static void proveConcurrency(Line* first, Line* second, Line* third);
	static void proveInscription(Point* first, Point* second, Point* third, Point* fourth);
	static void proveConstruction(Point* first, Point* second);
	static void proveEquivalence(Point* first, Point* second, Point* third, Point* fourth);
	static void proveParallel(Line* first, Line* second);
	static void proveOrthogonality(Line* first, Line* second);
	// Returns if provingThread (Proving the theorem) started
	static bool getStarted();
	// Returns if provingThread (Proving the theorem) finished
	static bool getFinished();
	// Returns is the theorem true or false
	// It is undefined(returns false) before proving finished
	static bool getTheorem();
};