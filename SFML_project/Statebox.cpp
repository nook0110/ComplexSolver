#include "Statebox.h"

Statebox::Statebox(Vector2f position, Vector2f size, std::string name, int states)
	:position(position), size(size), name(name), maxState(states)
{
	statesTextures.resize(states);
	statesSprites.resize(states);
	statesTexts.resize(states);
	font.loadFromFile("Textures\\Font\\font.ttf");
	textSize = size.y / 3;
	text = Text(name, font, textSize);
	text.setFillColor(textColor);
	background.setFillColor(backgroundColor);
	
}

void Statebox::setStateTexture(int state, std::string textState, std::string texturePath)
{
	statesTextures[state].loadFromFile(texturePath);
	statesTexts[state] = Text(textState, font, textSize);
	statesTexts[state].setFillColor(Color::Black);
	statesTexts[state].setPosition(position.x + size.x * ratio, position.y + 7 * size.y / 12);
	statesSprites[state].setTexture(statesTextures[state]);
	statesSprites[state].setScale(1 / statesSprites[state].getScale().x, 1 / statesSprites[state].getScale().y); // Scaling Sprite to 1*1;
	statesSprites[state].setScale(
		(size.x * ratio) / statesSprites[state].getLocalBounds().width,
		size.y / statesSprites[state].getLocalBounds().height); // Scaling Sprite to required size;
	update();
}

void Statebox::setPosition(Vector2f position)
{
	Statebox::position = position;
	update();
}

void Statebox::setSize(float sizeY)
{
	size *= sizeY / size.y;
}

bool Statebox::mouseCheck(View clickView = view)
{
	Vector2f point = mainWindow.mapPixelToCoords(WrapMouse::getClickedCoord(Mouse::Button::Left), clickView);
	return background.getGlobalBounds().contains(point);
}

bool Statebox::clickCheck(View clickView = view)
{
	if (mouseCheck(clickView) && Mouse::isButtonPressed(Mouse::Button::Left))
	{
		state++;
		state %= maxState;
		return true;
	}
	else return false;
}

void Statebox::draw()
{
	mainWindow.draw(background);
	mainWindow.draw(statesSprites[state]);
	mainWindow.draw(statesTexts[state]);
	mainWindow.draw(text);
}

int Statebox::getState()
{
	return state;
}

void Statebox::update()
{
	background.setOrigin(size / 2.f);
	textSize = size.y / 3;
	text = Text(name, font, textSize);
	text.setOrigin(size / 2.f);
	text.setFillColor(textColor);
	background.setFillColor(backgroundColor);
	background.setSize(size);
	background.setPosition(position);
	Vector2f textPos = Vector2f(position.x + size.x * ratio, position.y + size.y / 12);
	text.setPosition(textPos);
	for (int state = 0; state < maxState; state++)
	{
		statesTexts[state].setCharacterSize(textSize);
		statesTexts[state].setOrigin(size / 2.f);
		statesTexts[state].setPosition(position.x + size.x * ratio, position.y + 7 * size.y / 12);
		statesSprites[state].setScale(1 / statesSprites[state].getScale().x, 1 / statesSprites[state].getScale().y); // Scaling Sprite to 1*1;
		statesSprites[state].setScale(
			(size.x * ratio) / statesSprites[state].getLocalBounds().width,
			size.y / statesSprites[state].getLocalBounds().height); // Scaling Sprite to required size;
		statesSprites[state].setPosition(position - size / 2.f);
	}
}

void StateMenu::updateBoxes()
{
}

void StateMenu::update()
{
	Vector2f menuSize = mainWindow.getView().getSize();
	float size = menuSize.y / stateboxes.size() / 2;
	float nextSize = size;
	const float coeff = 1.001;
	while (nextSize*stateboxes.size() < menuSize.y - nextSize*shiftRatio)
	{
		size = nextSize;
		nextSize *= coeff;
	}
	size = std::min(100.f, size);
	Vector2f center = mainWindow.getView().getCenter();
	Vector2f delta = Vector2f(0, center.y - size * stateboxes.size() / 2);
	for (int i = 0; i < stateboxes.size(); i++)
	{
		stateboxes[i]->setSize(size);
		Vector2f position = Vector2f(center.x, i * size * (1.f + shiftRatio) + delta.y);
		stateboxes[i]->setPosition(Vector2f(position));
	}
}

StateMenu::StateMenu()
{
}

void StateMenu::pushStatebox(Statebox* newStatebox)
{
	stateboxes.push_back(newStatebox);
}

Statebox* StateMenu::clickCheck()
{
	for (auto& stBox : stateboxes)
	{
		if (stBox->clickCheck())
		{
			return stBox;
		}
	}
	return nullptr;
}

void StateMenu::draw()
{
	for (auto& stBox : stateboxes)
	{
		stBox->draw();
	}
}
