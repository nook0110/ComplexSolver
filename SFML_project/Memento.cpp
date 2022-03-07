#include "Memento.h"
std::list<Memento*> Memento::mementos;
std::list<Memento*>::iterator Memento::it;

Memento::Memento(Object* object)
	:object(object)
{
	while (!mementos.empty() && prev(mementos.end()) != it)
	{
		auto delIt = prev(mementos.end());
		Memento* mem = (*delIt);
		objectDestructionMutex.lock();
		if (dynamic_cast<MementoDeletion*>(mem))
		{
			delete mem->getObject();
			delete mem;
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
	mem->undoMemento();
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
	mem->doMemento();
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
	NameBox::names[object->getLowerCaseName()] = false;
	object->del();
}

void MementoDeletion::undoMemento()
{
	NameBox::names[object->getLowerCaseName()] = true;
	object->add();
}

MementoAddition::MementoAddition(Object* object) : Memento(object)
{
}

void MementoAddition::doMemento()
{
	NameBox::names[object->getLowerCaseName()] = true;
	Drawer::allVisibleObjects.push_back(object);
}

void MementoAddition::undoMemento()
{
	NameBox::names[object->getLowerCaseName()] = false;
	object->del();
}