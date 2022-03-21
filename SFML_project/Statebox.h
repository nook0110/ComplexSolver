#pragma once
#include <SFML/Graphics.hpp>
#include "WrapMouse.h"
#include <vector>

#include <iostream>
using namespace sf;
extern RenderWindow mainWindow;
extern View view;



class Statebox
{
private:
	const float ratio = 0.2;
	Vector2f position;
	Vector2f size;
	std::string name;
	Color backgroundColor = Color(200, 200, 200);
	Color textColor = Color::Black;
	unsigned int textSize;
	Font font;
	Text text;
	RectangleShape background;
	std::vector<Texture> statesTextures;
	std::vector<Sprite> statesSprites;
	std::vector<Text> statesTexts;
	int state;
	int maxState;
	void update();
public:
	Statebox(Vector2f position, Vector2f size, std::string name, int states);
	void setStateTexture(int state, std::string textState, std::string texturePath);
	void setPosition(Vector2f postion);
	void setSize(float sizeY);
	bool mouseCheck(View);
	bool clickCheck(View);
	void draw();
	int getState();
};

class StateMenu
{
private:
	std::vector<Statebox*> stateboxes;
	void updateBoxes();
	float shiftRatio = 0.1;
public:
	void update();
	StateMenu();
	void pushStatebox(Statebox* newStatebox);
	Statebox* clickCheck();
	void draw();
};

