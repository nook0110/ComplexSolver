#pragma once
#include "Object.h"
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
	function<VisibleObject* (void)> CurrentMethod = []()->VisibleObject* {
		std::this_thread::sleep_for(std::chrono::seconds(1));
		Create();
		return nullptr;
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
