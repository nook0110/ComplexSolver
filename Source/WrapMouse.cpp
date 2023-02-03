#include "WrapMouse.h"

extern RenderWindow mainWindow;

Vector2i WrapMouse::leftClickedCoord;
Vector2i WrapMouse::rightClickedCoord;
bool WrapMouse::leftClicked;
bool WrapMouse::rightClicked;

bool WrapMouse::getClicked(Mouse::Button button)
{
  switch (button)
  {
    case Mouse::Button::Left:
    return WrapMouse::leftClicked;
    break;
    case Mouse::Button::Right:
    return WrapMouse::rightClicked;
    break;
  }
}

Vector2i WrapMouse::getClickedCoord(Mouse::Button button)
{
  WrapMouse::checkPress(button);
  switch (button)
  {
    case Mouse::Button::Left:
    return WrapMouse::leftClickedCoord;
    break;
    case Mouse::Button::Right:
    return WrapMouse::rightClickedCoord;
    break;
  }
}

void WrapMouse::setClicked(Mouse::Button button, bool click)
{
  switch (button)
  {
    case Mouse::Button::Left:
    WrapMouse::leftClicked = click;
    break;
    case Mouse::Button::Right:
    WrapMouse::rightClicked = click;
    break;
  }
}

void WrapMouse::setClickedCoord(Mouse::Button button)
{
  switch (button)
  {
    case Mouse::Button::Left:
    WrapMouse::leftClickedCoord = Mouse::getPosition(mainWindow);
    break;
    case Mouse::Button::Right:
    WrapMouse::rightClickedCoord = Mouse::getPosition(mainWindow);
    break;
  }
}

bool WrapMouse::checkPress(Mouse::Button button)
{
  bool pressed = Mouse::isButtonPressed(button);
  if (Mouse::isButtonPressed(button) && !WrapMouse::getClicked(button))
  {
    WrapMouse::setClicked(button, true);
    WrapMouse::setClickedCoord(button);
  }

  if (!Mouse::isButtonPressed(button))
  {
    WrapMouse::setClicked(button, false);
  }

  return WrapMouse::getClicked(button);
}