#include "gui.h"
#include "Creation.h"

#include <iostream>
#include <iomanip>
using namespace sf;

extern RenderWindow mainWindow;
extern View view;
extern double const epsilon;

extern Vector2i maxTextureResolution;
extern FloatRect mainWindowRect;
extern MODES Mousemode;

extern Button moveButton;
extern Button pointButton;
extern Button lineButton;
extern Button centralProjectionButton;
extern Button perpendicularButton;
extern Button tangentButton;
extern Button rotateLeftButton;
extern Button rotateRightButton;
extern Button deleteButton;
extern Button midPointButton;
extern Button symmetryButton;
extern Button scalarButton;
extern Button hideButton;
extern Button fourPointsOnACircle;
extern Button clearButton;
extern Button projectionButton;
extern Button parallelButton;
extern Button pointBetweenPoints;
extern Button ProveConstructionButton;
extern Button switchButton;

Vector2f CamCenter = Vector2f(0, 0);

int main()
{
	view.setViewport(mainWindowRect);
	view.move(-500, -500);
	mainWindow.setFramerateLimit(60);

	//Menu initialization
	mainMenu.pushButton(&moveButton, 0);
	mainMenu.pushButton(&lineButton, 0);
	mainMenu.pushButton(&pointButton, 0);
	mainMenu.pushButton(&pointBetweenPoints, 0);
	mainMenu.pushButton(&centralProjectionButton, 0);
	mainMenu.pushButton(&perpendicularButton, 0);
	mainMenu.pushButton(&tangentButton, 0);
	mainMenu.pushButton(&projectionButton, 0);
	mainMenu.pushButton(&rotateLeftButton, 0);
	mainMenu.pushButton(&rotateRightButton, 0);
	mainMenu.pushButton(&deleteButton, 0);
	mainMenu.pushButton(&parallelButton, 0);
	mainMenu.pushButton(&scalarButton, 0);
	mainMenu.pushButton(&symmetryButton, 0);
	mainMenu.pushButton(&clearButton, 0);
	mainMenu.pushButton(&hideButton, 0);
	mainMenu.pushButton(&switchButton, 0);

	mainMenu.pushButton(&fourPointsOnACircle, 1);
	mainMenu.pushButton(&ProveConstructionButton, 1);
	mainMenu.pushButton(&switchButton, 1);

	while (mainWindow.isOpen())
	{
		Event event;
		while (mainWindow.pollEvent(event))
		{
			Drawer::update(event);
			if (event.type == Event::Resized)
			{
				// update the view to the new size of the mainWindow
				FloatRect visibleArea(view.getCenter().x - view.getSize().x / 2, view.getCenter().y - view.getSize().y / 2, event.size.width, event.size.height);
				mainWindow.setView(View(visibleArea));
				view = View(visibleArea);
				view.setViewport(mainWindowRect);
			}
			if (event.type == Event::Closed || (event.type == Event::KeyPressed) && (event.key.code == Keyboard::Escape))
			{
				mainWindow.close();
			}
			if (event.type == Event::MouseWheelScrolled)
			{
				if (event.mouseWheelScroll.delta < 0)
				{
					view.zoom(1.2);
				}
				else
				{
					view.zoom(1 / 1.2);
				}
			}
			mainWindow.setView(view);
		}
		mainWindow.clear(Color::White);
		mainWindow.setView(view);
		Drawer::draw();


		mainWindow.display();
	}
	return 0;
}

