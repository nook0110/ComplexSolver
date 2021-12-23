#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <thread>
#include <functional>
#include <future>
#include "Button.h"
#include "Creation.h"
#include <ctime>
using namespace std;
using namespace sf;
Creation Cr;
bool Creation::created;
extern RenderWindow  window;
extern View view;
extern double const epsilon;

extern Vector2i maxTextureResolution;
extern FloatRect mainWindowRect;
extern Menu mainMenu;

extern MODES Mousemode;

extern Button moveButton;
extern Button pointButton;
extern Button lineButton;
extern Button perpendicularButton;

Vector2f CamCenter = Vector2f(0, 0);
Plane* plane = Plane::getInstance();
UnitCircle* unitCircle = UnitCircle::getInstance();


int main()
{

	view.setViewport(sf::FloatRect(0.f, 0.2f, 1.0f, 1.0f));
	window.setPosition(Vector2i(0, 0));

	

	window.setVerticalSyncEnabled(true);
	Creation::Create();
	//Menu initialization
	mainMenu.pushButton(moveButton);
	mainMenu.pushButton(lineButton);
	mainMenu.pushButton(pointButton);
	mainMenu.pushButton(perpendicularButton);

	//mainMenu.pushButton(midPointButton);*/
	view.move(-500, -500);

	while (window.isOpen())
	{
		window.setView(view);
		Event event;
		while (window.pollEvent(event))
		{

			if (event.type == sf::Event::Resized)
			{
				mainMenu.update(event);
				// update the view to the new size of the window
				sf::FloatRect visibleArea(view.getCenter().x - view.getSize().x / 2, view.getCenter().y - view.getSize().y / 2, event.size.width, event.size.height);
				window.setView(sf::View(visibleArea));
				view = sf::View(visibleArea);
				view.setViewport(mainWindowRect);
			}

			WrapMouse::checkPress(Mouse::Button::Left);
			WrapMouse::checkPress(Mouse::Button::Right);
			if (event.type == Event::Closed)
			{
				window.close();
			}
			
			window.setView(view);
			Button* X = mainMenu.leftClickCheck();
			if (X != nullptr)
			{
				if (!X->getPressed())
				{

					mainMenu.unpress();
					X->press();
					Cr.CurrentMethod = X->getObjectCreationMethod();

				}
				else
				{
					X->unpress();
				}
			}
			window.setView(view);
			


		}

		Cr();
		window.clear(Color::White);



		//plane->draw();
		unitCircle->draw();



		for (auto X : ConstructionData::allVisibleObjects)
		{
			X->draw();
		}
		
		window.setView(view);
		mainMenu.draw();
		window.setView(view);

		window.display();
	}

	return 0;
}

