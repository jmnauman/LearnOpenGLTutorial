#pragma once

#include <string>
#include <glad/glad.h>
#include <iostream>
#include "helpers.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	unsigned int id;

	Shader(const char* vertexPath, const char* fragmentPath);

	void use() const;
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setMatrix4(const std::string& name, glm::mat4 mat) const;
};