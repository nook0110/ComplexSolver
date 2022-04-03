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
private:
	static void compileSettings(std::stringstream& sstring);
	static void compilePoint(std::stringstream& sstring, std::string name, std::string construction);
	static void compileUnitPoint(std::stringstream& sstring, std::string name, std::string construction);
	static void compileLine(std::stringstream& sstring, std::string name, std::string construction);
	static void compileChord(std::stringstream& sstring, std::string name, std::string construction);
	static void compileProve(std::stringstream& sstring, std::string name, std::string construction);
public:
	static std::map<std::string, Point*> points;
	static std::map<std::string, Line*> lines;
	static void codeLineCompile(std::string line);
	static void compile(std::string filePath);
};