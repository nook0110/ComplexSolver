#pragma once
#include <cmath>
#include <functional>
#include <thread>
#include "Object.h"

using namespace std;

class VisibleObject;
//Checks if mode has been changed
class Checker
{
private:
	MODES last;
public:
	MODES getLast();
	bool checkMode();
};

//Changes the thread with thread that creates another object
//Use operator () to run thread or check if thread is needed to be changed
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
	MODES getCurrentMode();
	static void Create();
};
