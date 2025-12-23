#pragma once

#include <vec3.hpp>
#include <vector>
#include <glad/glad.h>

GLuint getTriangleVAO();
GLuint getRectangleVAO(float texScale, float texOffset);
GLuint getTwoTrianglesVAO();
GLuint getTriangleTwoVAO();
GLuint getTriangleVAOWithTexCoord();
GLuint getBoxVAO();
GLuint getLightSourceVAO();
std::vector<glm::vec3> tenRandomPositions();