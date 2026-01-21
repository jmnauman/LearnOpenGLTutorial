#pragma once
#include "Mesh.h"
#include "assimp/scene.h"
#include "assimp/mesh.h"
#include "assimp/material.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"

class Shader;

class Model
{
public:
	Model(const char *path)
	{
		loadModel(path);
	}
	void Draw(Shader & shader);

private:
	std::vector<Mesh> meshes;
	std::string directory;
	std::vector<Texture> loadedTextures;

	void loadModel(std::string path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh * mesh, const aiScene *scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};