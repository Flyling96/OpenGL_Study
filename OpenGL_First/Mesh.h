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
	Mesh(Vertex * vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices);
	Mesh(Vertex * vertices, unsigned int numVertices);
	void Draw(bool isEleMents);
	~Mesh();
private:
	enum
	{
		POSITION_VB,
		NUM_BUFFERS
	};
	GLuint m_vertexArrayObject;					//VAO,�����������
	GLuint m_vertexArrayBuffers[NUM_BUFFERS];	//VBO,���㻺�����
	GLuint m_elementBufferObject;				//EBO,�����������
	unsigned int m_drawCount;

};


