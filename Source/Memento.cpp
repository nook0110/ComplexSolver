#include "Memento.h"
std::list<Memento*> Memento::mementos;
std::list<Memento*>::iterator Memento::it = Memento::mementos.end();

Memento::Memento(Object* object)
	:object(object)
{
	objectDestructionMutex.lock();
	while (!mementos.empty() && prev(mementos.end()) != it)
	{
		auto delIt = prev(mementos.end());
		Memento* mem = (*delIt);
		if (dynamic_cast<MementoAddition*>(mem))
		{
			delete mem->getObject();
			delete mem;
		}
		mementos.pop_back();
	}
	objectDestructionMutex.unlock();
	mementos.push_back(this);
	it = prev(mementos.end());
}

void Memento::clear()
{
	
	while (!mementos.empty())
	{
		auto delIt = prev(mementos.end());
		Memento* mem = (*delIt);
		if (dynamic_cast<MementoAddition*>(mem))
		{
			objectDestructionMutex.lock();
			delete mem->getObject();
			delete mem;
			objectDestructionMutex.unlock();
		}
		mementos.pop_back();
	}

	it = mementos.begin();
}

void Memento::previousMemento()
{
	if (Mousemode == MODES::MODE_NAMING)
	{
		return;
	}
	auto current = Mousemode;
	Mousemode = MODES::MODE_MEMENTING;
	std::this_thread::sleep_for(std::chrono::nanoseconds(10000));
	if (it == mementos.end())
	{
		Mousemode = current;
		return;
	}
	Memento* mem = (*it);
	mem->undoMemento();
	if (it == mementos.begin())
	{
		Mousemode = current;
		return;
	}
	it--;
}

void Memento::nextMemento()
{
	if (it == mementos.end())
	{
		return;
	}
	Memento* mem = (*it);
	mem->doMemento();
	if (it == prev(mementos.end()))
	{
		return;
	}
	it++;
}

Object* Memento::getObject()
{
	return object;
}

MementoDeletion::MementoDeletion(Object* object) : Memento(object)
{
}

void MementoDeletion::doMemento()
{
	NameBox::names[object->GetLowerCaseName()] = false;
	object->DeleteFromDrawer();
}

void MementoDeletion::undoMemento()
{
	NameBox::names[object->GetLowerCaseName()] = true;
	object->AddToDrawer();
}

MementoAddition::MementoAddition(Object* object) : Memento(object)
{
}

void MementoAddition::doMemento()
{
	NameBox::names[object->GetLowerCaseName()] = true;
	Drawer::all_visible_objects.push_back(object);
}

void MementoAddition::undoMemento()
{
	NameBox::names[object->GetLowerCaseName()] = false;
	object->DeleteFromDrawer();
}