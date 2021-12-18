#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <future>
#include <functional>

using namespace std;
using namespace sf;

class WrapMouse
{
public:
	static Vector2i leftClickedCoord;
	static Vector2i rightClickedCoord;
	static bool leftClicked;
	static bool rightClicked;
	static void setClicked(Mouse::Button button, bool click);
	static void setClickedCoord(Mouse::Button button);
public:
	static bool getClicked(Mouse::Button button);
	static Vector2i getClickedCoord(Mouse::Button button);
	static bool checkPress(Mouse::Button button);
};