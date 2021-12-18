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

extern Menu mainMenu;

extern MODES Mousemode;

/*Button moveButton = Button(Vector2f(10, 10), Vector2f(10, 10), &window,
	"C:\\Textures\\SFML_project\\Test.jpg", Vector2i(0, 0), maxTextureResolution,
	MODE_NOTHING, []()->VisibleObject* {
		return nullptr;
	});
Button pointButton = Button(Vector2f(120, 10), Vector2f(10, 100), &window,
	"C:\\Textures\\SFML_project\\Test.jpg", Vector2i(0, 0), maxTextureResolution,
	MODE_POINT, []()->VisibleObject* {
		while (!Mouse::isButtonPressed(Mouse::Button::Left))
		{

		}
		Vector2f Offset = getOffset(view);
		Vector2f MousePosition;
		MousePosition.x = Mouse::getPosition().x + Offset.x;
		MousePosition.y = Mouse::getPosition().y + Offset.y;
		for (Point* point : allPoints)
		{
			if (point->isNearby(MousePosition))
			{
				return point;
			}
		}
		vector<Line*> lines;
		for (Line* line : allLines)
		{
			if (line->isNearby(MousePosition))
			{
				lines.push_back(line);
			}
			if (lines.size() == 2)
			{
				break;
			}
		}
		if (lines.size() == 1)
		{
			return new Point(lines[0], MousePosition);
		}
		if (lines.size() == 2)
		{
			return new Point(lines[0], lines[1]);
		}
		//Unit circle check
		return new Point(MousePosition);
	});
Button lineButton = Button(Vector2f(230, 10), Vector2f(100, 10), &window,
	"C:\\Textures\\SFML_project\\Test.jpg", Vector2i(0, 0), maxTextureResolution,
	MODE_NOTHING, []()->VisibleObject* {
		while (!Mouse::isButtonPressed(Mouse::Button::Left))
		{

		}
		Vector2f Offset = getOffset(view);
		Vector2f MousePosition;
		MousePosition.x = Mouse::getPosition().x + Offset.x;
		MousePosition.y = Mouse::getPosition().y + Offset.y;
		vector<Point*> points;
		for (Point* point :  allPoints)
		{
			if (point->isNearby(MousePosition))
			{
				points.push_back(point);
				points.resize(2);
			}
		}
		vector<Line*> lines;
		for (Line* line : allLines)
		{
			if (line->isNearby(MousePosition) && lines.size() < 2)
			{
				lines.push_back(line);
				lines.resize(2);
			}
		}
		if (lines.size() == 1)
		{
			points.push_back(new Point(lines[0], MousePosition));
		}
		if (lines.size() == 2)
		{
			points.push_back(new Point(lines[0], lines[1]));
		}
		//Unit circle check
		return new Point(MousePosition);
	});
Button perpendicularButton = Button(Vector2f(340, 10), Vector2f(239, 100), &window,
	"C:\\Textures\\SFML_project\\Test.jpg", Vector2i(0, 0), maxTextureResolution,
	MODE_NOTHING, []()->VisibleObject* {
		return nullptr;
	});
Button midPointButton = Button(Vector2f(450, 10), Vector2f(100, 239), &window,
	"C:\\Textures\\SFML_project\\Test.jpg", Vector2i(0, 0), maxTextureResolution,
	MODE_NOTHING, []()->VisibleObject* {
		return nullptr;
	});

*/
extern Button moveButton;
extern Button pointButton;
extern Button lineButton;
Vector2f CamCenter = Vector2f(0, 0);
Plane* plane = Plane::getInstance();
UnitCircle* unitCircle = UnitCircle::getInstance();
void test()
{
	while (true)
	{

	}
}

int main()
{

	view.setViewport(sf::FloatRect(0.f, 0.2f, 1.0f, 1.0f));
	window.setPosition(Vector2i(0, 0));

	Button TestButton = Button(Vector2f(300.f, 10.f), Vector2f(100.f, 200.f), &window, "C:\\Textures\\SFML_project\\Test.jpg");
	Point TestPoint1 = Point(Vector2f(100.f, 0.f));
	Point TestPoint2 = Point(Vector2f(600.f, 250.f));
	//Line TestLine = Line(&TestPoint1, &TestPoint2);


	window.setVerticalSyncEnabled(true);
	Creation::Create();
	//Menu initialization
	mainMenu.pushButton(moveButton);
	mainMenu.pushButton(lineButton);
	mainMenu.pushButton(pointButton);

	//mainMenu.pushButton(perpendicularButton);
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
				view.setViewport(sf::FloatRect(0.f, 0.2f, 1.0f, 1.0f));
			}

			WrapMouse::checkPress(Mouse::Button::Left);
			WrapMouse::checkPress(Mouse::Button::Right);
			if (event.type == Event::Closed)
			{
				window.close();
			}
			if ((TestButton).leftClickCheck(view))
			{

				if (!TestButton.getPressed())
				{

					TestButton.press();
					view.move(Vector2f(10, -10));

				}
				else
				{
					TestButton.unpress();
				}
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
			if ((TestButton).rightClickCheck(view))
			{

				//exit(0);
			}


		}

		Cr();
		window.clear(Color::White);



		//plane->draw();
		unitCircle->draw();
		(TestPoint1).draw();
		(TestPoint2).draw();
		//(TestLine).draw();



		for (auto X : ConstructionData::allVisibleObjects)
		{
			X->draw();
		}
		(TestButton).draw();
		window.setView(view);
		mainMenu.draw();
		window.setView(view);

		window.display();
	}

	return 0;
}

