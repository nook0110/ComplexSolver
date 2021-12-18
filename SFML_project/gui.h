#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <future>
#include <functional>

using namespace std;
using namespace sf;

extern Vector2i maxTextureResolution;


enum MODES
{
	MODE_NOTHING = 0,
	MODE_MOVE,
	MODE_POINT,
	MODE_LINE,
	MODE_PERPENDICULAR,
	MODE_PERPENDICULAR_BISECTOR,
	MODE_TANGENT,
	MODE_MIDPOINT,
	MODE_PARALLEL,
	MODE_REFLECT_LINE,
	MODE_REFLECT_POINT,
	MODE_INTERSECT
};





