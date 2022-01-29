#pragma once
#include <cmath>
#include <functional>
#include <thread>
#include "Object.h"
#include "gui.h" 
class Menu;
extern Menu mainMenu;
extern RenderWindow mainWindow;

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


class Creation
{
	Creation();
	~Creation();
	static Creation* creator;
	//static MODES last;
	static Checker checker;
public:
	std::function<void(void)> CurrentMethod = [](void) {
		std::this_thread::sleep_for(std::chrono::nanoseconds(1000));
		Create();
		return;
	};
private:
	static bool created;

	std::thread* running = new std::thread([&]() {
		while (true)
		{
			std::this_thread::sleep_for(std::chrono::nanoseconds(1000));
			if (created && getCurrentMode() != MODE_NOTHING)
			{
				created = false;
				try
				{
					CurrentMethod();
				}
				catch (const std::exception& ex)
				{
					created = true;
				}
			}
			if (!checker.checkMode())
			{
				created = true;
			}
		}
		});
public:
	static Creation* getInstance();
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
	static void drawProveObject();
	static void resizeMenu(Event event);
	static void updateMenu();
	static void resizeTextBoxes(Event event);
	static void resizeDialogBox(Event event);
public:
	Drawer() = delete;
	static std::list<VisibleObject*> allVisibleObjects;
	static std::list<Parametr*> allParametrs;
	static std::list<TextBox*> allTextBoxes;
	static DialogBox* dialogBox;
	static void update(Event event);
	static void draw();
};