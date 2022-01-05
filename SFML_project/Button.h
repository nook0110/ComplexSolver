#pragma once
#include <SFML/Graphics.hpp>
#include "gui.h"
#include "WrapMouse.h"
using namespace std;
using namespace sf;

extern Vector2i maxTextureResolution;

//Button is a clickable object on the screen.
//It has it own mode and function.
//Use getObjectCreationMethod() to get funсtion. 
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
	function<void(std::stop_token)> modeFunction = [](std::stop_token) {};

	bool pressed = false;
	bool LeftPressed = false;
	bool RightPressed = false;
	bool texturePressed = false;

	void setTexture(string _textureLocation, Vector2i _textureStart, Vector2i _textureSize);
public:
	Button(Vector2f _position, Vector2f _size, RenderWindow* _window,
		string _textureLocation, Vector2i _textureStart = Vector2i(0, 0), Vector2i _textureSize = maxTextureResolution,
		MODES _mode = MODE_NOTHING, function<void(std::stop_token)> _modeFunction = [](std::stop_token) {});
	Button(Vector2f _position, Vector2f _size, RenderWindow* _window,
		string _textureLocation, string _texturePressedLocation,
		MODES _mode = MODE_NOTHING, function<void(std::stop_token)> _modeFunction = [](std::stop_token) {});
	Button(Vector2f _position, Vector2f _size, RenderWindow* _window,
		string _textureLocation, Vector2i _textureStart, Vector2i _textureSize,
		string _texturePressedLocation, Vector2i _texturePressedStart, Vector2i _texturePressedSize,
		MODES _mode = MODE_NOTHING, function<void(std::stop_token)> _modeFunction = [](std::stop_token) {});
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
	function<void(std::stop_token)> getObjectCreationMethod();
};

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
