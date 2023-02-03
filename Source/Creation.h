#pragma once
#include <cmath>
#include <functional>
#include <thread>
#include "Object.h"
#include "gui.h"
#include "Memento.h"

//Creation objects and drawing them

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
class Creation
{
  Creation();
  ~Creation();
  static Checker checker;
  static Creation* creator;
public:
  std::function<Object* (void)>  CurrentMethod = [](void)->Object*
  {
    std::this_thread::sleep_for(std::chrono::nanoseconds(1000));
    return nullptr;
  };
private:

  std::thread* running = new std::thread([&]()
  {
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
//Draws everything on the screen and updates it
class Drawer
{
  static void drawObjects();
  static void drawDescriptions();
  static void drawDialogBox();
  static void resizeMenu(Event event);
  static void updateMenu();
  static void resizeDialogBox(Event event);
public:
  Drawer() = delete;
  //List of all objects
  static std::list<Object*> all_visible_objects;
  static bool VisibleObjectsContains(Object* ptr);
  static void delObject(Object* object);
  static void addObject(Object* object);
  //List of all descriptions
  static std::list<Description*> allDescriptions;
  //Dialog box (Only 1 cfn be on the screen);
  static DialogBox* dialogBox;
  //Updates menu(Resize, Clicl);
  static void updateMenu(Event event);
  //Updates dialog box (Resize and cin);
  static void update(Event event);
  //Draws all objects
  static void Draw();
  //Draw menu
  static void drawMenu();
};