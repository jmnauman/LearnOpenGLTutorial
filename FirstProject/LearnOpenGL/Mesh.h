#pragma once
#include <string>
#include <vec2.hpp>
#include <vec3.hpp>
#include <vector>

#include "shader.h"
#include "glad/glad.h"

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture
{
	GLuint id;
	std::string type;
};

class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	Mesh(std::vector<Vertex> &vertices, std::vector<GLuint> &indices, std::vector<Texture> &textures);
	void Draw(Shader &shader);

private:
	GLuint VAO = 0, VBO = 0, EBO = 0;
	void setupMesh();
};

