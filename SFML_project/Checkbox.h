#pragma once
#include <SFML/Graphics.hpp>
#include "WrapMouse.h"
using namespace sf;
extern RenderWindow mainWindow;
extern View view;
class Checkbox
{
private:
	const float ratio = 0.2;
	View checkboxView;
	Vector2f position;
	Vector2f size;
	std::string name;
	Color textColor = Color::Black;
	const unsigned int textSize = 35;
	Font font;
	Text text;
	RectangleShape background;
	bool state;
public:
	Checkbox(Vector2f position, Vector2f size, std::string name);

	bool mouseCheck(View);
	bool clickCheck(View);

	void draw();
};