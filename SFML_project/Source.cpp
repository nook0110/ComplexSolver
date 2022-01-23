#include "gui.h"
#include "Creation.h"
using namespace sf;
bool Creation::created;
extern RenderWindow  window;
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
extern Button scalarButton;
extern Button hideButton;
extern Button clearButton;

Vector2f CamCenter = Vector2f(0, 0);
Plane* plane = Plane::getInstance();

int main()
{

	CenterPoint::getInstance();

	view.setViewport(sf::FloatRect(0.f, 0.2f, 1.0f, 1.0f));
	view.move(-500, -500);
	window.setVerticalSyncEnabled(true);

	//Menu initialization
	mainMenu.pushButton(moveButton);
	mainMenu.pushButton(lineButton);
	mainMenu.pushButton(pointButton);
	mainMenu.pushButton(centralProjectionButton);
	//mainMenu.pushButton(perpendicularButton);
	mainMenu.pushButton(tangentButton);
	mainMenu.pushButton(deleteButton);
	mainMenu.pushButton(scalarButton);
	mainMenu.pushButton(clearButton);
	mainMenu.pushButton(hideButton);



	
	(*Creation::getInstance())();
	while (window.isOpen())
	{	
		Event event;
		while (window.pollEvent(event))
		{
			Drawer::update(event);
			if (event.type == sf::Event::Resized)
			{
				// update the view to the new size of the window
				sf::FloatRect visibleArea(view.getCenter().x - view.getSize().x / 2, view.getCenter().y - view.getSize().y / 2, event.size.width, event.size.height);
				window.setView(sf::View(visibleArea));
				view = sf::View(visibleArea);
				view.setViewport(mainWindowRect);
			}
			if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
			{
				window.close();
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
			window.setView(view);
		}
		window.clear(Color::White);
		window.setView(view);
		Drawer::draw();
		mainMenu.draw();
		window.display();
	}
	return 0;
}

