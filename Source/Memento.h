#pragma once
#include "Object.h"
#include <list>
extern std::mutex objectDestructionMutex;


class Object;
class Memento
{
protected:
	static std::list<Memento*> mementos;
	static std::list<Memento*>::iterator it;
	Object* object;
	virtual void doMemento() = 0;
	virtual void undoMemento() = 0;
public:
	Memento(Object* object);
	static void previousMemento();
	static void nextMemento();
	static void clear();
	Object* getObject();
};

class MementoDeletion : public Memento
{
	virtual void doMemento();
	virtual void undoMemento();
public:
	MementoDeletion(Object* object);
};

class MementoAddition : public Memento
{
	virtual void doMemento();
	virtual void undoMemento();
public:
	MementoAddition(Object* object);
};