// TODO: Refactor

#pragma once
#include <SFML/Graphics.hpp>
#include "WrapMouse.h"
#include "Creation.h"
#include <mutex>
#include <iostream>
#include <map>

//File about GUI(graphical user interface): Menu, Buttons, Dialog Boxes, Descriptions

extern sf::Event event;
extern sf::FloatRect mainWindowRect;
extern sf::RenderWindow mainWindow;
extern sf::View view;

extern MODES Mousemode;

class Object;

//Button is a clickable object on the screen.
//It has it own mode and std::function.
class Button : public sf::Drawable
{
public:
  Button(sf::RenderWindow& window, std::string texture_location,
         MODES mode = MODE_NOTHING, std::function<Object* (void)> modeFunction = []()->Object* {});
  void SetPosition(sf::Vector2f);
  void SetSize(sf::Vector2f size);
  bool GetPressed() const;

  void Draw();
  bool CheckMouse(View) const;
  bool leftClickCheck(View);
  bool rightClickCheck(View);
  const sf::Vector2f& getSize() const;
  const sf::Vector2f& GetPosition() const;
  void press();
  void unpress();
  const std::function<Object* (void)>& GetFunction() const;

  void draw(sf::RenderTarget& target, sf::RenderStates states) const final {};
private:
  void UpdateMouseMode() const;
  void UpdateSprite();
  void UpdateTexture(std::string texture_location, sf::Vector2i texture_start, sf::Vector2i textureSize);

  const sf::Color kUnpressedColor = sf::Color(255, 255, 255, 255);
  const sf::Color kPressedColor = sf::Color(128, 128, 128, 255);

  sf::Vector2f size_;
  sf::Vector2i texture_size_ = sf::Vector2i(0, 0);
  sf::Vector2i texture_pressed_size_ = sf::Vector2i(0, 0);
  sf::Vector2i texture_start_ = sf::Vector2i(0, 0);
  sf::Vector2i texture_pressed_start_ = sf::Vector2i(0, 0);
  std::string texture_location_;
  std::string texture_pressed_location_;
  sf::RenderWindow& window_;
  Texture texture_;
  Sprite sprite_;

  //Button mode
  MODES mode_ = MODE_NOTHING;
  std::function<Object* (void)> mode_function_ = []()->Object* {};

  bool pressed_ = false;
  bool left_pressed_ = false;
  bool right_pressed_ = false;
  bool texture_pressed_ = false;
};

//Menu is a list of buttons, you can push buttons in it.
class Menu
{
private:
  View menu_view;
  sf::RenderWindow& window_;
  const float kShiftRatio = 0.1f; // 10%
  sf::Vector2i buttonTable;
  FloatRect viewport = FloatRect(0.f, 0.f, 1.0f, mainWindowRect.top);
  const FloatRect normal_viewport = FloatRect(0.f, 0.f, 1.0f, mainWindowRect.top);
  View normal_view;
  const Color color = Color(160, 160, 160, 255);
  const sf::Vector2f position = sf::Vector2f(0, 0);
  int layer = 0;
  std::vector<std::vector<Button*>> buttons;
  RectangleShape background;
  float adjustSize(float size, sf::Vector2f menuSize, int count);
  void updateButtons();

  void resize(float newSize);
public:
  void setViewport(FloatRect viewport);
  Menu(sf::RenderWindow& window);
  // Resize menu
  void update(Event event);
  bool mouseOnMenu();
  void pushButton(Button* newButton, int layerPB);
  bool checkMouse();
  Button* leftClickCheck();
  void unpress();
  void switchLayer(unsigned int layer);
  void Draw();
};

class DialogBox
{
protected:
  View dialogBoxView;
  Color color = Color(128, 128, 128);
  Color shadowColor = Color(32, 32, 32);
  const FloatRect viewport = FloatRect(0.f, 0.f, 1.0f, 1.0f);
  sf::Vector2f sizeDialogBox = sf::Vector2f(300, 60);
  sf::Vector2f sizeTextBox = sf::Vector2f(295, 55);
  const unsigned int textSize = 35;
  const sf::Vector2f textOffset = sf::Vector2f((sizeTextBox.y - textSize) / 2, (sizeTextBox.y - textSize) / 2);
  const sf::Vector2f textBoxOffset = (sizeDialogBox - sizeTextBox) / 2.f;
  const sf::Vector2f shadowOffset = sf::Vector2f(1.f, 1.f);
  RectangleShape dialogBox = RectangleShape(sizeDialogBox);
  RectangleShape shadow = dialogBox;
  RectangleShape textBox = RectangleShape(sizeTextBox);
  Font font;
  std::string textIn;
  std::string formatIn;
  bool finished = false;

  std::mutex deletion;
public:
  DialogBox();
  ~DialogBox();
  // Resize DialogBox
  void update(Event event);
  void setSize(sf::Vector2f size);
  // Cin into the box
  virtual void cin(Event event);
  virtual void Draw();
  // Returns if "Enter" was entered
  bool isFinished();
};

// A box to input to numbers: "p:q"
class ScalarBox : public DialogBox
{
public:
  ScalarBox();
  //Returns two numbers p and q
  std::pair<int, int>  getRatio();
  void cin(Event event) override;
};

// A box to name Point
class NameBox : public DialogBox
{
  std::string newName(std::string name);
public:
  static std::map<std::string, bool> names;
  NameBox();
  NameBox(std::string name);
  void cin(Event event) override;
  void setName(std::string name);
  std::string getName();
};

// In construction...
class TextBox : public DialogBox
{
  sf::Vector2f cornerOffset = sf::Vector2f(-50, -50);
public:
  TextBox();
  void setText(std::string text);
  void setColor(Color color);
  void Draw();
};

// A LaTeX description of an Object
class Description
{
  Sprite sprite;
  Texture texture;
  RectangleShape background;
  sf::Vector2f position = sf::Vector2f();
  sf::Vector2f size;
  const sf::Vector2f backgroundDelta = sf::Vector2f(5, 5);
  const float outline_thickness_ = 1;
  std::string name;
public:
  Description(std::string filePath, std::string name);
  ~Description();
  bool contains(sf::Vector2f point);
  void moveTo(sf::Vector2f position);
  //Return delta of a (Left-Upper) Corner and Position
  sf::Vector2f getDelta(sf::Vector2f position);
  void Draw();
};