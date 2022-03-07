#include "Memento.h"
std::list<Memento*> Memento::mementos;
std::list<Memento*>::iterator Memento::it;

Memento::Memento(Object* object, CHANGES change)
	:object(object), change(change)
{
	while (!mementos.empty() && prev(mementos.end()) != it)
	{
		auto delIt = prev(mementos.end());
		Memento* mem = (*delIt);
		objectDestructionMutex.lock();
		if (mem->getChange() == CHANGES::ADDITION)
		{
			delete mem->getObject();
		}
		objectDestructionMutex.unlock();
		mementos.pop_back();
	}
	mementos.push_back(this);
	it = prev(mementos.end());
}

void Memento::previousMemento()
{
	if (it == mementos.begin())
	{
		return;
	}
	Memento* mem = (*it);
	Object* obj = mem->getObject();
	switch (mem->getChange())
	{
	case CHANGES::ADDITION:
		obj->del();
		break;
	case CHANGES::DELETION:
		obj->add();
		break;
	}
	it--;
}

void Memento::nextMemento()
{
	if (it == prev(mementos.end()))
	{
		return;
	}
	it++;
	Memento* mem = (*it);
	Object* obj = mem->getObject();
	switch (mem->getChange())
	{
	case CHANGES::ADDITION:
		obj->add();
		break;
	case CHANGES::DELETION:
		obj->del();
		break;
	}
}

CHANGES Memento::getChange()
{
	return change;
}

Object* Memento::getObject()
{
	return object;
}
