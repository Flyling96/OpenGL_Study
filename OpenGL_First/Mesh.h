#pragma once
#include<GLFW/glfw3.h>
#include<glad/glad.h>
#include<glm/glm.hpp>
#include<vector>

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

class Vertex
{
public:
	Vertex()
	{
		posX = 0, posY = 0,posZ = 0;
	}
	Vertex(float x,float y,float z)
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

class TextureDate
{

public:
	TextureDate()
	{
		u = 0, v = 0;
	}
	TextureDate(float a, float b)
	{
		u = a;
		v = b;
	}

private:
	float u;
	float v;

};

class VertexData
{
public:
	VertexData(Vertex v, Color c, TextureDate t)
	{
		vertex = v;
		color = c;
		texture = t;
		normal = Normal(0, 0, 0);
	}

	VertexData(Vertex v, Color c, TextureDate t, Normal n)
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

private:
	Vertex vertex;
	Color color;
	TextureDate texture;
	Normal normal;
};


class Mesh
{
public:
	Mesh();
	Mesh(VertexData * vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices);
	void Init(VertexData * vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices);
	Mesh(Vertex * vertices, unsigned int numVertices);
	Mesh(VertexData * vertices, unsigned int numVertexData);
	void Init(VertexData * vertices, unsigned int numVertexData);
	void Draw(bool isEleMents);

	~Mesh();


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


