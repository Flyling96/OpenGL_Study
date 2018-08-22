#pragma once
#include"Shader.h"
#include"Mesh.h"
#include"math_3d.h"

#include<fstream>
#include<sstream>
#include<iostream>
#include<vector>
#include<string>
#include<map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


using namespace std;


struct BoneInfo
{
	glm::mat4 BoneOffset;
	glm::mat4 FinalTransformation;

};

class Model
{
public:
	/*  函数   */
	Model(char *path);
	Model() { m_pScene = NULL; }
	void Init(string const &path);
	~Model();
	void Draw(Shader &shader);
	void BoneTransform(float TimeInSeconds, vector<glm::mat4>& Transforms);
private:
	/*  模型数据  */
	vector<Texture> textures_loaded;
	vector<Mesh> meshes;
	vector<BoneInfo> boneInfos;		   //
	vector<VertexBoneData> boneDates;  //顶点对应的骨骼信息
	string directory;
	map<string, int> boneMapping; // maps a bone name to its index
	int m_NumBones;
	glm::mat4 m_GlobalInverseTransform;
	const aiScene *m_pScene;
	string modelPath;

	/*  函数   */
	void LoadModel(string path);
	void LoadBones(int baseVertex, const aiMesh* pMesh);
	void ProcessNode(const aiNode *node, const aiScene *scene, int &numVertex);
	Mesh ProcessMesh(const aiMesh *mesh, const aiScene *scene);
	vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
	string typeName);
	unsigned int LoadTexture(std::string name, std::string directory);

	//骨骼矩阵计算相关
	int FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
	int FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
	int FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
	void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
	void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
	void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
	void ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const glm::mat4& ParentTransform);
	const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const string NodeName);
};

Model::Model(char *path)
{
	Init(path);
}

void Model::Init(string const &path)
{
	m_NumBones = 0;
	LoadModel(path);

}

Model::~Model()
{
}

void Model::LoadModel(string path)
{
	modelPath = path;
	Assimp::Importer importer;
	m_pScene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);
	//ReadFile(路径,后期处理类型)  
	//aiProcess_Triangulate  如果模型不是（全部）由三角形组成，它需要将模型所有的图元形状变换为三角形
	//aiProcess_FlipUVs  将在处理的时候翻转y轴的纹理坐标

	if (!m_pScene || m_pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !m_pScene->mRootNode)
	{
		cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
		return;
	}

	math_3d::ChangeMat4x4Value(m_GlobalInverseTransform,m_pScene->mRootNode->mTransformation);
	math_3d::Inverse(m_GlobalInverseTransform);

	directory = path.substr(0, path.find_last_of('/'));

	int sumVertices = 0;
	for (int i = 0; i < m_pScene->mNumMeshes; i++) {
		sumVertices += m_pScene->mMeshes[i]->mNumVertices;
	}
	boneDates.resize(sumVertices);

	int numVertex = 0;
	ProcessNode(m_pScene->mRootNode, m_pScene, numVertex);

	for (int i = 0; i < meshes.size(); i++)
	{
		for (int j = 0; j < meshes[i].vertices.size(); j++)
		{
			meshes[i].vertices[j].vertexBoneData = boneDates[meshes[i].baseVertex + j];
		}
		meshes[i].Init();
	}

	int a = 0;

}

void Model::ProcessNode(const aiNode *node, const aiScene *scene,int &numVertex)
{
	// 处理节点所有的网格（如果有的话）
	for (int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(ProcessMesh(mesh, scene));
		meshes[meshes.size() - 1].baseVertex = numVertex;
		LoadBones(numVertex, mesh);
		numVertex += scene->mMeshes[i]->mNumVertices;
	}
	// 接下来对它的子节点重复这一过程
	for (int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i],scene, numVertex);
	}
}

Mesh Model::ProcessMesh(const aiMesh *mesh, const aiScene *scene)
{
	vector<Vertex> vertices;
	vector<int> indices;
	vector<Texture> textures;

	for (int i = 0; i < mesh->mNumVertices; i++)
	{
		// 处理顶点位置、法线和纹理坐标
		VertexPosition position(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);;
		Normal normal;
		if (mesh->mNormals)
		{
			normal.ChangeNormal(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		}
		TextureUV uv;
		if (mesh->mTextureCoords[0]) // 网格是否有纹理坐标
		{
			uv.ChangeUV(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		}
		Tangent tangent;
		if (mesh->mTangents)
		{
			tangent.ChangeTangent(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
		}

		Bitangent bitangent;
		if (mesh->mBitangents)
		{
			bitangent.ChangeBitangent(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
		}


		Vertex vertex(position, Color(1, 1, 1, 1), uv, normal, tangent, bitangent);
		vertices.push_back(vertex);
	}
	// 处理索引
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	// 处理材质
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
		//漫反射贴图
		vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		//镜面反射贴图
		vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		//法线贴图
		std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		//高度图
		std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
	}

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
		{   // 如果纹理还没有被加载，则加载它
			Texture texture;
			texture.id = LoadTexture(str.C_Str(), directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture); // 添加到已加载的纹理中
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
	// 为当前绑定的纹理对象设置环绕、过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	// 加载并生成纹理
	int width, height, nrChannels;
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

void Model::Draw(Shader &shader)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].Draw(shader);
}

void Model::LoadBones(int baseVertex, const aiMesh* pMesh)
{
	for (int i = 0; i < pMesh->mNumBones; i++) {
		int boneIndex = 0;
		string boneName = pMesh->mBones[i]->mName.data;

		if (boneMapping.find(boneName) == boneMapping.end()) {
			// Allocate an index for a new bone
			boneIndex = m_NumBones;
			m_NumBones++;
			BoneInfo boneInfo;
			boneInfos.push_back(boneInfo);
			math_3d::ChangeMat4x4Value(boneInfos[boneIndex].BoneOffset, pMesh->mBones[i]->mOffsetMatrix);
			boneMapping[boneName] = boneIndex;
		}
		else {
			boneIndex = boneMapping[boneName];
		}

		for (int j = 0; j < pMesh->mBones[i]->mNumWeights; j++) {
			int vertexID = baseVertex + pMesh->mBones[i]->mWeights[j].mVertexId;
			float weight = pMesh->mBones[i]->mWeights[j].mWeight;
			boneDates[vertexID].AddBoneData(boneIndex, weight);
		}
	}
}

void Model::ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const glm::mat4& ParentTransform)
{
	string NodeName = pNode->mName.data;

	glm::mat4 NodeTransformation;
	math_3d::ChangeMat4x4Value(NodeTransformation, pNode->mTransformation);

	const aiNodeAnim* pNodeAnim = FindNodeAnim(m_pScene->mAnimations[0], NodeName);

	if (pNodeAnim) {
		// 计算放缩矩阵
		aiVector3D Scaling;
		CalcInterpolatedScaling(Scaling, AnimationTime, pNodeAnim);
		glm::mat4 ScalingM;
		math_3d::InitScaleTransform(ScalingM,glm::vec3(Scaling.x, Scaling.y, Scaling.z));

		// 计算旋转矩阵
		aiQuaternion RotationQ;
		CalcInterpolatedRotation(RotationQ, AnimationTime, pNodeAnim);
		glm::mat4 RotationM;
		math_3d::ChangeMat4x4Value(RotationM, RotationQ.GetMatrix());

		// 计算位移矩阵
		aiVector3D Translation;
		CalcInterpolatedPosition(Translation, AnimationTime, pNodeAnim);
		glm::mat4 TranslationM;
		math_3d::InitTranslationTransform(TranslationM,glm::vec3(Translation.x, Translation.y, Translation.z));

		NodeTransformation = math_3d::Multiply(TranslationM,math_3d::Multiply(RotationM, ScalingM));
	}

	glm::mat4 GlobalTransformation = math_3d::Multiply(ParentTransform , NodeTransformation);

	if (boneMapping.find(NodeName) != boneMapping.end()) {
		int boneIndex = boneMapping[NodeName];
		boneInfos[boneIndex].FinalTransformation = math_3d::Multiply(m_GlobalInverseTransform,math_3d::Multiply(GlobalTransformation , boneInfos[boneIndex].BoneOffset));
	}

	for (int i = 0; i < pNode->mNumChildren; i++) {
		ReadNodeHeirarchy(AnimationTime, pNode->mChildren[i], GlobalTransformation);
	}
}


void Model::BoneTransform(float TimeInSeconds, vector<glm::mat4>& Transforms)
{

	Assimp::Importer importer;
	m_pScene = importer.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

	glm::mat4 Identity;

	float TicksPerSecond = (float)(m_pScene->mAnimations[0]->mTicksPerSecond != 0 ? m_pScene->mAnimations[0]->mTicksPerSecond : 25.0f);
	float TimeInTicks = TimeInSeconds * TicksPerSecond;
	float AnimationTime = fmod(TimeInTicks, (float)m_pScene->mAnimations[0]->mDuration);
	//float AnimationTime = 5;

	ReadNodeHeirarchy(AnimationTime, m_pScene->mRootNode, Identity);

	Transforms.resize(m_NumBones);

	for (int i = 0; i < m_NumBones; i++) {
		Transforms[i] = boneInfos[i].FinalTransformation;
	}
}

int Model::FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	for (int i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++) {
		if (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime) {
			return i;
		}
	}

	assert(0);

	return 0;
}


int Model::FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	assert(pNodeAnim->mNumRotationKeys > 0);

	for (int i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++) {
		if (AnimationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime) {
			return i;
		}
	}

	assert(0);

	return 0;
}


int Model::FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	assert(pNodeAnim->mNumScalingKeys > 0);

	for (int i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++) {
		if (AnimationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime) {
			return i;
		}
	}

	assert(0);

	return 0;
}


void Model::CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	if (pNodeAnim->mNumPositionKeys == 1) {
		Out = pNodeAnim->mPositionKeys[0].mValue;
		return;
	}

	int PositionIndex = FindPosition(AnimationTime, pNodeAnim);
	int NextPositionIndex = (PositionIndex + 1);
	assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);
	float DeltaTime = (float)(pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiVector3D& Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
	const aiVector3D& End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
	aiVector3D Delta = End - Start;
	Out = Start + Factor * Delta;
}


void Model::CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	// we need at least two values to interpolate...
	if (pNodeAnim->mNumRotationKeys == 1) {
		Out = pNodeAnim->mRotationKeys[0].mValue;
		return;
	}

	int RotationIndex = FindRotation(AnimationTime, pNodeAnim);
	int NextRotationIndex = (RotationIndex + 1);
	assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
	float DeltaTime = (float)(pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
	const aiQuaternion& EndRotationQ = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
	aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
	Out = Out.Normalize();
}


void Model::CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	if (pNodeAnim->mNumScalingKeys == 1) {
		Out = pNodeAnim->mScalingKeys[0].mValue;
		return;
	}

	int ScalingIndex = FindScaling(AnimationTime, pNodeAnim);
	int NextScalingIndex = (ScalingIndex + 1);
	assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);
	float DeltaTime = (float)(pNodeAnim->mScalingKeys[NextScalingIndex].mTime - pNodeAnim->mScalingKeys[ScalingIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiVector3D& Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
	const aiVector3D& End = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
	aiVector3D Delta = End - Start;
	Out = Start + Factor * Delta;
}

const aiNodeAnim* Model::FindNodeAnim(const aiAnimation* pAnimation, const string NodeName)
{
	for (int i = 0; i < pAnimation->mNumChannels; i++) {
		const aiNodeAnim* pNodeAnim = NULL;
		pNodeAnim = pAnimation->mChannels[i];

		if (string(pNodeAnim->mNodeName.data) == NodeName) {
			return pNodeAnim;
		}
	}

	return NULL;
}