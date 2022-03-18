#include "Checkbox.h"

Checkbox::Checkbox(Vector2f position, Vector2f size, std::string name)
	:position(position), size(size), name(name)
{
	checkboxView = mainWindow.getDefaultView();
	mainWindow.setView(checkboxView);
	font.loadFromFile("Textures\\Font\\font.ttf");
	text = Text(name, font, textSize);
	text.setFillColor(textColor);
	Vector2f textPos = Vector2f(position.x + size.x * ratio, position.y + size.y/2);
	text.setPosition(position);
}

bool Checkbox::mouseCheck(View clickView = view)
{
	Vector2f point = mainWindow.mapPixelToCoords(WrapMouse::getClickedCoord(Mouse::Button::Left), clickView);
	return background.getGlobalBounds().contains(point);
}

bool Checkbox::clickCheck(View clickView = view)
{
	Vector2f point = mainWindow.mapPixelToCoords(WrapMouse::getClickedCoord(Mouse::Button::Left), clickView);
	if (mouseCheck(clickView)
		&& Mouse::isButtonPressed(Mouse::Button::Left))
	{
		state = !state;
		return true;
	}
	else return false;
}

void Checkbox::draw()
{
	mainWindow.setView(checkboxView);
	mainWindow.draw(background);
	mainWindow.draw(text);
}

