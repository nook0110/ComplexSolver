// TODO: Refactor

#include "gui.h"

void Button::UpdateSprite()
{
  sprite_.setScale(1 / sprite_.getScale().x, 1 / sprite_.getScale().y); // Scaling Sprite to 1*1;
  sprite_.setScale(
    size_.x / sprite_.getLocalBounds().width,
    size_.y / sprite_.getLocalBounds().height); // Scaling Sprite to required size;
}

void Button::UpdateTexture(std::string texture_location, Vector2i texture_start, Vector2i texture_size)
{
  texture_.loadFromFile(texture_location, IntRect(texture_start, texture_size));
  texture_.setSmooth(true);
  sprite_.setTexture(texture_);
  UpdateSprite();
}

Button::Button(RenderWindow& window, std::string texture_location, MODES mode, std::function<Object* (void)> modeFunction)
  :window_(window), texture_location_(texture_location), mode_(mode), mode_function_(modeFunction)
{
  UpdateTexture(texture_location, Vector2i(0, 0), Vector2i(0, 0));
  sprite_.setColor(kUnpressedColor);
}

void Button::SetPosition(Vector2f position)
{
  sprite_.setPosition(position);
}

void Button::SetSize(Vector2f size)
{
  Button::size_ = size;
  UpdateSprite();
}

void Button::Draw()
{
  window_.draw(sprite_);
}

bool Button::CheckMouse(View buttonView = view) const
{
  Vector2f point = window_.mapPixelToCoords(WrapMouse::getClickedCoord(Mouse::Button::Left), buttonView);
  return sprite_.getGlobalBounds().contains(point);
}

bool Button::leftClickCheck(View buttonView = view)
{
  Vector2f point = window_.mapPixelToCoords(WrapMouse::getClickedCoord(Mouse::Button::Left), buttonView);
  if (!Mouse::isButtonPressed(Mouse::Button::Left)) left_pressed_ = false;
  if (!left_pressed_
      && sprite_.getGlobalBounds().contains(point)
      && Mouse::isButtonPressed(Mouse::Button::Left))
  {
    left_pressed_ = true;
    return true;
  }
  else return false;
}

bool Button::rightClickCheck(View buttonView = view)
{
  Vector2f point = window_.mapPixelToCoords(WrapMouse::getClickedCoord(Mouse::Button::Right), buttonView);
  if (!Mouse::isButtonPressed(Mouse::Button::Right)) right_pressed_ = false;
  if (!right_pressed_ && sprite_.getGlobalBounds().contains(point))
  {
    right_pressed_ = true;
    return true;
  }
  else return false;
}

const Vector2f& Button::getSize() const
{
  return size_;
}

const Vector2f& Button::GetPosition() const
{
  return  sprite_.getPosition();
}

void Button::UpdateMouseMode() const
{
  Mousemode = mode_;
}

void Button::press()
{
  pressed_ = true;
  UpdateMouseMode();
  if (texture_pressed_)
  {
    UpdateTexture(texture_pressed_location_, texture_pressed_start_, texture_pressed_size_);
  }
  else
  {
    sprite_.setColor(kPressedColor);
  }
}

void Button::unpress()
{
  pressed_ = false;
  if (texture_pressed_)
  {
    UpdateTexture(texture_location_, texture_start_, texture_size_);
  }
  else
  {
    sprite_.setColor(kUnpressedColor);
  }
}

bool Button::GetPressed() const
{
  return pressed_;
}

const std::function<Object* (void)>& Button::GetFunction() const
{
  return mode_function_;
}

float Menu::adjustSize(float size, Vector2f menuSize, int count)
{
  const float coeff = 1.001f;
  float nextSize = size * coeff;
  while (floor((menuSize.x - nextSize * kShiftRatio) / nextSize) * floor((menuSize.y - nextSize * kShiftRatio) / nextSize) > count - 1)
  {
    size = nextSize;
    nextSize *= coeff;
  }
  return size;
}

void Menu::updateButtons()
{
  window_.setView(normal_view);
  Vector2f menuSize = normal_view.getSize();
  float button_size;

  // c - buttons.size(), s - button_size, x - menuSize.x, y - menuSize.y
  // (x / s - 1) * (y / s - 1) > c
  // (c - 1) * s ^ 2 + (x + y) * s - x * y = 0

  if (buttons[layer].size() > 1)
  {
    float a = buttons[layer].size() - 1;
    float b = menuSize.x + menuSize.y;
    float c = -menuSize.x * menuSize.y;
    button_size = ((-b + sqrt(b * b - 4 * a * c)) / (2 * a));
  }
  else
  {
    button_size = std::min(menuSize.x, menuSize.y) / 2;
  }
  button_size = adjustSize(button_size, menuSize, buttons[layer].size());

  buttonTable.x = (menuSize.x) / (button_size);
  buttonTable.y = ceil((float)buttons[layer].size() / (float)buttonTable.x);
  resize(button_size * buttonTable.y + kShiftRatio * button_size);
  button_size /= (1 + kShiftRatio);
  float shiftSize = button_size * kShiftRatio;

  for (int i = 0; i < buttons[layer].size(); i++)
  {
    int row = i / buttonTable.x;
    buttons[layer][i]->SetPosition(Vector2f(
      (i - row * buttonTable.x) * (button_size + shiftSize) + shiftSize,
      row * (button_size + shiftSize) + shiftSize
    ));
    buttons[layer][i]->SetSize(Vector2f(button_size, button_size));
  }
}

void Menu::resize(float newSize)
{
  float ratio = newSize / menu_view.getSize().y;
  mainWindowRect.top = ratio * normal_viewport.height;
  view.setViewport(mainWindowRect);
  background.setSize(Vector2f(menu_view.getSize().x, newSize));
  background.setPosition(position);
}

void Menu::setViewport(FloatRect viewport)
{
  Menu::viewport = viewport;
  menu_view.setViewport(viewport);
  menu_view.setCenter(menu_view.getSize() / 2.f + position);
  background.setSize(menu_view.getSize());
  background.setPosition(position);
}

Menu::Menu(RenderWindow& window)
  :window_(window)
{
  buttons.resize(3);
  normal_view = window_.getDefaultView();
  normal_view.setSize(normal_view.getSize().x * normal_viewport.width, normal_view.getSize().y * normal_viewport.height);
  normal_view.setCenter(normal_view.getCenter().x * normal_viewport.width, normal_view.getCenter().y * normal_viewport.height);
  normal_view.setViewport(normal_viewport);
  menu_view = window_.getDefaultView();
  menu_view.setSize(menu_view.getSize().x * viewport.width, menu_view.getSize().y * viewport.height);
  menu_view.setCenter(menu_view.getCenter().x * viewport.width, menu_view.getCenter().y * viewport.height);
  menu_view.setViewport(viewport);
  background.setFillColor(color);
}
void Menu::update(Event event)
{
  FloatRect visibleArea(menu_view.getCenter().x - menu_view.getSize().x / 2, menu_view.getCenter().y - menu_view.getSize().y / 2, event.size.width * viewport.width, event.size.height * viewport.height);
  FloatRect normalVisibleArea = FloatRect(menu_view.getCenter().x - menu_view.getSize().x / 2, menu_view.getCenter().y - menu_view.getSize().y / 2, event.size.width * normal_viewport.width, event.size.height * normal_viewport.height);

  normal_view = View(normalVisibleArea);
  window_.setView(View(visibleArea));
  menu_view = View(visibleArea);
  updateButtons();
}

bool Menu::mouseOnMenu()
{
  Vector2f mouse_position = window_.mapPixelToCoords(Mouse::getPosition(window_), menu_view);
  return background.getGlobalBounds().contains(mouse_position);
}

void Menu::pushButton(Button* newButton, int layerPB)
{
  window_.setView(menu_view);
  if (buttons.size() <= layerPB)
  {
    buttons.resize(layerPB + 1);
  }
  buttons[layerPB].push_back(newButton);
  updateButtons();
}

bool Menu::checkMouse()
{
  window_.setView(menu_view);
  for (auto& Button : buttons[layer])
  {
    if (Button->CheckMouse(menu_view))
    {
      return true;
    }
  }
  return false;
}

Button* Menu::leftClickCheck()
{
  window_.setView(menu_view);
  for (auto& Button : buttons[layer])
  {
    if (Button->leftClickCheck(menu_view))
    {
      return Button;
    }
  }
  return nullptr;
}

void Menu::unpress()
{
  window_.setView(menu_view);
  for (auto& Button : buttons[layer])
  {
    Button->unpress();
  }
  Mousemode = MODE_NOTHING;
}

void Menu::switchLayer(unsigned int newLayer)
{
  if (layer == newLayer)
  {
    layer = 0;
  }
  else
  {
    layer = newLayer;
  }
  updateButtons();
  unpress();
}

void Menu::Draw()
{
  menu_view.setViewport(viewport);
  menu_view.setCenter(menu_view.getSize() / 2.f + position);
  window_.setView(menu_view);
  window_.draw(background);
  for (auto& button : buttons[layer])
  {
    button->Draw();
  }
}

DialogBox* Drawer::dialogBox;
std::list<Description*> Drawer::allDescriptions;
DialogBox::DialogBox()
{
  if (Drawer::dialogBox)
  {
    throw std::runtime_error("Second DialogBox is not supported");
  }
  Drawer::dialogBox = this;
  font.loadFromFile("Textures\\Font\\font.ttf");
  dialogBox.setFillColor(color);
  shadow.setFillColor(shadowColor);
  FloatRect visibleArea(dialogBoxView.getCenter().x - dialogBoxView.getSize().x / 2, dialogBoxView.getCenter().y - dialogBoxView.getSize().y / 2, mainWindow.getSize().x * viewport.width, mainWindow.getSize().y * viewport.height);
  dialogBoxView = View(visibleArea);
}

void DialogBox::update(Event event)
{
  FloatRect visibleArea(dialogBoxView.getCenter().x - dialogBoxView.getSize().x / 2, dialogBoxView.getCenter().y - dialogBoxView.getSize().y / 2, event.size.width * viewport.width, event.size.height * viewport.height);
  mainWindow.setView(dialogBoxView = View(visibleArea));
}

void DialogBox::setSize(Vector2f size)
{
  sizeDialogBox = size;
  sizeTextBox = size - Vector2f(5, 5);
  dialogBox.setSize(sizeDialogBox);
  shadow = dialogBox;
  textBox.setSize(sizeTextBox);
}

void DialogBox::cin(Event event)
{
}

void DialogBox::Draw()
{
  deletion.lock();
  mainWindow.setView(dialogBoxView);
  Vector2f position = dialogBoxView.getCenter() - sizeDialogBox / 2.f;
  dialogBox.setPosition(position);
  textBox.setPosition(position + textBoxOffset);
  shadow.setPosition(position + shadowOffset);
  mainWindow.draw(shadow);
  mainWindow.draw(dialogBox);
  mainWindow.draw(textBox);
  Text text;
  if (textIn.empty())
  {
    text = Text(formatIn, font, textSize);
    text.setFillColor(color);
  }
  else
  {
    text = Text(textIn, font, textSize);
    text.setFillColor(Color::Black);
  }
  text.setPosition(position + textBoxOffset + textOffset);
  mainWindow.draw(text);
  deletion.unlock();
}

bool DialogBox::isFinished()
{
  return finished;
}

DialogBox::~DialogBox()
{
  deletion.lock();
  Drawer::dialogBox = nullptr;
  deletion.unlock();
}

ScalarBox::ScalarBox() :DialogBox()
{
  formatIn = "Input: p:q";
}

std::pair<int, int> ScalarBox::getRatio()
{
  auto position = textIn.find(":");
  try
  {
    int firstNumber = stoi(textIn.substr(0, position));
    int secondNumber = stoi(textIn.substr(position + 1));
    return { firstNumber,secondNumber };
  }
  catch (std::invalid_argument)
  {
    return { 1,1 };
  }
}

void ScalarBox::cin(Event event)
{
  switch (event.text.unicode)
  {
    case 8:
    if (!textIn.empty())
    {
      textIn.pop_back();
    }
    break;
    case 13:
    finished = true;
    break;
    default:
    if ((event.text.unicode >= '0' && event.text.unicode <= '9' || event.text.unicode == '-')
        || event.text.unicode == '.' || event.text.unicode == ',' || event.text.unicode == ':'
        )
    {
      Text text(textIn, font, textSize);
      if (text.getLocalBounds().width < sizeTextBox.x - 2 * textOffset.x)
      {
        textIn += event.text.unicode;
      }
    }
  }
}

std::map<std::string, bool> NameBox::names;

std::string NameBox::newName(std::string name)
{
  auto lowCaseName = name;
  std::transform(lowCaseName.begin(), lowCaseName.end(), lowCaseName.begin(),
                 [](unsigned char c) { return std::tolower(c); });
  if (names[lowCaseName])
  {
    return newName(name + "1");
  }
  else
  {
    names[lowCaseName] = 1;
    return name;
  }
}

NameBox::NameBox() : DialogBox()
{
  formatIn = "Enter name...";
}

NameBox::NameBox(std::string name) : DialogBox()
{
  formatIn = "Enter name...";
  textIn = name;
  finished = true;
}

void NameBox::cin(Event event)
{
  switch (event.text.unicode)
  {
    case 8:
    if (textIn.empty())
    {
      break;
    }
    textIn.pop_back();
    break;
    case 13:
    if (textIn.empty())
    {
      break;
    }
    finished = true;
    break;
    default:
    Text text(textIn, font, textSize);
    if (text.getLocalBounds().width < sizeTextBox.x - 2 * textOffset.x)
    {
      textIn += event.text.unicode;
    }
  }
}

void NameBox::setName(std::string name)
{
  textIn = name;
  finished = true;
}

std::string NameBox::getName()
{
  return newName(textIn);
}

TextBox::TextBox() : DialogBox()
{
  setSize(Vector2f(400, 60));
}

void TextBox::setText(std::string text)
{
  textIn = text;
  Text txt = Text(textIn, font, textSize);
  setSize(Vector2f(txt.getGlobalBounds().width, txt.getGlobalBounds().height) + (textBoxOffset + textOffset) * 3.f);
}

void TextBox::setColor(Color color)
{
  DialogBox::color = color;
}

void TextBox::Draw()
{
  deletion.lock();
  mainWindow.setView(dialogBoxView);
  Vector2f position = dialogBoxView.getCenter() + dialogBoxView.getSize() / 2.f - sizeDialogBox + cornerOffset;
  dialogBox.setPosition(position);
  textBox.setPosition(position + textBoxOffset);
  shadow.setPosition(position + shadowOffset);
  mainWindow.draw(shadow);
  mainWindow.draw(dialogBox);
  mainWindow.draw(textBox);
  Text text;

  text = Text(textIn, font, textSize);
  text.setFillColor(color);

  text.setPosition(position + textBoxOffset + textOffset);
  mainWindow.draw(text);
  deletion.unlock();
}

Description::Description(std::string filePath, std::string name)
  :name(name)
{
  texture.loadFromFile(filePath);
  texture.setSmooth(true);
  sprite.setTexture(texture);
  Vector2f size = Vector2f(texture.getSize()) / 3.f;
  background.setOrigin(backgroundDelta);
  background.setSize(size + backgroundDelta * 2.f);
  background.setFillColor(Color::White);
  background.setOutlineColor(Color::Black);
  background.setOutlineThickness(outline_thickness_);;
  sprite.setScale(1 / sprite.getScale().x, 1 / sprite.getScale().y); // Scaling Sprite to 1*1;
  sprite.setScale(
    size.x / sprite.getLocalBounds().width,
    size.y / sprite.getLocalBounds().height);
  Drawer::allDescriptions.push_front(this);
}

Description::~Description()
{
  Drawer::allDescriptions.remove(this);
}

bool Description::contains(Vector2f point)
{
  return background.getGlobalBounds().contains(point);
}

void Description::moveTo(Vector2f position)
{
  Description::position = position;
  sprite.setPosition(position);
  background.setPosition(position);
}

Vector2f Description::getDelta(Vector2f position)
{
  return position - background.getPosition();
}

void Description::Draw()
{
  mainWindow.draw(background);
  mainWindow.draw(sprite);
}