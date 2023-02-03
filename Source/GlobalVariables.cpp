#include "gui.h"
#include "Object.h"

float const unitSeg = 200; //Unit seg-200 units in SFMl "=" 1 unit in real world
RenderWindow mainWindow(VideoMode(1000, 1000), "Solver", Style::Default, ContextSettings(0, 0, 8)); //mainWindow (only one creates in this project)
View view = mainWindow.getDefaultView(); //main View (on the plain)
float const epsilon = 30; //In pixels
FloatRect mainWindowRect(0.f, 0.3f, 1.0f, 1.0f); //Menu-Plain ratio
Menu mainMenu(mainWindow); //Main menu(with buttons) on the screen
MODES Mousemode = MODE_NOTHING; //Current MODE of mouse
std::mutex objectDestructionMutex; //Prevents drawing and destructing objects simultaneously