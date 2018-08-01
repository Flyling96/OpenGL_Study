#include<GLFW/glfw3.h>
#include<glad/glad.h>
#include<iostream>
#include"Shader.h"
#include"Mesh.h"
#include"Windows.h"

#define CRTDBG_MAP_ALLOC    //注意顺序不能乱
#include <stdlib.h>    
#include <crtdbg.h> 


void framebuffer_size_callback(GLFWwindow* window, int width, int height);


Vertex vertices[] =
{
	Vertex(-0.5,-0.5,0),
	Vertex(0,0.5,0),
	Vertex(0.5,-0.5,0)
};

VertexData vertexDatas[] =
{
	VertexData(Vertex(-0.5,-0.5,0),Color(1,0,0,1),TextureDate(0,1)),
	VertexData(Vertex(0,0.5,0),Color(0,1,0,1),TextureDate(0.5,0)),
	VertexData(Vertex(0.5,-0.5,0),Color(0,0,1,1),TextureDate(1,1))
};

VertexData vertexDatas2[] =
{
	VertexData(Vertex(1.0f, 1.0f, 0.0f),Color(1,1,0,1),TextureDate(1,0)),
	VertexData(Vertex(1.0f, -1.0f, 0.0f),Color(1,0,1,1),TextureDate(1,1)),
	VertexData(Vertex(-1.0f, -1.0f, 0.0f),Color(1,1,1,1),TextureDate(0,1)),
	VertexData(Vertex(-1.0f, 1.0f, 0.0f),Color(0,1,1,1),TextureDate(0,0))
};

unsigned int indices[] = { // 注意索引从0开始! 
	0, 1, 3, // 第一个三角形
	1, 2, 3  // 第二个三角形
};

int main()
{


	Windows window(800,600,"LearnOpenGL");

	Mesh mesh(vertexDatas2, sizeof(vertexDatas2) / sizeof(vertexDatas2[0]), indices, sizeof(indices) / sizeof(indices[0]));
	//Mesh mesh(vertices, sizeof(vertices) / sizeof(vertices[0]));
	//Mesh mesh(vertexDatas, sizeof(vertexDatas) / sizeof(vertexDatas[0]));

	Shader shader("./res/basicShader");

	unsigned int texture1, texture2;
	shader.LoadTexture("Penguins.jpg",texture1);
	shader.LoadTexture("Lighthouse.jpg",texture2);
	shader.Use();

	shader.BindTexture(GL_TEXTURE0, texture1, "texture1", 0);
	shader.BindTexture(GL_TEXTURE1, texture2, "texture2", 1);


	//主循环
	while (!window.IsClosed())
	{
		window.Clear(0.2f, 0.3f, 0.3f, 1.0f);

		shader.BindUniform();

		mesh.Draw(true);

		window.Update();

	}

	glfwTerminate();
	return 0;
}
