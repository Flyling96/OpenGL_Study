#include "Model.h"



Model::Model(char *path)
{
	Init(path);
}

void Model::Init(char *path)
{
	loadModel(path);
}

Model::~Model()
{
}

void Model::loadModel(string path)
{
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	//ReadFile(·��,���ڴ�������)  
	//aiProcess_Triangulate  ���ģ�Ͳ��ǣ�ȫ��������������ɣ�����Ҫ��ģ�����е�ͼԪ��״�任Ϊ������
	//aiProcess_FlipUVs  ���ڴ����ʱ��תy�����������

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);

}

void Model::processNode(aiNode *node, const aiScene *scene)
{
	// ����ڵ����е���������еĻ���
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	// �������������ӽڵ��ظ���һ����
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
	vector<Vertex> vertices;
	vector<int> indices;
	vector<Texture> textures;

	for (int i = 0; i < mesh->mNumVertices; i++)
	{
		// ������λ�á����ߺ���������
		VertexPosition position(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);;
		Normal normal(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		TextureUV uv;
		if (mesh->mTextureCoords[0]) // �����Ƿ����������ꣿ
		{
			uv.ChangeUV(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		}
		
		Vertex vertex(position, Color(1, 1, 1, 1), uv, normal);
		vertices.push_back(vertex);
	}
	// ��������
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	//// �������
	//if (mesh->mMaterialIndex >= 0)
	//{
	//	aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
	//	//��������ͼ
	//	vector<Texture> diffuseMaps = loadMaterialTextures(material,aiTextureType_DIFFUSE, "texture_diffuse");
	//	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	//	//���淴����ͼ
	//	vector<Texture> specularMaps = loadMaterialTextures(material,aiTextureType_SPECULAR, "texture_specular");
	//	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	//	//������ͼ
	//	std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
	//	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	//	//�߶�ͼ
	//	std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
	//	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
	//}

	return Mesh(vertices, indices, textures);
}

vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName)
{
	vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++)
		{
			if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{   // �������û�б����أ��������
			Texture texture;
			texture.id = LoadTexture(str.C_Str(), directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture); // ��ӵ��Ѽ��ص�������
		}
	}
	return textures;
}

unsigned int Model::LoadTexture(std::string name, std::string directory)
{
	string imagePath = string(name);
	imagePath = directory + '/' + imagePath;

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	// ���ز���������
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(imagePath.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		GLenum format;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	return texture;
}

void Model::Draw(Shader shader)
{	
	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].Draw(shader);
}
