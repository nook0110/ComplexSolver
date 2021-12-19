#include "Button.h"

using namespace std;
using namespace sf;

extern RenderWindow window;
extern View view;
extern Vector2i maxTextureResolution;

extern MODES Mousemode;

void Button::setTexture(string _textureLocation, Vector2i _textureStart, Vector2i _textureSize)
{
	texture.loadFromFile(_textureLocation, IntRect(_textureStart, _textureSize));
	Sprite.setScale(1 / Sprite.getScale().x, 1 / Sprite.getScale().y); // Scaling Sprite to 1*1;
	Sprite.setTexture(texture);
	Sprite.setScale(
		size.x / Sprite.getLocalBounds().width,
		size.y / Sprite.getLocalBounds().height); // Scaling Sprite to required size;

}



Button::Button(Vector2f _position, Vector2f _size, RenderWindow* _window,
	string _textureLocation, Vector2i _textureStart, Vector2i _textureSize,
	MODES _mode, function<void(void)> _modeFunction)
	:position(_position),
	size(_size),
	window(_window),
	textureSize(_textureSize),
	textureStart(_textureStart),
	textureLocation(_textureLocation),
	mode(_mode), modeFunction(_modeFunction)
{
	setTexture(textureLocation, textureStart, textureSize);
}

Button::Button(Vector2f _position, Vector2f _size, RenderWindow* _window,
	string _textureLocation, string _texturePressedLocation,
	MODES _mode, function<void(void)> _modeFunction)
	:position(_position),
	size(_size),
	window(_window),
	textureLocation(_textureLocation),
	texturePressedLocation(_texturePressedLocation),
	texturePressed(true),
	mode(_mode), modeFunction(_modeFunction)
{
	setTexture(textureLocation, textureStart, textureSize);
}

Button::Button(Vector2f _position, Vector2f _size, RenderWindow* _window,
	string _textureLocation, Vector2i _textureStart, Vector2i _textureSize,
	string _texturePressedLocation, Vector2i _texturePressedStart, Vector2i _texturePressedSize,
	MODES _mode, function<void(void)> _modeFunction)
	:position(_position),
	size(_size),
	window(_window),
	textureSize(_textureSize), texturePressedSize(_texturePressedSize),
	textureStart(_textureStart), texturePressedStart(_texturePressedStart),
	textureLocation(_textureLocation), texturePressedLocation(_texturePressedLocation),
	texturePressed(true),
	mode(_mode), modeFunction(_modeFunction)
{
	setTexture(textureLocation, textureStart, textureSize);
}

void Button::setPosition(Vector2f _position)
{
	position = _position;
	Sprite.setPosition(position);
}

void Button::draw() {
	(*window).draw(Sprite);
}

bool Button::mouseCheck(View buttonView = view)
{
	Vector2f point = (*window).mapPixelToCoords(WrapMouse::getClickedCoord(Mouse::Button::Left), buttonView);
	//cout << MouseCoords.x + Offset.x;
	return Sprite.getGlobalBounds().contains(point);
}

bool Button::leftClickCheck(View buttonView = view)
{
	Vector2f point = (*window).mapPixelToCoords(WrapMouse::getClickedCoord(Mouse::Button::Left), buttonView);
	if (!Mouse::isButtonPressed(Mouse::Button::Left)) LeftPressed = false;
	if (!LeftPressed
		&& Sprite.getGlobalBounds().contains(point)
		&& Mouse::isButtonPressed(Mouse::Button::Left))
	{
		LeftPressed = true;
		return true;
	}
	else return false;
}

bool Button::rightClickCheck(View buttonView = view)
{
	Vector2f point = (*window).mapPixelToCoords(WrapMouse::getClickedCoord(Mouse::Button::Right), buttonView);
	if (!Mouse::isButtonPressed(Mouse::Button::Right)) RightPressed = false;
	if (!RightPressed && Sprite.getGlobalBounds().contains(point)) {
		RightPressed = true;
		return true;
	}
	else return false;
}

Vector2f Button::getSize()
{
	return size;
}

Vector2f Button::getLocalPosition()
{
	return  position;
}

Vector2f Button::getGlobalPosition()
{
	return  Sprite.getPosition();
}

void Button::setMode()
{
	Mousemode = mode;
}


void Button::press()
{
	pressed = true;
	setMode();
	if (texturePressed)
	{
		setTexture(texturePressedLocation, texturePressedStart, texturePressedSize);
	}
	else
	{
		Sprite.setColor(GreyColor);
	}
}

void Button::unpress()
{
	pressed = false;
	if (texturePressed)
	{
		setTexture(textureLocation, textureStart, textureSize);
	}
	else
	{
		Sprite.setColor(WhiteColor);
	}
}

bool Button::getPressed()
{
	return pressed;
}

function<void(void)> Button::getObjectCreationMethod()
{
	return modeFunction;
}




Menu::Menu(RenderWindow* _window)
{
	window = _window;
	menuView = (*window).getDefaultView();
	menuView.setSize(menuView.getSize().x * viewport.width, menuView.getSize().y * viewport.height);
	menuView.setCenter(menuView.getCenter().x * viewport.width, menuView.getCenter().x * viewport.height);
	menuView.setViewport(viewport);
	background.setFillColor(color);

}
void Menu::update(Event event)
{
	sf::FloatRect visibleArea(menuView.getCenter().x - menuView.getSize().x / 2, menuView.getCenter().y - menuView.getSize().y / 2, event.size.width * viewport.width, event.size.height * viewport.height);
	(*window).setView(sf::View(visibleArea));
	menuView = sf::View(visibleArea);
}


void Menu::pushButton(Button newButton)
{
	buttons.push_back(newButton);
}

bool Menu::checkMouse()
{
	(*window).setView(menuView);
	for (auto& Button : buttons)
	{
		if (Button.mouseCheck(menuView))
		{
			return true;
		}
	}
	return false;
}
Button* Menu::leftClickCheck()
{
	(*window).setView(menuView);
	for (auto& Button : buttons)
	{
		if (Button.leftClickCheck(menuView))
		{
			return &Button;
		}
	}
	return nullptr;
}

void Menu::unpress()
{
	(*window).setView(menuView);
	for (auto& Button : buttons)
	{
		Button.unpress();
	}
}
void Menu::draw()
{
	(*window).setView(menuView);
	//update();
	//cout << (*window).mapPixelToCoords(Mouse::getPosition(*window), menuView).x << " " << (*window).mapPixelToCoords(Mouse::getPosition(*window), menuView).y << endl;
	menuView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 0.2f));
	background.setSize(menuView.getSize());
	background.setPosition(position);
	(*window).draw(background);
	for (auto& Button : buttons)
	{
		Button.setPosition(Button.getLocalPosition());
		Button.draw();
	}

}

/*
void Menu::update()
{
	for (auto& Button : buttons)
	{
		Button.updatePosition();
	}
}
*/
/*
void Menu::reposition()
{
	update();
	double xmin = view.getCenter().x + (view.getSize().x / 2),
		ymin = view.getCenter().y + (view.getSize().y / 2);
	for (auto& Button : buttons)
	{
		if (Button.getLocalPosition().x < xmin)
		{
			xmin = Button.getLocalPosition().x;
		}
		if (Button.getLocalPosition().y < ymin)
		{
			ymin = Button.getLocalPosition().y;
		}
	}
	position = Vector2f(xmin, ymin);
}

Vector2f Menu::getLocalPosition()
{

	return position;
}

Vector2f Menu::getGlobalPosition()
{
	double xmin = view.getCenter().x - (view.getSize().x / 2),
		ymin = view.getCenter().y - (view.getSize().y / 2);
	for (auto& Button : buttons)
	{
		if (Button.getGlobalPosition().x < xmin)
		{
			xmin = Button.getGlobalPosition().x;
		}
		if (Button.getGlobalPosition().y < ymin)
		{
			ymin = Button.getGlobalPosition().y;
		}
	}
	return Vector2f(xmin, ymin);
}
*/
/*
void Menu::resize()
{

	double
		xmin = view.getCenter().x + (view.getSize().x / 2),
		xmax = view.getCenter().x - (view.getSize().x / 2),
		ymin = view.getCenter().y + (view.getSize().y / 2),
		ymax = view.getCenter().y - (view.getSize().y / 2);
	for (auto& Button : buttons)
	{
		if (Button.getGlobalPosition().x < xmin)
		{
			xmin = Button.getGlobalPosition().x;
		}
		if (Button.getGlobalPosition().y < ymin)
		{
			ymin = Button.getGlobalPosition().y;
		}

		if (Button.getGlobalPosition().x + Button.getSize().x > xmax)
		{
			xmax = Button.getGlobalPosition().x + Button.getSize().x;
		}

		if (Button.getGlobalPosition().y + Button.getSize().x > ymax)
		{
			ymax = Button.getGlobalPosition().y + Button.getSize().y;
		}
	}
	Outline = ymin;
	size = Vector2f(view.getCenter().x + (view.getSize().x / 2), ymax + Outline);

}
*/
/*
Vector2f Menu::getSize()
{
	return size;
}


*/