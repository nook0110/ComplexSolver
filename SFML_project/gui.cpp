#include "gui.h"


extern RenderWindow mainWindow;
extern View view;
extern Vector2i maxTextureResolution;

extern MODES Mousemode;

void Button::updateSprite()
{
	sprite.setScale(1 / sprite.getScale().x, 1 / sprite.getScale().y); // Scaling Sprite to 1*1;
	sprite.setTexture(texture);
	sprite.setScale(
		size.x / sprite.getLocalBounds().width,
		size.y / sprite.getLocalBounds().height); // Scaling Sprite to required size;
}

void Button::setTexture(std::string textureLocation, Vector2i textureStart, Vector2i textureSize)
{
	texture.loadFromFile(textureLocation, IntRect(textureStart, textureSize));
	updateSprite();
}

Button::Button(Vector2f position, Vector2f size, RenderWindow* window,
	std::string textureLocation, Vector2i textureStart, Vector2i textureSize,
	MODES mode, std::function<VisibleObject* (void)> modeFunction)
	:position(position),
	size(size),
	window(window),
	textureSize(textureSize),
	textureStart(textureStart),
	textureLocation(textureLocation),
	mode(mode), modeFunction(modeFunction)
{
	setTexture(textureLocation, textureStart, textureSize);
}

Button::Button(Vector2f position, Vector2f size, RenderWindow* window,
	std::string textureLocation, std::string texturePressedLocation,
	MODES mode, std::function<VisibleObject* (void)> modeFunction)
	:position(position),
	size(size),
	window(window),
	textureLocation(textureLocation),
	texturePressedLocation(texturePressedLocation),
	texturePressed(true),
	mode(mode), modeFunction(modeFunction)
{
	setTexture(textureLocation, textureStart, textureSize);
}

Button::Button(Vector2f position, Vector2f size, RenderWindow* window,
	std::string textureLocation, Vector2i textureStart, Vector2i textureSize,
	std::string texturePressedLocation, Vector2i texturePressedStart, Vector2i texturePressedSize,
	MODES mode, std::function<VisibleObject* (void)> modeFunction)
	:position(position),
	size(size),
	window(window),
	textureSize(textureSize), texturePressedSize(texturePressedSize),
	textureStart(textureStart), texturePressedStart(texturePressedStart),
	textureLocation(textureLocation), texturePressedLocation(texturePressedLocation),
	texturePressed(true),
	mode(mode), modeFunction(modeFunction)
{
	setTexture(textureLocation, textureStart, textureSize);
}

void Button::setPosition(Vector2f position)
{
	position = position;
	sprite.setPosition(position);
}

void Button::setSize(Vector2f size)
{
	Button::size = size;
	updateSprite();
}

void Button::draw() {
	window->draw(sprite);
}

bool Button::mouseCheck(View buttonView = view)
{
	Vector2f point = window->mapPixelToCoords(WrapMouse::getClickedCoord(Mouse::Button::Left), buttonView);
	return sprite.getGlobalBounds().contains(point);
}

bool Button::leftClickCheck(View buttonView = view)
{
	Vector2f point = window->mapPixelToCoords(WrapMouse::getClickedCoord(Mouse::Button::Left), buttonView);
	if (!Mouse::isButtonPressed(Mouse::Button::Left)) LeftPressed = false;
	if (!LeftPressed
		&& sprite.getGlobalBounds().contains(point)
		&& Mouse::isButtonPressed(Mouse::Button::Left))
	{
		LeftPressed = true;
		return true;
	}
	else return false;
}

bool Button::rightClickCheck(View buttonView = view)
{
	Vector2f point = window->mapPixelToCoords(WrapMouse::getClickedCoord(Mouse::Button::Right), buttonView);
	if (!Mouse::isButtonPressed(Mouse::Button::Right)) RightPressed = false;
	if (!RightPressed && sprite.getGlobalBounds().contains(point)) {
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
	return  sprite.getPosition();
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
		sprite.setColor(GreyColor);
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
		sprite.setColor(WhiteColor);
	}
}

bool Button::getPressed()
{
	return pressed;
}

std::function<VisibleObject* (void)> Button::getObjectCreationMethod()
{
	return modeFunction;
}

void Menu::updateButtons()
{
	Vector2f menuSize = menuView.getSize();
	//c-buttons.size(), s-size, x-menuSize.x, y-menuSize.y
	//(x/s-1)(y/s-1)>c
	//(c-1)s^2+(x+y)*s-xy=0
	double a = buttons[layer].size() - 1;
	double b = menuSize.x + menuSize.y;
	double c = -menuSize.x * menuSize.y;
	double size = ((-b + sqrt(b * b - 4 * a * c)) / (2 * a)) / (1 + shiftRatio);
	buttonTable.x = (menuSize.x - size * shiftRatio) / (size * (1 + shiftRatio));
	buttonTable.y = (menuSize.y - size * shiftRatio) / (size * (1 + shiftRatio));
	double shiftSize = size * shiftRatio;

	for (int i = 0; i < buttons[layer].size(); i++)
	{
		int row = i / buttonTable.x;
		buttons[layer][i]->setPosition(Vector2f(
			(i - row * buttonTable.x) * (size + shiftSize) + shiftSize,
			row * (size + shiftSize) + shiftSize
		));
		buttons[layer][i]->setSize(Vector2f(size, size));
	}

}

Menu::Menu(RenderWindow* window)
	:window(window)
{
	buttons.resize(2);
	menuView = window->getDefaultView();
	menuView.setSize(menuView.getSize().x * viewport.width, menuView.getSize().y * viewport.height);
	menuView.setCenter(menuView.getCenter().x * viewport.width, menuView.getCenter().y * viewport.height);
	menuView.setViewport(viewport);
	background.setFillColor(color);
}
void Menu::update(Event event)
{
	FloatRect visibleArea(menuView.getCenter().x - menuView.getSize().x / 2, menuView.getCenter().y - menuView.getSize().y / 2, event.size.width * viewport.width, event.size.height * viewport.height);
	window->setView(View(visibleArea));
	menuView = View(visibleArea);
	updateButtons();
}

bool Menu::mouseOnMenu()
{
	Vector2f mousePosition = window->mapPixelToCoords(Mouse::getPosition(*window), menuView);
	return background.getGlobalBounds().contains(mousePosition);
}

void Menu::pushButton(Button* newButton, int layerPB)
{
	window->setView(menuView);
	try
	{
		buttons[layerPB].push_back(newButton);
	}
	catch (std::exception)
	{
		throw std::out_of_range("Layer is not supported");
	}
	updateButtons();
}

bool Menu::checkMouse()
{
	window->setView(menuView);
	for (auto& Button : buttons[layer])
	{
		if (Button->mouseCheck(menuView))
		{
			return true;
		}
	}
	return false;
}

Button* Menu::leftClickCheck()
{
	window->setView(menuView);
	for (auto& Button : buttons[layer])
	{
		if (Button->leftClickCheck(menuView))
		{
			return Button;
		}
	}
	return nullptr;
}

void Menu::unpress()
{
	window->setView(menuView);
	for (auto& Button : buttons[layer])
	{
		Button->unpress();
	}
	Mousemode = MODE_NOTHING;
}

void Menu::switchLayer()
{
	layer = 1 - layer; //0<->1 switcher
	updateButtons();
	unpress();
}

void Menu::draw()
{
	window->setView(menuView);
	menuView.setViewport(viewport);
	background.setSize(menuView.getSize());
	background.setPosition(position);
	window->draw(background);
	for (auto& button : buttons[layer])
	{
		button->draw();
	}
}

DialogBox* Drawer::dialogBox;
std::list<TextBox*> Drawer::allTextBoxes;
DialogBox::DialogBox(RenderWindow* window)
	:window(window)
{
	if (Drawer::dialogBox)
	{
		throw std::runtime_error("Second DialogBox is not supported");
	}
	Drawer::dialogBox = this;
	font.loadFromFile("Textures\\SFML_project\\Fonts\\arial.ttf");
	dialogBox.setFillColor(color);
	shadow.setFillColor(shadowColor);
}

void DialogBox::update(Event event)
{
	FloatRect visibleArea(dialogBoxView.getCenter().x - dialogBoxView.getSize().x / 2, dialogBoxView.getCenter().y - dialogBoxView.getSize().y / 2, event.size.width * viewport.width, event.size.height * viewport.height);
	window->setView(View(visibleArea));
	dialogBoxView = View(visibleArea);
}

void DialogBox::cin(Event event)
{

}

void DialogBox::draw()
{
	window->setView(dialogBoxView);
	Vector2f position = dialogBoxView.getCenter() - sizeDialogBox / 2.f;
	dialogBox.setPosition(position);
	textBox.setPosition(position + textBoxOffset);
	shadow.setPosition(position + shadowOffset);
	window->draw(shadow);
	window->draw(dialogBox);
	window->draw(textBox);
	Text text;
	if (textIn.empty())
	{
		text = Text(formatIn, font, textSize);
		text.setFillColor(color);
	}
	else
	{
		text = Text(textIn, font, textSize);
		text.setFillColor(Color::Black);
	}
	text.setPosition(position + textBoxOffset + textOffset);
	window->draw(text);

}

bool DialogBox::isFinished()
{
	return finished;
}

DialogBox::~DialogBox()
{
	Drawer::dialogBox = nullptr;
}

ScalarBox::ScalarBox(RenderWindow* window) :DialogBox(window)
{
	formatIn = "Input: p:q";
}

double ScalarBox::getDouble()
{
	auto position = textIn.find(":");
	try
	{
		double firstNumber = stod(textIn.substr(0, position));
		double secondNumber = stod(textIn.substr(position + 1));
		return firstNumber / secondNumber;
	}
	catch (std::invalid_argument)
	{
		return 1;
	}
}

void ScalarBox::cin(Event event)
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
		if ((event.text.unicode >= '0' && event.text.unicode <= '9')
			|| event.text.unicode == '.' || event.text.unicode == ',' || event.text.unicode == ':'
			)
		{
			Text text(textIn, font, textSize);
			if (text.getLocalBounds().width < sizeTextBox.x - 2 * textOffset.x)
			{
				textIn += event.text.unicode;
			}
		}
	}
}

void TextBox::setText(std::string text)
{
	TextBox::text = text;
}

void TextBox::draw()
{
}

