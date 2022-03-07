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

Creation::~Creation()
{
	delete running;
}


Creation* Creation::getInstance()
{
	if (creator == nullptr) {
		creator = new Creation();
	}
	return creator;
}

Checker Creation::checker;
MODES Creation::getCurrentMode()
{
	return checker.getLast();
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
	if (Creation::getInstance()->getCurrentMode() == MODE_CLEAR)
	{
		return;
	}
	if (Creation::getInstance()->getCurrentMode() != MODE_HIDE)
	{
		for (auto object : Drawer::allVisibleObjects)
		{
			if (object->getVisibility())
			{
				if (Creation::getInstance()->getCurrentMode() == MODE_CLEAR)
				{
					return;
				}
				object->draw();
			}
		}
	}
	else
	{
		for (auto object : Drawer::allVisibleObjects)
		{
			if (Creation::getInstance()->getCurrentMode() == MODE_CLEAR)
			{
				return;
			}
			object->draw();
		}
	}
}

void Drawer::drawMenu()
{
	mainMenu.draw();
}

void Drawer::drawDescriptions()
{
	for (auto description : Drawer::allDescriptions)
	{
		description->draw();
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
	switch (event.type)
	{
	case Event::Resized:
		resizeMenu(event);
		break;
	case Event::MouseButtonPressed:
	case Event::MouseButtonReleased:
		updateMenu();
	}
}

void Drawer::resizeMenu(Event event)
{
	mainMenu.update(event);
}

void Drawer::updateMenu()
{
	Button* button = mainMenu.leftClickCheck();
	if (button)
	{
		if (!button->getPressed())
		{
			mainMenu.unpress();
			button->press();
			Creation::getInstance()->CurrentMethod = button->getObjectCreationMethod();
		}
		else
		{
			mainMenu.unpress();
			button->unpress();
		}
	}
}

void Drawer::resizeDialogBox(Event event)
{
	if (dialogBox)
	{
		dialogBox->update(event);
	}
}

bool Drawer::VisibleObjectsContains(Object* ptr)
{
	for (auto obj : allVisibleObjects)
	{
		if (obj == ptr)
		{
			return true;
		}
	}
	return false;
}

void Drawer::delObject(Object* object)
{
	new Memento(object, CHANGES::DELETION);
	object->del();
}

void Drawer::addObject(Object* object)
{
	new Memento(object, CHANGES::ADDITION);
	object->add();
}

void Drawer::update(Event event)
{

	switch (event.type)
	{
	case Event::Resized:
		resizeDialogBox(event);
		break;
	case Event::TextEntered:
		if (dialogBox)
		{
			dialogBox->cin(event);
		}
		break;
	}
}

void Drawer::draw()
{
	objectDestructionMutex.lock();
	drawObjects();
	objectDestructionMutex.unlock();
	drawDescriptions();
	drawDialogBox();
}