#pragma once
#include<GLFW/glfw3.h>
#include<glad/glad.h>
#include<glm/glm.hpp>
#include<vector>

#include"Shader.h"

class Normal
{
public:
	Normal()
	{
		x = 0, y = 0, z = 0;
	}
	Normal(float a, float b, float c)
	{
		ChangeNormal(a, b, c);
	}
	void ChangeNormal(float a, float b, float c)
	{
		x = a;
		y = b;
		z = c;
	}
	glm::vec3 GetNormal()
	{
		return glm::vec3(x, y, z);
	}
private:
	float x;
	float y;
	float z;
};

class VertexPosition
{
public:
	VertexPosition()
	{
		posX = 0, posY = 0,posZ = 0;
	}
	VertexPosition(float x,float y,float z)
	{
		posX = x;
		posY = y;
		posZ = z;
	}

	glm::vec3 GetVertex()
	{
		return glm::vec3(posX, posY, posZ);
	}
private:
	float posX;
	float posY;
	float posZ;
};

class Color
{
public:
	Color()
	{
		r = 1, g = 1, b = 1, a = 1;
	}
	Color(float x, float y, float z,float w )
	{
		r = x;
		g = y;
		b = z;
		a = w;
	}
private:
	float r;
	float g;
	float b;
	float a;
};

class TextureUV
{

public:
	TextureUV()
	{
		u = 0, v = 0;
	}
	TextureUV(float a, float b)
	{
		u = a;
		v = b;
	}

	void ChangeUV(float a, float b)
	{
		u = a;
		v = b;
	}

private:
	float u;
	float v;

};

class Vertex
{
public:
	Vertex() {}
	Vertex(VertexPosition v, Color c, TextureUV t)
	{
		vertex = v;
		color = c;
		texture = t;
		normal = Normal(0, 0, 0);
	}

	Vertex(VertexPosition v, Color c, TextureUV t, Normal n)
	{
		vertex = v;
		color = c;
		texture = t;
		normal = n;
	}
	void ChangeNormal(float a, float b, float c)
	{
		normal.ChangeNormal(a,b,c);
	}
	glm::vec3 GetVertex()
	{
		return vertex.GetVertex();
	}
	VertexPosition vertex;
	Color color;
	TextureUV texture;
	Normal normal;
private:
	//VertexPosition vertex;
	//Color color;
	//TextureUV texture;
	//Normal normal;
};

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh
{
public:
	Mesh();
	Mesh(std::vector<Vertex> vertices, std::vector<int> indices,std::vector<Texture> textures);
	void Init(std::vector<Vertex> vertices, std::vector<int> indices);
	Mesh(VertexPosition * vertices, unsigned int numVertices);
	Mesh(std::vector<Vertex> vertices);
	void Init(std::vector<Vertex> vertices);
	void Draw(bool isEleMents);
	void Draw(Shader shader);

	~Mesh();

	std::vector<Vertex> vertices;
	std::vector<int> indices;
	std::vector<Texture> textures;

private:
	enum
	{
		POSITION_VB,
		NUM_BUFFERS
	};
	GLuint m_vertexArrayObject;					//VAO,顶点数组对象
	GLuint m_vertexArrayBuffers[NUM_BUFFERS];	//VBO,顶点缓存对象
	GLuint m_elementBufferObject;				//EBO,索引缓存对象
	unsigned int m_drawCount;


};


