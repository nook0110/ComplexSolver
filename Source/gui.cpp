#include "gui.h"




void Button::updateSprite()
{
	sprite_.setScale(1 / sprite_.getScale().x, 1 / sprite_.getScale().y); // Scaling Sprite to 1*1;
	sprite_.setScale(
		size_.x / sprite_.getLocalBounds().width,
		size_.y / sprite_.getLocalBounds().height); // Scaling Sprite to required size;
}

void Button::setTexture(std::string texture_location, Vector2i texture_start, Vector2i texture_size)
{
	texture_.loadFromFile(texture_location, IntRect(texture_start, texture_size));
	texture_.setSmooth(true);
	sprite_.setTexture(texture_);
	updateSprite();
}

Button::Button(Vector2f position, Vector2f size, RenderWindow& window,
	std::string texture_location, Vector2i texture_start, Vector2i texture_size,
	MODES mode, std::function<Object* (void)> mode_function)
	:position_(position),
	size_(size),
	window_(window),
	texture_size_(texture_size),
	texture_start_(texture_start),
	texture_location_(texture_location),
	mode(mode), modeFunction(mode_function)
{
	setTexture(texture_location, texture_start, texture_size);
}

Button::Button(Vector2f position, Vector2f size, RenderWindow& window,
	std::string texture_location, std::string texture_pressed_location,
	MODES mode, std::function<Object* (void)> modeFunction)
	:position_(position),
	size_(size),
	window_(window),
	texture_location_(texture_location),
	texture_pressed_location_(texture_pressed_location),
	texturePressed(true),
	mode(mode), modeFunction(modeFunction)
{
	setTexture(texture_location, texture_start_, texture_size_);
}

Button::Button(Vector2f position, Vector2f size, RenderWindow& window,
	std::string texture_location, Vector2i texture_start, Vector2i texture_size,
	std::string texture_pressed_location, Vector2i texturePressedStart, Vector2i texturePressedSize,
	MODES mode, std::function<Object* (void)> modeFunction)
	:position_(position),
	size_(size),
	window_(window),
	texture_size_(texture_size), texture_pressed_size_(texturePressedSize),
	texture_start_(texture_start), texture_pressed_start_(texturePressedStart),
	texture_location_(texture_location), texture_pressed_location_(texture_pressed_location),
	texturePressed(true),
	mode(mode), modeFunction(modeFunction)
{
	setTexture(texture_location, texture_start, texture_size);
}

Button::Button(RenderWindow& window, std::string texture_location, MODES mode, std::function<Object* (void)> modeFunction)
	:window_(window), texture_location_(texture_location), mode(mode), modeFunction(modeFunction)
{
	setTexture(texture_location, Vector2i(0, 0), Vector2i(0, 0));
	sprite_.setColor(kUnpressedColor);
}

void Button::set_position(Vector2f position)
{
	position = position;
	sprite_.setPosition(position);
}

void Button::set_size(Vector2f size)
{
	Button::size_ = size;
	updateSprite();
}

void Button::Draw() {
	window_.draw(sprite_);
}

bool Button::CheckMouse(View buttonView = view)
{
	Vector2f point = window_.mapPixelToCoords(WrapMouse::getClickedCoord(Mouse::Button::Left), buttonView);
	return sprite_.getGlobalBounds().contains(point);
}

bool Button::leftClickCheck(View buttonView = view)
{
	Vector2f point = window_.mapPixelToCoords(WrapMouse::getClickedCoord(Mouse::Button::Left), buttonView);
	if (!Mouse::isButtonPressed(Mouse::Button::Left)) LeftPressed = false;
	if (!LeftPressed
		&& sprite_.getGlobalBounds().contains(point)
		&& Mouse::isButtonPressed(Mouse::Button::Left))
	{
		LeftPressed = true;
		return true;
	}
	else return false;
}

bool Button::rightClickCheck(View buttonView = view)
{
	Vector2f point = window_.mapPixelToCoords(WrapMouse::getClickedCoord(Mouse::Button::Right), buttonView);
	if (!Mouse::isButtonPressed(Mouse::Button::Right)) RightPressed = false;
	if (!RightPressed && sprite_.getGlobalBounds().contains(point)) {
		RightPressed = true;
		return true;
	}
	else return false;
}

Vector2f Button::getSize()
{
	return size_;
}

Vector2f Button::getLocalPosition()
{
	return  position_;
}

Vector2f Button::getGlobalPosition()
{
	return  sprite_.getPosition();
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
		setTexture(texture_pressed_location_, texture_pressed_start_, texture_pressed_size_);
	}
	else
	{
		sprite_.setColor(kPressedColor);
	}
}

void Button::unpress()
{
	pressed = false;
	if (texturePressed)
	{
		setTexture(texture_location_, texture_start_, texture_size_);
	}
	else
	{
		sprite_.setColor(kUnpressedColor);
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

float Menu::adjustSize(float size, Vector2f menuSize, int count)
{
	const float coeff = 1.001f;
	float nextSize = size * coeff;
	while (floor((menuSize.x - nextSize * shiftRatio) / nextSize) * floor((menuSize.y - nextSize * shiftRatio) / nextSize) > count - 1)
	{
		size = nextSize;
		nextSize *= coeff;
	}
	return size;
}

void Menu::updateButtons()
{
	window->setView(normal_view);
	Vector2f menuSize = normal_view.getSize();
	//c-buttons.size(), s-size, x-menuSize.x, y-menuSize.y
	//(x/s-1)(y/s-1)>c
	//(c-1)s^2+(x+y)*s-xy=0
	float size;
	if (buttons[layer].size() > 1)
	{
		float a = buttons[layer].size() - 1;
		float b = menuSize.x + menuSize.y;
		float c = -menuSize.x * menuSize.y;
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
	float shiftSize = size * shiftRatio;

	for (int i = 0; i < buttons[layer].size(); i++)
	{
		int row = i / buttonTable.x;
		buttons[layer][i]->set_position(Vector2f(
			(i - row * buttonTable.x) * (size + shiftSize) + shiftSize,
			row * (size + shiftSize) + shiftSize
		));
		buttons[layer][i]->set_size(Vector2f(size, size));
	}

}

void Menu::resize(float newSize)
{
	float ratio = newSize / menu_view.getSize().y;
	mainWindowRect.top = ratio * normal_viewport.height;
	view.setViewport(mainWindowRect);
	background.setSize(Vector2f(menu_view.getSize().x, newSize));
	background.setPosition(position);
}

void Menu::setViewport(FloatRect viewport)
{
	Menu::viewport = viewport;
	menu_view.setViewport(viewport);
	menu_view.setCenter(menu_view.getSize() / 2.f + position);
	background.setSize(menu_view.getSize());
	background.setPosition(position);
}

Menu::Menu(RenderWindow* window)
	:window(window)
{
	buttons.resize(3);
	normal_view = window->getDefaultView();
	normal_view.setSize(normal_view.getSize().x * normal_viewport.width, normal_view.getSize().y * normal_viewport.height);
	normal_view.setCenter(normal_view.getCenter().x * normal_viewport.width, normal_view.getCenter().y * normal_viewport.height);
	normal_view.setViewport(normal_viewport);
	menu_view = window->getDefaultView();
	menu_view.setSize(menu_view.getSize().x * viewport.width, menu_view.getSize().y * viewport.height);
	menu_view.setCenter(menu_view.getCenter().x * viewport.width, menu_view.getCenter().y * viewport.height);
	menu_view.setViewport(viewport);
	background.setFillColor(color);
}
void Menu::update(Event event)
{
	FloatRect visibleArea(menu_view.getCenter().x - menu_view.getSize().x / 2, menu_view.getCenter().y - menu_view.getSize().y / 2, event.size.width * viewport.width, event.size.height * viewport.height);
	FloatRect normalVisibleArea = FloatRect(menu_view.getCenter().x - menu_view.getSize().x / 2, menu_view.getCenter().y - menu_view.getSize().y / 2, event.size.width * normal_viewport.width, event.size.height * normal_viewport.height);

	normal_view = View(normalVisibleArea);
	window->setView(View(visibleArea));
	menu_view = View(visibleArea);
	updateButtons();
}

bool Menu::mouseOnMenu()
{
	Vector2f mouse_position = window->mapPixelToCoords(Mouse::getPosition(*window), menu_view);
	return background.getGlobalBounds().contains(mouse_position);
}

void Menu::pushButton(Button* newButton, int layerPB)
{
	window->setView(menu_view);
	if (buttons.size() <= layerPB)
	{
		buttons.resize(layerPB + 1);
	}
	buttons[layerPB].push_back(newButton);
	updateButtons();
}

bool Menu::checkMouse()
{
	window->setView(menu_view);
	for (auto& Button : buttons[layer])
	{
		if (Button->CheckMouse(menu_view))
		{
			return true;
		}
	}
	return false;
}

Button* Menu::leftClickCheck()
{
	window->setView(menu_view);
	for (auto& Button : buttons[layer])
	{
		if (Button->leftClickCheck(menu_view))
		{
			return Button;
		}
	}
	return nullptr;
}

void Menu::unpress()
{
	window->setView(menu_view);
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

void Menu::Draw()
{
	menu_view.setViewport(viewport);
	menu_view.setCenter(menu_view.getSize() / 2.f + position);
	window->setView(menu_view);
	window->draw(background);
	for (auto& button : buttons[layer])
	{
		button->Draw();
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

void DialogBox::Draw()
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

std::pair<int, int> ScalarBox::getRatio()
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

void TextBox::Draw()
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
	background.setOutlineThickness(outline_thickness_);;
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

void Description::Draw()
{
	mainWindow.draw(background);
	mainWindow.draw(sprite);
}
