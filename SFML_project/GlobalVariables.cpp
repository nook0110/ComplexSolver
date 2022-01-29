#include "gui.h"
#include "Object.h"

//namespace Global
//{
double const unitSeg = 200;
std::thread Creator;
ContextSettings settings(0, 0, 8);
RenderWindow mainWindow(VideoMode(1000,1000), "Version 0.5", Style::Default, settings);

View view = mainWindow.getDefaultView();
double const epsilon = unitSeg / 10;
Vector2i maxTextureResolution = Vector2i(10000, 10000);
Menu mainMenu(&mainWindow);
MODES Mousemode = MODE_NOTHING;
FloatRect mainWindowRect(0.f, 0.2f, 1.0f, 1.0f);
//}