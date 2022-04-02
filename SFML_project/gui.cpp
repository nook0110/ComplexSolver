#include "gui.h"




void Button::updateSprite()
{
	sprite.setScale(1 / sprite.getScale().x, 1 / sprite.getScale().y); // Scaling Sprite to 1*1;
	sprite.setScale(
		size.x / sprite.getLocalBounds().width,
		size.y / sprite.getLocalBounds().height); // Scaling Sprite to required size;
}

void Button::setTexture(std::string textureLocation, Vector2i textureStart, Vector2i textureSize)
{
	texture.loadFromFile(textureLocation, IntRect(textureStart, textureSize));
	texture.setSmooth(true);
	sprite.setTexture(texture);
	updateSprite();
}

Button::Button(Vector2f position, Vector2f size, RenderWindow* window,
	std::string textureLocation, Vector2i textureStart, Vector2i textureSize,
	MODES mode, std::function<Object* (void)> modeFunction)
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
	MODES mode, std::function<Object* (void)> modeFunction)
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
	MODES mode, std::function<Object* (void)> modeFunction)
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

Button::Button(RenderWindow* window, std::string textureLocation, MODES mode, std::function<Object* (void)> modeFunction)
	:window(window), textureLocation(textureLocation), mode(mode), modeFunction(modeFunction)
{
	setTexture(textureLocation, Vector2i(0, 0), Vector2i(0, 0));
	sprite.setColor(unpressedColor);
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
		sprite.setColor(pressedColor);
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
		sprite.setColor(unpressedColor);
	}
}

bool Button::getPressed()
{
	return pressed;
}

std::function<Object* (void)> Button::getObjectCreationMethod()
{
	return modeFunction;
}

double Menu::adjustSize(double size, Vector2f menuSize, int count)
{
	const double coeff = 1.001;
	double nextSize = size * coeff;
	while (floor((menuSize.x - nextSize * shiftRatio) / nextSize) * floor((menuSize.y - nextSize * shiftRatio) / nextSize) > count - 1)
	{
		size = nextSize;
		nextSize *= coeff;
	}
	return size;
}

void Menu::updateButtons()
{
	window->setView(normalView);
	Vector2f menuSize = normalView.getSize();
	//c-buttons.size(), s-size, x-menuSize.x, y-menuSize.y
	//(x/s-1)(y/s-1)>c
	//(c-1)s^2+(x+y)*s-xy=0
	double size;
	if (buttons[layer].size() > 1)
	{
		double a = buttons[layer].size() - 1;
		double b = menuSize.x + menuSize.y;
		double c = -menuSize.x * menuSize.y;
		size = ((-b + sqrt(b * b - 4 * a * c)) / (2 * a));
	}
	else
	{
		size = std::min(menuSize.x, menuSize.y) / 2;
	}
	size = adjustSize(size, menuSize, buttons[layer].size());

	buttonTable.x = (menuSize.x) / (size);
	buttonTable.y = ceil((float)buttons[layer].size() / (float)buttonTable.x);
	resize(size * buttonTable.y + shiftRatio * size);
	size /= (1 + shiftRatio);
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

void Menu::resize(double newSize)
{
	double ratio = newSize / menuView.getSize().y;
	mainWindowRect.top = ratio * normalViewport.height;
	view.setViewport(mainWindowRect);
	background.setSize(Vector2f(menuView.getSize().x, newSize));
	background.setPosition(position);
}

void Menu::setViewport(FloatRect viewport)
{
	Menu::viewport = viewport;
	menuView.setViewport(viewport);
	menuView.setCenter(menuView.getSize() / 2.f + position);
	background.setSize(menuView.getSize());
	background.setPosition(position);
}

Menu::Menu(RenderWindow* window)
	:window(window)
{
	buttons.resize(3);
	normalView = window->getDefaultView();
	normalView.setSize(normalView.getSize().x * normalViewport.width, normalView.getSize().y * normalViewport.height);
	normalView.setCenter(normalView.getCenter().x * normalViewport.width, normalView.getCenter().y * normalViewport.height);
	normalView.setViewport(normalViewport);
	menuView = window->getDefaultView();
	menuView.setSize(menuView.getSize().x * viewport.width, menuView.getSize().y * viewport.height);
	menuView.setCenter(menuView.getCenter().x * viewport.width, menuView.getCenter().y * viewport.height);
	menuView.setViewport(viewport);
	background.setFillColor(color);
}
void Menu::update(Event event)
{
	FloatRect visibleArea(menuView.getCenter().x - menuView.getSize().x / 2, menuView.getCenter().y - menuView.getSize().y / 2, event.size.width * viewport.width, event.size.height * viewport.height);
	FloatRect normalVisibleArea = FloatRect(menuView.getCenter().x - menuView.getSize().x / 2, menuView.getCenter().y - menuView.getSize().y / 2, event.size.width * normalViewport.width, event.size.height * normalViewport.height);

	normalView = View(normalVisibleArea);
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
	if (buttons.size() <= layerPB)
	{
		buttons.resize(layerPB + 1);
	}
	buttons[layerPB].push_back(newButton);
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

void Menu::switchLayer(unsigned int newLayer)
{
	if (layer == newLayer)
	{
		layer = 0;
	}
	else
	{
		layer = newLayer;
	}
	updateButtons();
	unpress();
}

void Menu::draw()
{
	menuView.setViewport(viewport);
	menuView.setCenter(menuView.getSize() / 2.f + position);
	window->setView(menuView);
	window->draw(background);
	for (auto& button : buttons[layer])
	{
		button->draw();
	}
}

DialogBox* Drawer::dialogBox;
std::list<Description*> Drawer::allDescriptions;
DialogBox::DialogBox()
{
	if (Drawer::dialogBox)
	{
		throw std::runtime_error("Second DialogBox is not supported");
	}
	Drawer::dialogBox = this;
	font.loadFromFile("Textures\\Font\\font.ttf");
	dialogBox.setFillColor(color);
	shadow.setFillColor(shadowColor);
	FloatRect visibleArea(dialogBoxView.getCenter().x - dialogBoxView.getSize().x / 2, dialogBoxView.getCenter().y - dialogBoxView.getSize().y / 2, mainWindow.getSize().x * viewport.width, mainWindow.getSize().y * viewport.height);
	dialogBoxView = View(visibleArea);
}

void DialogBox::update(Event event)
{
	FloatRect visibleArea(dialogBoxView.getCenter().x - dialogBoxView.getSize().x / 2, dialogBoxView.getCenter().y - dialogBoxView.getSize().y / 2, event.size.width * viewport.width, event.size.height * viewport.height);
	mainWindow.setView(dialogBoxView = View(visibleArea));
}

void DialogBox::setSize(Vector2f size)
{
	sizeDialogBox = size;
	sizeTextBox = size - Vector2f(5, 5);
	dialogBox.setSize(sizeDialogBox);
	shadow = dialogBox;
	textBox.setSize(sizeTextBox);
}

void DialogBox::cin(Event event)
{

}

void DialogBox::draw()
{
	deletion.lock();
	mainWindow.setView(dialogBoxView);
	Vector2f position = dialogBoxView.getCenter() - sizeDialogBox / 2.f;
	dialogBox.setPosition(position);
	textBox.setPosition(position + textBoxOffset);
	shadow.setPosition(position + shadowOffset);
	mainWindow.draw(shadow);
	mainWindow.draw(dialogBox);
	mainWindow.draw(textBox);
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
	mainWindow.draw(text);
	deletion.unlock();
}

bool DialogBox::isFinished()
{
	return finished;
}

DialogBox::~DialogBox()
{
	deletion.lock();
	Drawer::dialogBox = nullptr;
	deletion.unlock();
}

ScalarBox::ScalarBox() :DialogBox()
{
	formatIn = "Input: p:q";
}

std::pair<int, int> ScalarBox::getDouble()
{
	auto position = textIn.find(":");
	try
	{
		int firstNumber = stoi(textIn.substr(0, position));
		int secondNumber = stoi(textIn.substr(position + 1));
		return { firstNumber,secondNumber };
	}
	catch (std::invalid_argument)
	{
		return { 1,1 };
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
		if ((event.text.unicode >= '0' && event.text.unicode <= '9' || event.text.unicode == '-')
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

std::map<std::string, bool> NameBox::names;

std::string NameBox::newName(std::string name)
{
	auto lowCaseName = name;
	std::transform(lowCaseName.begin(), lowCaseName.end(), lowCaseName.begin(),
		[](unsigned char c) { return std::tolower(c); });
	if (names[lowCaseName])
	{
		return newName(name + "1");
	}
	else
	{
		names[lowCaseName] = 1;
		return name;
	}
}

NameBox::NameBox() : DialogBox()
{
	formatIn = "Enter name...";
}

NameBox::NameBox(std::string name) : DialogBox()
{
	formatIn = "Enter name...";
	textIn = name;
	finished = true;
}

void NameBox::cin(Event event)
{
	switch (event.text.unicode)
	{
	case 8:
		if (textIn.empty())
		{
			break;
		}
		textIn.pop_back();
		break;
	case 13:
		if (textIn.empty())
		{
			break;
		}
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

void NameBox::setName(std::string name)
{
	textIn = name;
	finished = true;
}

std::string NameBox::getName()
{
	return newName(textIn);
}


TextBox::TextBox() : DialogBox()
{
	setSize(Vector2f(400, 60));
}

void TextBox::setText(std::string text)
{
	textIn = text;
	Text txt = Text(textIn, font, textSize);
	setSize(Vector2f(txt.getGlobalBounds().width, txt.getGlobalBounds().height) + (textBoxOffset + textOffset) * 3.f);
}

void TextBox::setColor(Color color)
{
	DialogBox::color = color;
}

void TextBox::draw()
{
	deletion.lock();
	mainWindow.setView(dialogBoxView);
	Vector2f position = dialogBoxView.getCenter() + dialogBoxView.getSize() / 2.f - sizeDialogBox + cornerOffset;
	dialogBox.setPosition(position);
	textBox.setPosition(position + textBoxOffset);
	shadow.setPosition(position + shadowOffset);
	mainWindow.draw(shadow);
	mainWindow.draw(dialogBox);
	mainWindow.draw(textBox);
	Text text;

	text = Text(textIn, font, textSize);
	text.setFillColor(color);

	text.setPosition(position + textBoxOffset + textOffset);
	mainWindow.draw(text);
	deletion.unlock();
}

Description::Description(std::string filePath, std::string name)
	:name(name)
{
	texture.loadFromFile(filePath);
	texture.setSmooth(true);
	sprite.setTexture(texture);
	Vector2f size = Vector2f(texture.getSize()) / 3.f;
	background.setOrigin(backgroundDelta);
	background.setSize(size + backgroundDelta * 2.f);
	background.setFillColor(Color::White);
	background.setOutlineColor(Color::Black);
	background.setOutlineThickness(outlineThikness);;
	sprite.setScale(1 / sprite.getScale().x, 1 / sprite.getScale().y); // Scaling Sprite to 1*1;
	sprite.setScale(
		size.x / sprite.getLocalBounds().width,
		size.y / sprite.getLocalBounds().height);
	Drawer::allDescriptions.push_front(this);
}

Description::~Description()
{
	Drawer::allDescriptions.remove(this);
}

bool Description::contains(Vector2f point)
{
	return background.getGlobalBounds().contains(point);
}

void Description::moveTo(Vector2f position)
{
	Description::position = position;
	sprite.setPosition(position);
	background.setPosition(position);
}

Vector2f Description::getDelta(Vector2f position)
{
	return position - background.getPosition();
}

void Description::draw()
{
	mainWindow.draw(background);
	mainWindow.draw(sprite);
}
