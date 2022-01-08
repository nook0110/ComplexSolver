#pragma once
#include <cmath>
#include <functional>
#include <thread>
#include "Object.h"
#include "gui.h" 

using namespace std;
class Menu;
extern Menu mainMenu;


class VisibleObject;
//Checks if mode has been changed
class Checker
{
private:
	MODES last = MODE_NOTHING;
public:
	MODES getLast();
	bool checkMode();
};

//Changes the thread with thread that creates another object
//Use operator () to run thread or check if thread is needed to be changed
void tes();

class Creation
{
	Creation();
	static Creation* creator;
	//static MODES last;
	static Checker checker;
public:
	function<void(void)> CurrentMethod = [&](void) {
		std::this_thread::sleep_for(std::chrono::nanoseconds(1000));
		Create();
		return;
	};
private:
	static bool created;

	thread* running = new thread([&]() {
		while (true)
		{
			std::this_thread::sleep_for(std::chrono::nanoseconds(1000));
			if (created && getCurrentMode() != MODE_NOTHING)
			{
				created = false;
				CurrentMethod();
			}
			if (!checker.checkMode())
			{
				created = true;
			}
		}
		});
public:
	static Creation* getInstance();
	void operator()();
	static MODES getCurrentMode();
	static void Create();
};

class VisibleObject;
class Parametr;
class TextBox;
class DialogBox;
class Drawer
{
	static void drawObjects();
	static void drawMenu();
	static void drawTextBoxes();
	static void drawDialogBox();
	static void resizeMenu(Event event);
	static void updateMenu();
	static void resizeTextBoxes(Event event);
	static void resizeDialogBox(Event event);
public:
	Drawer() = delete;
	static list<VisibleObject*> allVisibleObjects;
	static list<Parametr*> allParametrs;
	static list<TextBox*> allTextBoxes;
	static DialogBox* dialogBox;
	static void update(Event event);
	static void draw();
};