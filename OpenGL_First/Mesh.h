#pragma once
#include<GLFW/glfw3.h>
#include<glad/glad.h>

class Vertex
{
public:
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

class Mesh
{
public:
	Mesh(Vertex *vertices, unsigned int numVertices);
	void Draw();
	~Mesh();
private:
	enum
	{
		POSITION_VB,
		NUM_BUFFERS
	};
	GLuint m_vertexArrayObject;
	GLuint m_vertexArrayBuffers[NUM_BUFFERS];
	unsigned int m_drawCount;

};


