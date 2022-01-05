#include "Button.h"
#include "Creation.h"
using namespace std;
using namespace sf;
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
extern Button tangentButton;
extern Button deleteButton;
extern Button midPointButton;
extern Button hideButton;
extern Button clearButton;

Vector2f CamCenter = Vector2f(0, 0);
Plane* plane = Plane::getInstance();

int main()
{
	CenterPoint::getInstance();


	view.setViewport(sf::FloatRect(0.f, 0.2f, 1.0f, 1.0f));
	window.setPosition(Vector2i(0, 0));
	window.setVerticalSyncEnabled(true);
	Creation::Create();
	//Menu initialization
	mainMenu.pushButton(moveButton);
	mainMenu.pushButton(lineButton);
	mainMenu.pushButton(pointButton);
	mainMenu.pushButton(perpendicularButton);
	mainMenu.pushButton(tangentButton);
	mainMenu.pushButton(deleteButton);
	mainMenu.pushButton(midPointButton);
	mainMenu.pushButton(clearButton);
	mainMenu.pushButton(hideButton);

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
			if (event.type == Event::Closed)
			{
				window.close();
			}
			if (event.type == Event::MouseWheelScrolled)
			{
				if (event.mouseWheelScroll.delta == -1)
				{
					view.zoom(1.2);
				}
				else
				{
					view.zoom(1 / 1.2);
				}
			}


			WrapMouse::checkPress(Mouse::Button::Left);
			WrapMouse::checkPress(Mouse::Button::Right);

			window.setView(view);
			Button* X = mainMenu.leftClickCheck();
			if (X != nullptr)
			{
				if (!X->getPressed())
				{

					mainMenu.unpress();
					X->press();
					Creation::getInstance()->CurrentMethod = X->getObjectCreationMethod();

				}
				else
				{
					X->unpress();
				}
			}
			window.setView(view);
		}
		(*Creation::getInstance())();
		window.clear(Color::White);
		if (UnitCircle::getInstance()->getVisibility() || Creation::getInstance()->getCurrentMode() == MODE_HIDE)
		{
			UnitCircle::getInstance()->draw();
		}
		if (CenterPoint::getInstance()->getVisibility() || Creation::getInstance()->getCurrentMode() == MODE_HIDE)
		{
			CenterPoint::getInstance()->draw();
		}
		if (Creation::getInstance()->getCurrentMode() != MODE_CLEAR)
		{
			if (Creation::getInstance()->getCurrentMode() != MODE_HIDE)
			{
				for (auto object : Drawer::allVisibleObjects)
				{
					if (object->getVisibility())
					{
						object->draw();
					}
				}
			}
			else
			{
				for (auto object : Drawer::allVisibleObjects)
				{
					object->draw();
				}
			}
		}
		window.setView(view);
		mainMenu.draw();
		window.setView(view);
		window.display();
	}
	return 0;
}

