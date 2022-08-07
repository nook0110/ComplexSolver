#pragma once
#include <iostream>
#include <filesystem>
#include <SFML/Graphics.hpp>
#include <fstream>
using namespace sf;

// Return a path to texture that created from TeX
std::string makeTexture(std::string texText, std::string fileName);
void deleteFiles(std::string fileName);