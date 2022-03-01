#pragma once
#include <cmath>
#include <functional>
#include <thread>
#include "Object.h"
#include "gui.h" 
class Menu;
extern Menu mainMenu;
extern RenderWindow mainWindow;
extern std::mutex objectDestructionMutex;

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
			checker.checkMode();
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

		}
		});
public:
	static Creation* getInstance();
	static MODES getCurrentMode();
};

class Object;
class Description;
class TextBox;
class DialogBox;
class Drawer
{
	static void drawObjects();
	static void drawDescriptions();
	static void drawDialogBox();
	static void resizeMenu(Event event);
	static void updateMenu();
	static void resizeTextBoxes(Event event);
	static void resizeDialogBox(Event event);
public:
	Drawer() = delete;
	static std::list<Object*> allVisibleObjects;
	static std::list<Description*> allDescriptions;
	static DialogBox* dialogBox;
	static void updateMenu(Event event);
	static void update(Event event);
	static void draw();
	static void drawMenu();
};