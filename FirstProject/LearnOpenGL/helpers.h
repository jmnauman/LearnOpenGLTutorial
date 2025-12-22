#pragma once
#include <string>
#include <fstream>
#include <glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "glad/glad.h"
#include <sstream>

void printNumberOfVertexAttributes();
bool readFile(const std::string& path, std::string& outSrc);

glm::vec3 zAxis();
glm::mat4 t(glm::vec3 trans);
glm::mat4 r(glm::vec3 axis, float angle);
glm::mat4 tr(glm::vec3 trans, glm::vec3 axis, float angle);
glm::mat4 ts(glm::vec3 trans, glm::vec3 scale);
glm::mat4 ts(glm::vec3 trans, float scale);
glm::mat4 trs(glm::vec3 trans, glm::vec3 axis, float angle, float scale);
glm::mat4 trs(glm::vec3 trans, glm::vec3 axis, float angle, glm::vec3 scale);
glm::mat4 oProj(float minusX, float x, float minusY, float y, float minusZ, float z);
glm::mat4 pProj(float fovDeg, float width, float height, float near, float far);