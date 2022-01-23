#pragma once
#include <SFML/Graphics.hpp>
#include "WrapMouse.h"
#include "Creation.h"
#include<iostream>
using namespace std;
using namespace sf;

extern Vector2i maxTextureResolution;
extern Event event;
//Button is a clickable object on the screen.
//It has it own mode and function.
//Use getObjectCreationMethod() to get funñtion. 
class Button
{
private:

	Color WhiteColor = Color(255, 255, 255, 255);
	Color GreyColor = Color(128, 128, 128, 255);

	Vector2f position;
	Vector2f size;
	Vector2i textureSize = maxTextureResolution, texturePressedSize = maxTextureResolution;
	Vector2i textureStart = Vector2i(0, 0), texturePressedStart = Vector2i(0, 0);
	string textureLocation, texturePressedLocation;
	RenderWindow* window;
	Texture texture;
	Sprite Sprite;

	//Button mode
	MODES mode = MODE_NOTHING;
	function<void(void)> modeFunction = [&](void) {};

	bool pressed = false;
	bool LeftPressed = false;
	bool RightPressed = false;
	bool texturePressed = false;

	void setTexture(string _textureLocation, Vector2i _textureStart, Vector2i _textureSize);
public:
	Button(Vector2f _position, Vector2f _size, RenderWindow* _window,
		string _textureLocation, Vector2i _textureStart = Vector2i(0, 0), Vector2i _textureSize = maxTextureResolution,
		MODES _mode = MODE_NOTHING, function<void(void)> _modeFunction = [&](void) {});
	Button(Vector2f _position, Vector2f _size, RenderWindow* _window,
		string _textureLocation, string _texturePressedLocation,
		MODES _mode = MODE_NOTHING, function<void(void)> _modeFunction = [&](void) {});
	Button(Vector2f _position, Vector2f _size, RenderWindow* _window,
		string _textureLocation, Vector2i _textureStart, Vector2i _textureSize,
		string _texturePressedLocation, Vector2i _texturePressedStart, Vector2i _texturePressedSize,
		MODES _mode = MODE_NOTHING, function<void(void)> _modeFunction = [&](void) {});
	void setPosition(Vector2f);
	void draw();
	bool mouseCheck(View);
	bool leftClickCheck(View);
	bool rightClickCheck(View);
	Vector2f getSize();
	Vector2f getLocalPosition();
	Vector2f getGlobalPosition();
private:
	void setMode();
public:
	void press();
	void unpress();
	bool getPressed();
	function<void(void)> getObjectCreationMethod();
};

class DialogBox;
class TextBox;
//Menu is a list of buttons, you can push buttons in it.
class Menu
{
private:
	View menuView;
	RenderWindow* window;
	const FloatRect viewport = FloatRect(0.f, 0.f, 1.0f, 0.2f);
	const Color color = Color(128, 128, 128, 255);
	const Vector2f position = Vector2f(0, 0);
	vector<Button> buttons;
	RectangleShape background;
public:
	Menu(RenderWindow* _window);
	void update(Event);
	bool mouseOnMenu();
	void pushButton(Button newButton);
	bool checkMouse();
	Button* leftClickCheck();
	void unpress();
	void draw();
};

class DialogBox
{
	RenderWindow* window;
	View dialogBoxView;
	Color color = Color(128, 128, 128);
	Color shadowColor = Color(32, 32, 32);
	const FloatRect viewport = FloatRect(0.f, 0.f, 1.0f, 1.0f);
	const Vector2f sizeDialogBox = Vector2f(300, 60);
	const Vector2f sizeTextBox = Vector2f(295, 55);
	const unsigned int textSize = 35;
	const Vector2f textOffset = Vector2f((sizeTextBox.y - textSize) / 2, (sizeTextBox.y - textSize) / 2);
	const Vector2f textBoxOffset = (sizeDialogBox - sizeTextBox) / 2.f;
	const Vector2f shadowOffset = Vector2f(1.f, 1.f);
	RectangleShape dialogBox = RectangleShape(sizeDialogBox);
	RectangleShape shadow = dialogBox;
	RectangleShape textBox = RectangleShape(sizeTextBox);
	Font font;
	string textIn;
	const string formatIn = "Input: p:q";
	bool finished = false;
public:
	DialogBox(RenderWindow* window);
	~DialogBox();
	void update(Event event);
	void cin(Event event);
	void draw();
	bool isFinished();
	double getDouble();
};

class TextBox
{
	RenderWindow* window;
	View textBoxView;
	const Vector2f sizeTextBox = Vector2f(30, 300);
	RectangleShape textBox = RectangleShape(sizeTextBox);
	Vector2f position;
	string text;
public:
	void setText(string text);
	void draw();
};