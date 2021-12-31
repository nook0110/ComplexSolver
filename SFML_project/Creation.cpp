#include "Creation.h"

bool Checker::checkMode()
{
	if (last != Mousemode)
	{
		last = Mousemode;
		return false;
	}
	return true;
}

void Creation::operator()()

{
	if (!checker.checkMode())
	{
		created = true;
	}
	if (running.joinable())
	{
		running.detach();
	}
	if (created)
	{
		created = false;
		thread Creator(CurrentMethod);
		swap(running, Creator);
		if (running.joinable())
		{
			running.detach();
		}
	}
}

void Creation::Create()
{
	created = true;
}