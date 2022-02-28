#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
using namespace sf;
class Printer
{
public:
	static std::string makeTexture(std::string texText, std::string fileName);
};