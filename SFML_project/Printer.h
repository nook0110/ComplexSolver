#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
using namespace sf;
class Printer
{
public:
	// Return a path to texture that created from TeX
	static std::string makeTexture(std::string texText, std::string fileName);
};