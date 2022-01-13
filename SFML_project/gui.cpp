#include "gui.h"

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

bool Menu::mouseOnMenu()
{
	Vector2f mousePosition = (*window).mapPixelToCoords(Mouse::getPosition(*window), menuView);
	return background.getGlobalBounds().contains(mousePosition);
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
	Mousemode = MODE_NOTHING;
}

void Menu::draw()
{
	(*window).setView(menuView);
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

DialogBox* Drawer::dialogBox;
list<TextBox*> Drawer::allTextBoxes;
DialogBox::DialogBox(RenderWindow* window)
	:window(window)
{
	if (Drawer::dialogBox)
	{
		throw runtime_error("Second DialogBox is not supported");
	}
	Drawer::dialogBox = this;
	font.loadFromFile("Textures\\SFML_project\\Fonts\\arial.ttf");
	dialogBox.setFillColor(color);
	shadow.setFillColor(shadowColor);
}

void DialogBox::update(Event event)
{
	sf::FloatRect visibleArea(dialogBoxView.getCenter().x - dialogBoxView.getSize().x / 2, dialogBoxView.getCenter().y - dialogBoxView.getSize().y / 2, event.size.width * viewport.width, event.size.height * viewport.height);
	(*window).setView(sf::View(visibleArea));
	dialogBoxView = sf::View(visibleArea);
}

void DialogBox::cin(Event event)
{
	switch (event.text.unicode)
	{
	case 8:
		if (!textIn.empty())
		{
			textIn.pop_back();
		}
		break;
	case 13:
		finished = true;
		break;
	default:
		Text text(textIn, font, textSize);
		if (text.getLocalBounds().width < sizeTextBox.x - 2 * textOffset.x)
		{
			textIn += event.text.unicode;
		}
	}
}

void DialogBox::draw()
{
	(*window).setView(dialogBoxView);
	Vector2f position = dialogBoxView.getCenter() - sizeDialogBox / 2.f;
	dialogBox.setPosition(position);
	textBox.setPosition(position + textBoxOffset);
	shadow.setPosition(position + shadowOffset);
	window->draw(shadow);
	window->draw(dialogBox);
	window->draw(textBox);
	Text text(textIn, font, textSize);
	text.setPosition(position + textBoxOffset + textOffset);
	text.setFillColor(Color::Black);
	window->draw(text);
}

bool DialogBox::isFinished()
{
	return finished;
}

double DialogBox::getDouble()
{
	auto position = textIn.find(":");
	try
	{
		double firstNumber = stod(textIn.substr(0, position));
		double secondNumber = stod(textIn.substr(position + 1));
		return firstNumber / secondNumber;
	}
	catch (invalid_argument)
	{
		return 1;
	}
}

DialogBox::~DialogBox()
{
	Drawer::dialogBox = nullptr;
}

void TextBox::setText(string text)
{
	TextBox::text = text;
}

void TextBox::draw()
{
}
