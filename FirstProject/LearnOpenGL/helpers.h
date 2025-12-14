#pragma once
#include <string>
#include <iostream>
#include "glad/glad.h"
#include <fstream>
#include <sstream>

void printNumberOfVertexAttributes();
bool readFile(const std::string& path, std::string& outSrc);
