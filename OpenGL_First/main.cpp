#include<GLFW/glfw3.h>
#include<glad/glad.h>
#include<iostream>
#include"Shader.h"
#include"Mesh.h"
#include"Windows.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);


Vertex vertices[] =
{
	Vertex(-0.5,-0.5,0),
	Vertex(0,0.5,0),
	Vertex(0.5,-0.5,0)
};

VertexData vertexDatas[] =
{
	VertexData(Vertex(-0.5,-0.5,0),Color(1,0,0,1)),
	VertexData(Vertex(0,0.5,0),Color(0,1,0,1)),
	VertexData(Vertex(0.5,-0.5,0),Color(0,0,1,1))
};

Vertex vertices2[] =
{
	Vertex(0.5f, 0.5f, 0.0f),
	Vertex(0.5f, -0.5f, 0.0f),
	Vertex(-0.5f, -0.5f, 0.0f),
	Vertex(-0.5f, 0.5f, 0.0f)
};

unsigned int indices[] = { // ע��������0��ʼ! 
	0, 1, 3, // ��һ��������
	1, 2, 3  // �ڶ���������
};

int main()
{


	Windows window(800,600,"LearnOpenGL");

	//Mesh mesh(vertices2, sizeof(vertices2) / sizeof(vertices2[0]), indices, sizeof(indices) / sizeof(indices[0]));
	//Mesh mesh(vertices, sizeof(vertices) / sizeof(vertices[0]));
	Mesh mesh(vertexDatas, sizeof(vertexDatas) / sizeof(vertexDatas[0]));

	Shader shader("./res/basicShader");

	//��ѭ��
	while (!window.IsClosed())
	{
		window.Clear(0.2f, 0.3f, 0.3f, 1.0f);

		shader.Bind();
		shader.BindUniform();

		mesh.Draw(false);

		window.Update();
	}

	glfwTerminate();

	return 0;
}
