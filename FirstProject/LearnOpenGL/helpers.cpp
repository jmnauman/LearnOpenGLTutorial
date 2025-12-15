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