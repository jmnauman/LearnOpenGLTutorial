#include "helpers.h"

// OpenGL guarantees 16 4-component vertex attributes, but more may be available depending on hardware.
void printNumberOfVertexAttributes()
{
	int nrAttributes = 0;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum number of vertex attributes supported: " << nrAttributes << '\n';
}

bool readFile(const std::string& path, std::string& outSrc)
{
	bool success = false;

	std::ifstream file;
	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		file.open(path);
		std::stringstream stream;
		stream << file.rdbuf();
		outSrc = stream.str();
		file.close();
		success = true;
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "Could not read shader file " << path << "\n" << e.what() << "\n";
	}

	return success;
}

glm::vec3 zAxis()
{
	return glm::vec3(0.f, 0.f, 1.f);
}

glm::mat4 t(glm::vec3 trans)
{
	glm::mat4 m = glm::mat4(1.f);
	return glm::translate(m, trans);
}

glm::mat4 r(glm::vec3 axis, float angle)
{
	glm::mat4 m = glm::mat4(1.f);
	return glm::rotate(m, angle, axis);
}

glm::mat4 tr(glm::vec3 trans, glm::vec3 axis, float angle)
{
	glm::mat4 m = t(trans);
	return glm::rotate(m, angle, axis);
}

glm::mat4 ts(glm::vec3 trans, glm::vec3 scale)
{
	glm::mat4 m = t(trans);
	return glm::scale(m, scale);
}

glm::mat4 ts(glm::vec3 trans, float scale)
{
	glm::vec3 v(scale);
	return ts(trans, v);
}

glm::mat4 trs(glm::vec3 trans, glm::vec3 axis, float angle, float scale)
{
	glm::vec3 v(scale);
	return trs(trans, axis, angle, v);
}

glm::mat4 trs(glm::vec3 trans, glm::vec3 axis, float angle, glm::vec3 scale)
{
	glm::mat4 m = tr(trans, axis, angle);
	return glm::scale(m, scale);
}

glm::mat4 oProj(float minusX, float x, float minusY, float y, float minusZ, float z)
{
	return glm::ortho(minusX, x, minusY, y, minusZ, z);
}

glm::mat4 pProj(float fovDeg, float width, float height, float near, float far)
{
	// fov is the vertical angle, i.e. around x axis.
	return glm::perspective(glm::radians(fovDeg), width / height, near, far);
}
