#include "Creation.h"

MODES Checker::getLast()
{
	return last;
}

bool Checker::checkMode()
{
	if (last != Mousemode)
	{
		last = Mousemode;
		return false;
	}
	return true;
}

Creation* Creation::creator;

Creation::Creation()
{
}


Creation* Creation::getInstance()
{
	if (creator == nullptr) {
		creator = new Creation();
	}
	return creator;
}

void Creation::operator()()
{
	if (running->joinable())
	{
		running->detach();
	}
}

Checker Creation::checker;
MODES Creation::getCurrentMode()
{
	return checker.getLast();
}

void Creation::Create()
{
	created = true;
}

void Drawer::drawObjects()
{
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
}

void Drawer::drawMenu()
{
	mainMenu.draw();
}

void Drawer::drawTextBoxes()
{
	for (auto textBox : Drawer::allTextBoxes)
	{
		textBox->draw();
	}
}

void Drawer::drawDialogBox()
{
	if (dialogBox)
	{
		dialogBox->draw();
	}
}

void Drawer::updateMenu(Event event)
{
	mainMenu.update(event);
}

void Drawer::updateTextBoxes(Event event)
{

}

void Drawer::updateDialogBox(Event event)
{
	if (dialogBox)
	{
		dialogBox->update(event);
	}
}

void Drawer::update(Event event)
{
	switch (event.type)
	{
	case sf::Event::Resized:
		updateMenu(event);
		updateTextBoxes(event);
		updateDialogBox(event);
		break;
	case sf::Event::TextEntered:
		if (dialogBox)
		{
			dialogBox->cin(event);
		}
	}
}

void Drawer::draw()
{
	drawObjects();
	drawTextBoxes();
	drawDialogBox();
}