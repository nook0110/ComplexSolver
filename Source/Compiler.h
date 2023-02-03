#pragma once
#include "Object.h"
#include "presetBoxes.h"

#include <sstream>
#include <fstream>
#include <map>

extern StateMenu stateMenu;
extern presets::isEnabled latex;
extern presets::isEnabled provePrinting;
extern presets::FPS fps;
extern presets::Construction preset;

class Compiler
{
public:
  static std::map<std::string, Point*, std::less<>> points;
  static std::map<std::string, Line*, std::less<>> lines;

  static void CodeLineCompile(std::string points_);
  static void Compile(std::string filePath);
private:
  static void CompileProperties(std::stringstream& sstring, Object* object);
  static void CompileSettings(std::stringstream& sstring);
  static Object* CompilePoint(std::stringstream& sstring, std::string name, std::string construction);
  static Object* CompileUnitPoint(std::stringstream& sstring, std::string name, std::string construction);
  static Object* CompileLine(std::stringstream& sstring, std::string name, std::string construction);
  static Object* CompileChord(std::stringstream& sstring, std::string name, std::string construction);
  static Object* CompileProve(std::stringstream& sstring, std::string name, std::string construction);
};