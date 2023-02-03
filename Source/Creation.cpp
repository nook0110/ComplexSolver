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
{}

Creation::~Creation()
{
  delete running;
}

Creation* Creation::getInstance()
{
  if (creator == nullptr)
  {
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
  if (UnitCircle::getInstance()->GetVisibility() || Creation::getInstance()->getCurrentMode() == MODE_HIDE)
  {
    UnitCircle::getInstance()->Draw();
  }
  if (CenterPoint::getInstance()->GetVisibility() || Creation::getInstance()->getCurrentMode() == MODE_HIDE)
  {
    CenterPoint::getInstance()->Draw();
  }
  if (Creation::getInstance()->getCurrentMode() == MODE_CLEAR)
  {
    return;
  }
  if (Creation::getInstance()->getCurrentMode() != MODE_HIDE)
  {
    std::list<Point*> points;
    std::list<Line*> lines;
    std::list<Object*> other;

    for (auto object : Drawer::all_visible_objects)
    {
      if (!object->GetVisibility())
        continue;

      auto point = dynamic_cast<Point*>(object);
      if (point)
      {
        points.push_back(point);
        continue;
      }

      auto line = dynamic_cast<Line*>(object);
      if (line)
      {
        lines.push_back(line);
        continue;
      }

      other.push_back(object);
    }

    for (auto object : other)
      object->Draw();

    for (auto line : lines)
      line->Draw();

    for (auto point : points)
      point->Draw();
  }
  else
  {
    for (auto object : Drawer::all_visible_objects)
    {
      if (Creation::getInstance()->getCurrentMode() == MODE_CLEAR)
      {
        return;
      }
      object->Draw();
    }
  }
}

void Drawer::drawMenu()
{
  mainMenu.Draw();
}

void Drawer::drawDescriptions()
{
  for (auto description_ : Drawer::allDescriptions)
  {
    description_->Draw();
  }
}

void Drawer::drawDialogBox()
{
  if (dialogBox)
  {
    dialogBox->Draw();
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
    if (!button->GetPressed())
    {
      mainMenu.unpress();
      button->press();
      Creation::getInstance()->CurrentMethod = button->GetFunction();
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
  for (auto obj : all_visible_objects)
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
  new MementoDeletion(object);
  object->DeleteFromDrawer();
}

void Drawer::addObject(Object* object)
{
  new MementoAddition(object);
  object->AddToDrawer();
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

void Drawer::Draw()
{
  objectDestructionMutex.lock();
  drawObjects();
  objectDestructionMutex.unlock();
  drawDescriptions();
  drawDialogBox();
}