#pragma once
#include <SFML/Graphics.hpp>
#include "WrapMouse.h"
#include "Creation.h"
#include <iostream>
#include <map>

//File about GUI(graphical user interface): Menu, Buttons, Dialog Boxes, Descriptions

using namespace sf;

extern Event event;
extern FloatRect mainWindowRect;
extern RenderWindow mainWindow;
extern View view;

extern MODES Mousemode;

class Object;
//Button is a clickable object on the screen.
//It has it own mode and std::function.
//Use getObjectCreationMethod() to get funñtion. 
class Button
{
private:

	const Color unpressedColor = Color(255, 255, 255, 255);
	const Color pressedColor = Color(128, 128, 128, 255);

	Vector2f position;
	Vector2f size;
	Vector2i textureSize = Vector2i(0, 0), texturePressedSize = Vector2i(0, 0);
	Vector2i textureStart = Vector2i(0, 0), texturePressedStart = Vector2i(0, 0);
	std::string textureLocation, texturePressedLocation;
	RenderWindow* window;
	Texture texture;
	Sprite sprite;

	//Button mode
	MODES mode = MODE_NOTHING;
	std::function<Object* (void)> modeFunction = []()->Object* {};

	bool pressed = false;
	bool LeftPressed = false;
	bool RightPressed = false;
	bool texturePressed = false;

	void updateSprite();
	void setTexture(std::string textureLocation, Vector2i textureStart, Vector2i textureSize);
public:
	Button(Vector2f position, Vector2f size, RenderWindow* window,
		std::string textureLocation, Vector2i textureStart = Vector2i(0, 0), Vector2i textureSize = Vector2i(0,0),
		MODES mode = MODE_NOTHING, std::function<Object* (void)> modeFunction = []()->Object* {});
	Button(Vector2f position, Vector2f size, RenderWindow* window,
		std::string textureLocation, std::string texturePressedLocation,
		MODES mode = MODE_NOTHING, std::function<Object* (void)> modeFunction = []()->Object* {});
	Button(Vector2f position, Vector2f size, RenderWindow* window,
		std::string textureLocation, Vector2i textureStart, Vector2i textureSize,
		std::string texturePressedLocation, Vector2i texturePressedStart, Vector2i texturePressedSize,
		MODES mode = MODE_NOTHING, std::function<Object* (void)> modeFunction = []()->Object* {});
	Button(RenderWindow* window, std::string textureLocation,
		MODES mode = MODE_NOTHING, std::function<Object* (void)> modeFunction = []()->Object* {});
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
	std::function<Object* (void)> getObjectCreationMethod();
};


//Menu is a list of buttons, you can push buttons in it.
class Menu
{
private:
	View menuView;
	RenderWindow* window;
	const double shiftRatio = 0.1;
	Vector2i buttonTable;
	FloatRect viewport = FloatRect(0.f, 0.f, 1.0f, mainWindowRect.top);
	const FloatRect normalViewport = FloatRect(0.f, 0.f, 1.0f, mainWindowRect.top);
	View normalView;
	const Color color = Color(160, 160, 160, 255);
	const Vector2f position = Vector2f(0, 0);
	int layer = 0;
	std::vector<std::vector<Button*>> buttons;
	RectangleShape background;
	double adjustSize(double size, Vector2f menuSize, int count);
	void updateButtons();

	void resize(double newSize);
public:
	void setViewport(FloatRect viewport);
	Menu(RenderWindow* window);
	// Resize menu
	void update(Event event);
	bool mouseOnMenu();
	void pushButton(Button* newButton, int layerPB);
	bool checkMouse();
	Button* leftClickCheck();
	void unpress();
	void switchLayer(unsigned int layer);
	void draw();
};

class DialogBox
{
protected:
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

	std::mutex deletion;
public:
	DialogBox();
	~DialogBox();
	// Resize DialogBox
	void update(Event event);
	// Cin into the box
	virtual void cin(Event event);
	void draw();
	// Returns if "Enter" was entered
	bool isFinished();

};

// A box to input to numbers: "p:q"
class ScalarBox : public DialogBox
{
public:
	ScalarBox();
	//Returns two numbers p and q
	std::pair<int, int>  getDouble();
	void cin(Event event) override;
};

// A box to name Point
class NameBox : public DialogBox
{
	std::string newName(std::string name);
public:
	static std::map<std::string, bool> names;
	NameBox();
	void cin(Event event) override;
	void setName(std::string name);
	std::string getName();
};


// In construction...
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

// A LaTeX description of an Object
class Description
{
	Sprite sprite;
	Texture texture;
	RectangleShape background;
	Vector2f position = Vector2f();
	Vector2f size;
	const Vector2f backgroundDelta = Vector2f(5, 5);
	const float outlineThikness= 1;
	std::string name;
public:
	Description(std::string filePath, std::string name);
	~Description();
	bool contains(Vector2f point);
	void moveTo(Vector2f position);
	//Return delta of a (Left-Upper) Corner and Position
	Vector2f getDelta(Vector2f position);
	void draw();
};