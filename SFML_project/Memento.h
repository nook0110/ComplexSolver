#pragma once
#include "Object.h"
#include <list>
extern std::mutex objectDestructionMutex;

enum CHANGES
{
	ADDITION,
	DELETION
};
class Object;
class Memento
{
	static std::list<Memento*> mementos;
	static std::list<Memento*>::iterator it;
	CHANGES change;
	Object* object;
public:
	Memento(Object* object, CHANGES change);
	static void previousMemento();
	static void nextMemento();
	CHANGES getChange();
	Object* getObject();
};
