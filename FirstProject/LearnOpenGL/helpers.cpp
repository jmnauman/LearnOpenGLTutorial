#include "helpers.h"
#include <iostream>
#include "glad/glad.h"

// OpenGL guarantees 16 4-component vertex attributes, but more may be available depending on hardware.
void printNumberOfVertexAttributes()
{
	int nrAttributes = 0;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum number of vertex attributes supported: " << nrAttributes << '\n';
}