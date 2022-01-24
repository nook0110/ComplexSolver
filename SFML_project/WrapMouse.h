#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <future>
#include <functional>

using namespace std;
using namespace sf;

//All modes of button
enum MODES
{
	MODE_NOTHING = 0,
	MODE_DELETE,
	MODE_CLEAR,
	MODE_MOVE,
	MODE_HIDE,
	MODE_POINT,
	MODE_LINE,
	MODE_CENTRAL_PROJECTION,
	MODE_SYMMETRY,
	MODE_PERPENDICULAR,
	MODE_PERPENDICULAR_BISECTOR,
	MODE_PROJECTION,
	MODE_TANGENT,
	MODE_MIDPOINT,
	MODE_PARALLEL,
	MODE_REFLECT_LINE,
	MODE_REFLECT_POINT,
	MODE_INTERSECT
};

//Wrappper of SFML sf::Mouse
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