#include "gui.h"
#include "Creation.h"
#include "Prover.h"
#include "Printer.h"
#include "Compiler.h"
#include "presetBoxes.h"
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
extern Button clearButton;
extern Button projectionButton;
extern Button parallelButton;
extern Button pointBetweenPoints;

extern Button switchButton;

extern Button switchTriangleButton;
extern Button orthocenterButton;
extern Button barycenterButton;

extern Button switchConstructionButton;
extern Button concurrencyOfLines;
extern Button ñollinearityOfPoints;
extern Button proveParallel;
extern Button proveOrthogonality;
extern Button proveConstructionButton;
extern Button twoLineSegments;
extern Button inscription;

extern Button debugButton;

extern StateMenu stateMenu;
extern presets::isEnabled latex;
extern presets::isEnabled provePrinting;
extern presets::FPS fps;
extern presets::Construction preset;

Vector2f CamCenter = Vector2f(0, 0);

void preInit()
{

	view.setViewport(mainWindowRect);
	view.move(-view.getCenter());
	system("rd /s /q Textures\\TeX");
	system("md Textures\\TeX");
	system("rd /s /q Textures\\Equations");
	system("md Textures\\Equations");
}

void menuInit()
{
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
	//mainMenu.pushButton(&debugButton, 0);
	mainMenu.pushButton(&switchConstructionButton, 0);
	mainMenu.pushButton(&switchTriangleButton, 0);

	mainMenu.pushButton(&concurrencyOfLines, 1);
	mainMenu.pushButton(&ñollinearityOfPoints, 1);
	mainMenu.pushButton(&proveParallel, 1);
	mainMenu.pushButton(&proveOrthogonality, 1);
	mainMenu.pushButton(&twoLineSegments, 1);
	mainMenu.pushButton(&inscription, 1);
	mainMenu.pushButton(&proveConstructionButton, 1);
	mainMenu.pushButton(&moveButton, 2);
	mainMenu.pushButton(&pointButton, 2);
	mainMenu.pushButton(&centralProjectionButton, 2);
	mainMenu.pushButton(&projectionButton, 2);
	mainMenu.pushButton(&rotateLeftButton, 2);
	mainMenu.pushButton(&rotateRightButton, 2);
	mainMenu.pushButton(&scalarButton, 2);
	mainMenu.pushButton(&symmetryButton, 2);
	mainMenu.pushButton(&switchButton, 2);
	mainMenu.pushButton(&switchButton, 1);

	mainMenu.pushButton(&orthocenterButton, 3);
	mainMenu.pushButton(&barycenterButton, 3);
	mainMenu.pushButton(&switchButton, 3);


}

void settings()
{
	mainWindow.setFramerateLimit(60);
	presets::stateboxesInit();
	bool finished = false;
	while (mainWindow.isOpen() && !finished)
	{
		Event event;
		while (mainWindow.pollEvent(event))
		{
			Drawer::update(event);
			Drawer::updateMenu(event);
			if (event.type == Event::MouseButtonPressed || event.type == Event::MouseButtonReleased)
			{
				stateMenu.clickCheck();
			}
			if (event.type == Event::Resized)
			{
				// update the view to the new size of the mainWindow
				FloatRect visibleArea(view.getCenter().x - view.getSize().x / 2, view.getCenter().y - view.getSize().y / 2, event.size.width, event.size.height);
				mainWindow.setView(View(visibleArea));
				view = View(visibleArea);
				stateMenu.update();
			}
			if (event.type == Event::Closed || (event.type == Event::KeyPressed) && (event.key.code == Keyboard::Escape))
			{
				exit(0);
			}
			if ((event.type == Event::KeyPressed) && (event.key.code == Keyboard::Enter))
			{
				finished = true;
			}
		}
		mainWindow.clear(Color::White);
		mainWindow.setView(view);
		stateMenu.draw();
		mainWindow.display();
		std::this_thread::sleep_for(std::chrono::microseconds(1));
	}
	presets::stateboxesApply();
}

void constructPreset()
{
	switch (fps)
	{
	case presets::FPS::FRAMES_60:
		mainWindow.setFramerateLimit(60);
		break;
	case presets::FPS::UNLIMITED:
		mainWindow.setFramerateLimit(0);
		break;
	case presets::FPS::VS:
		mainWindow.setVerticalSyncEnabled(1);
		break;
	}

	NameBox namebox;
	if (preset == presets::Construction::TRIANGLE)
	{
		namebox.setName("C");
		Compiler::points["C"] = new UnitPoint(UnitCircle::getInstance(), Vector2f(unitSeg, unitSeg));
		namebox.setName("B");
		Compiler::points["B"] = new UnitPoint(UnitCircle::getInstance(), Vector2f(0, -unitSeg));
		namebox.setName("A");
		Compiler::points["A"] = new UnitPoint(UnitCircle::getInstance(), Vector2f(-unitSeg, unitSeg));
	}
	else if (preset == presets::Construction::INCENTER)
	{
		namebox.setName("C");
		UnitPoint* pointC = new UnitPoint(UnitCircle::getInstance(), Vector2f(unitSeg, unitSeg));
		expr exprC(make_unit_term("c"));
		dynamic_cast<ConstructionPoint*>(pointC->construction)->coord = exprC * exprC;
		namebox.setName("B");
		UnitPoint* pointB = new UnitPoint(UnitCircle::getInstance(), Vector2f(0, -unitSeg));
		expr exprB(make_unit_term("b"));
		dynamic_cast<ConstructionPoint*>(pointB->construction)->coord = exprB * exprB;
		namebox.setName("A");
		UnitPoint* pointA = new UnitPoint(UnitCircle::getInstance(), Vector2f(-unitSeg, unitSeg));
		expr exprA(make_unit_term("a"));
		dynamic_cast<ConstructionPoint*>(pointA->construction)->coord = exprA * exprA;
		namebox.setName("I");
		Point* pointI = new Point(pointA, pointB, pointC, Point::INCENTER);
		dynamic_cast<ConstructionPoint*>(pointI->construction)->coord = -(exprA * exprB + exprA * exprC + exprB * exprC);
		namebox.setName("Da");
		UnitPoint* pointDa = new UnitPoint(UnitCircle::getInstance(), pointI, pointA);
		dynamic_cast<ConstructionPoint*>(pointDa->construction)->coord = -exprB * exprC;
		namebox.setName("Db");
		UnitPoint* pointDb = new UnitPoint(UnitCircle::getInstance(), pointI, pointB);
		dynamic_cast<ConstructionPoint*>(pointDb->construction)->coord = -exprA * exprC;
		namebox.setName("Dc");
		UnitPoint* pointDc = new UnitPoint(UnitCircle::getInstance(), pointI, pointC);
		dynamic_cast<ConstructionPoint*>(pointDc->construction)->coord = -exprA * exprB;
		pointA->updateTeX();
		pointB->updateTeX();
		pointC->updateTeX();
		pointI->updateTeX();
		pointDa->updateTeX();
		pointDb->updateTeX();
		pointDc->updateTeX();
		Compiler::points["C"] = pointC;
		Compiler::points["B"] = pointB;
		Compiler::points["A"] = pointA;
		Compiler::points["I"] = pointI;
		Compiler::points["Dc"] = pointDc;
		Compiler::points["Db"] = pointDb;
		Compiler::points["Da"] = pointDa;
	}
	else if (preset == presets::Construction::ORTHOCENTER)
	{
		namebox.setName("C");
		UnitPoint* pointC = new UnitPoint(UnitCircle::getInstance(), Vector2f(unitSeg, unitSeg));
		namebox.setName("B");
		UnitPoint* pointB = new UnitPoint(UnitCircle::getInstance(), Vector2f(0, -unitSeg));
		namebox.setName("A");
		UnitPoint* pointA = new UnitPoint(UnitCircle::getInstance(), Vector2f(-unitSeg, unitSeg));
		namebox.setName("H");
		Point* pointH = new Point(pointA, pointB, pointC, Point::ORTHOCENTER);
		Compiler::points["C"] = pointC;
		Compiler::points["B"] = pointB;
		Compiler::points["A"] = pointA;
		Compiler::points["H"] = pointH;
	}
}

void constructingTheDrawing()
{
	while (mainWindow.isOpen() && !Prover::getStarted())
	{
		Event event;
		while (mainWindow.pollEvent(event))
		{
			Drawer::update(event);
			Drawer::updateMenu(event);
			if (event.type == Event::Resized)
			{
				// update the view to the new size of the mainWindow
				FloatRect visibleArea(view.getCenter().x - view.getSize().x / 2, view.getCenter().y - view.getSize().y / 2, event.size.width, event.size.height);
				mainWindow.setView(View(visibleArea));
				view = View(visibleArea);
				view.setViewport(mainWindowRect);
				view.move(-view.getCenter());

			}
			if (event.type == Event::Closed || (event.type == Event::KeyPressed) && (event.key.code == Keyboard::Escape))
			{
				exit(0);
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



			if (event.type == Event::KeyPressed)
			{
				if (event.key.code == Keyboard::Z && event.key.control)
				{
					Memento::previousMemento();
				}
				if (event.key.code == Keyboard::Y && event.key.control)
				{
					Memento::nextMemento();
				}
			}
		}
		mainWindow.clear(Color::White);
		mainWindow.setView(view);
		Drawer::draw();
		Drawer::drawMenu();
		mainWindow.display();
		std::this_thread::sleep_for(std::chrono::microseconds(1));
	}
}

void provingTheProblem()
{
	TextBox* problemState = new TextBox();
	problemState->setText("Solving the problem...");
	mainWindowRect = FloatRect(0.f, 0.f, 1.0f, 1.0f);
	view.setViewport(mainWindowRect);
	moveButton.press();
	Creation::getInstance()->CurrentMethod = moveButton.getObjectCreationMethod();
	mainMenu.setViewport(FloatRect());
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
		}
		mainWindow.clear(Color::White);
		mainWindow.setView(view);
		Prover::updateTextBox();
		Drawer::draw();
		mainWindow.display();
		std::this_thread::sleep_for(std::chrono::microseconds(1));
	}
}

int main(int argc, char** argv)
{
	if (argc == 2)
	{
		mainWindow.setFramerateLimit(60);
		Compiler::compile(argv[1]);
		preInit();
	}
	else
	{
		settings();
		preInit();
		constructPreset();
	}
	menuInit();

	constructingTheDrawing();

	provingTheProblem();

	return 0;
}

