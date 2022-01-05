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
	Creation();
	static Creation* creator;
	//static MODES last;
	static Checker checker;
public:
	function<void(std::stop_token)> CurrentMethod = [](std::stop_token) {
		std::this_thread::sleep_for(std::chrono::nanoseconds(1000));
		Create();
		return;
	};
private:
	jthread running;
	static bool created;
public:
	static Creation* getInstance();
	void operator()();
	static MODES getCurrentMode();
	static void Create();
};

class VisibleObject;
class Parametr;
class Drawer
{
public:
	Drawer() = delete;
	static list<VisibleObject*> allVisibleObjects;
	static list<Parametr*> allParametrs;
	static void draw();
};