#include "Button.h"
#include "Object.h"

double const unitSeg = 200;
thread Creator;
ContextSettings settings(0, 0, 8);
RenderWindow  window(VideoMode(1000, 1000), "Version 0.24", Style::Default, settings);
View view = window.getDefaultView();
double const epsilon = unitSeg / 10;
Vector2i maxTextureResolution = Vector2i(10000, 10000);
Menu mainMenu(&window);
MODES Mousemode = MODE_NOTHING;
FloatRect mainWindowRect(0.f, 0.2f, 1.0f, 1.0f);