#pragma once
#include<GLFW/glfw3.h>
#include<glad/glad.h>

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

class VertexData
{
public:
	VertexData(Vertex v, Color c)
	{
		vertex = v;
		color = c;
	}
private:
	Vertex vertex;
	Color color;
};


class Mesh
{
public:
	Mesh(Vertex * vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices);
	Mesh(Vertex * vertices, unsigned int numVertices);
	Mesh(VertexData * vertices, unsigned int numVertexData);
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


