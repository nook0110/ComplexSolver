#pragma once
#include <SFML/Graphics.hpp>
#include "WrapMouse.h"
#include "Creation.h"
#include<iostream>
using namespace sf;


extern Vector2i maxTextureResolution;
extern Event event;
extern FloatRect mainWindowRect;


class VisibleObject;
//Button is a clickable object on the screen.
//It has it own mode and std::function.
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
	std::string textureLocation, texturePressedLocation;
	RenderWindow* window;
	Texture texture;
	Sprite sprite;

	//Button mode
	MODES mode = MODE_NOTHING;
	std::function<VisibleObject* (void)> modeFunction = []()->VisibleObject* {};

	bool pressed = false;
	bool LeftPressed = false;
	bool RightPressed = false;
	bool texturePressed = false;

	void updateSprite();
	void setTexture(std::string textureLocation, Vector2i textureStart, Vector2i textureSize);
public:
	Button(Vector2f position, Vector2f size, RenderWindow* window,
		std::string textureLocation, Vector2i textureStart = Vector2i(0, 0), Vector2i textureSize = maxTextureResolution,
		MODES mode = MODE_NOTHING, std::function<VisibleObject* (void)> modeFunction = []()->VisibleObject* {});
	Button(Vector2f position, Vector2f size, RenderWindow* window,
		std::string textureLocation, std::string texturePressedLocation,
		MODES mode = MODE_NOTHING, std::function<VisibleObject* (void)> modeFunction = []()->VisibleObject* {});
	Button(Vector2f position, Vector2f size, RenderWindow* window,
		std::string textureLocation, Vector2i textureStart, Vector2i textureSize,
		std::string texturePressedLocation, Vector2i texturePressedStart, Vector2i texturePressedSize,
		MODES mode = MODE_NOTHING, std::function<VisibleObject* (void)> modeFunction = []()->VisibleObject* {});
	void setPosition(Vector2f);
	void setSize(Vector2f size);
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
	std::function<VisibleObject*(void)> getObjectCreationMethod();
};


//Menu is a list of buttons, you can push buttons in it.
class Menu
{
private:
	View menuView;
	RenderWindow* window;
	const double shiftRatio = 0.1;
	Vector2i buttonTable;
	const FloatRect viewport = FloatRect(0.f, 0.f, 1.0f, mainWindowRect.top);
	const Color color = Color(128, 128, 128, 255);
	const Vector2f position = Vector2f(0, 0);
	int layer = 0;
	std::vector<std::vector<Button*>> buttons;
	RectangleShape background;
	void updateButtons();
public:
	Menu(RenderWindow* window);
	void update(Event);
	bool mouseOnMenu();
	void pushButton(Button* newButton, int layerPB);
	bool checkMouse();
	Button* leftClickCheck();
	void unpress();
	void switchLayer();
	void draw();
};

class DialogBox
{
protected:
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
	std::string textIn;
	std::string formatIn;
	bool finished = false;
public:
	DialogBox(RenderWindow* window);
	~DialogBox();
	void update(Event event);
	virtual void cin(Event event);
	void draw();
	bool isFinished();

};

class ScalarBox : public DialogBox
{
public:
	ScalarBox(RenderWindow* window);
	double getDouble();
	void cin(Event event) override;
};

class NameBox :DialogBox
{

};
class TextBox
{
	RenderWindow* window;
	View textBoxView;
	const Vector2f sizeTextBox = Vector2f(30, 300);
	RectangleShape textBox = RectangleShape(sizeTextBox);
	Vector2f position;
	std::string text;
public:
	void setText(std::string text);
	void draw();
};