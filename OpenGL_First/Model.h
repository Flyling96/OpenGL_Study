#pragma once
#include"Shader.h"
#include"Mesh.h"

#include<fstream>
#include<sstream>
#include<iostream>
#include<vector>
#include<string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


using namespace std;
class Model
{
public:
	/*  函数   */
	Model(char *path);
	Model() {};
	void Init(char *path);
	~Model();
	void Draw(Shader shader);
private:
	/*  模型数据  */
	vector<Texture> textures_loaded;
	vector<Mesh> meshes;
	string directory;
	/*  函数   */
	void loadModel(string path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
		string typeName);
	unsigned int LoadTexture(std::string name, std::string directory);
};