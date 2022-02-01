#include "gui.h"
#include "Creation.h"
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
Vector2f CamCenter = Vector2f(0, 0);
Plane* plane = Plane::getInstance();

int main()
{
	CenterPoint::getInstance();

	view.setViewport(FloatRect(0.f, 0.2f, 1.0f, 1.0f));
	view.move(-500, -500);
	//mainWindow.setVerticalSyncEnabled(true);

	//Menu initialization
	mainMenu.pushButton(&moveButton);
	mainMenu.pushButton(&lineButton);
	mainMenu.pushButton(&pointButton);
	mainMenu.pushButton(&pointBetweenPoints);
	mainMenu.pushButton(&centralProjectionButton);
	mainMenu.pushButton(&perpendicularButton);
	mainMenu.pushButton(&tangentButton);
	mainMenu.pushButton(&projectionButton);
	mainMenu.pushButton(&deleteButton);
	mainMenu.pushButton(&parallelButton);
	mainMenu.pushButton(&scalarButton);
	mainMenu.pushButton(&symmetryButton);
	mainMenu.pushButton(&clearButton);
	mainMenu.pushButton(&fourPointsOnACircle);
	mainMenu.pushButton(&hideButton);
	mainMenu.pushButton(&ProveConstructionButton);


	//(*Creation::getInstance())();
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
		mainMenu.draw();


		mainWindow.display();
	}
	return 0;
}

