#include "gui.h"
#include "Object.h"

double const unitSeg = 200;
RenderWindow mainWindow(VideoMode(1000,1000), "Version 0.6", Style::Default, ContextSettings(0, 0, 8));
View view = mainWindow.getDefaultView();
double const epsilon = 50; //In pixels
Vector2i maxTextureResolution = Vector2i(10000, 10000);
FloatRect mainWindowRect(0.f, 0.3f, 1.0f, 1.0f);
Menu mainMenu(&mainWindow);
MODES Mousemode = MODE_NOTHING;