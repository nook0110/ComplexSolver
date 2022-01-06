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
	if (!checker.checkMode())
	{
		created = true;
	}
	if (running.joinable() && getCurrentMode() != MODE_NOTHING)
	{
		running.detach();
	}
	if (created)
	{
		created = false;
		jthread Creator(CurrentMethod);
		swap(running, Creator);
		if (running.joinable() && getCurrentMode() != MODE_NOTHING)
		{
			Creator.request_stop();
			running.detach();
		}
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

void Drawer::draw()
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
