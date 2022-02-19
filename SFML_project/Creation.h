#pragma once
#include <cmath>
#include <functional>
#include <thread>
#include "Object.h"
#include "gui.h" 
class Menu;
extern Menu mainMenu;
extern RenderWindow mainWindow;

class Object;
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
	static Checker checker;
	static Creation* creator;
public:
	std::function<Object* (void)>  CurrentMethod = [](void)->Object* {
		std::this_thread::sleep_for(std::chrono::nanoseconds(1000));
		return nullptr;
	};
private:

	std::thread* running = new std::thread([&]() {
		while (true)
		{
			std::this_thread::sleep_for(std::chrono::nanoseconds(1000));
			if (getCurrentMode() != MODE_NOTHING)
			{
				try
				{
					CurrentMethod();
				}
				catch (const std::exception& ex)
				{
				}
			}
			checker.checkMode();
		}
		});
public:
	static Creation* getInstance();
	static MODES getCurrentMode();
};

class Object;
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
	static std::list<Object*> allVisibleObjects;
	static std::list<Parametr*> allParametrs;
	static std::list<TextBox*> allTextBoxes;
	static DialogBox* dialogBox;
	static void update(Event event);
	static void draw();
};