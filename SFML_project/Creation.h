#pragma once
#include <cmath>
#include <functional>
#include <thread>
#include "Object.h"

using namespace std;

class VisibleObject;
class Checker
{
private:
	MODES last;
public:
	bool checkMode();
};

class Creation
{
private:
	//static MODES last;
	Checker checker;
public:
	function<void (void)> CurrentMethod = [](){
		std::this_thread::sleep_for(std::chrono::nanoseconds(1000));
		Create();
		return;
	};
private:
	thread running;
	static bool created;
public:
	void operator()();
	/*static bool checkMode()
	{
		if (last != Mousemode)
		{
			created = true;
			last = Mousemode;
		}
		return last == Mousemode;
	}*/
	static void Create();
};
