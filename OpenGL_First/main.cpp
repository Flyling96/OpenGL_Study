#include<GLFW/glfw3.h>
#include<glad/glad.h>
#include<iostream>
#include"Shader.h"
#include"Mesh.h"
#include"Windows.h"
#include"Camera.h"

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
	VertexData(Vertex(1.0f, 1.0f, 1.0f),Color(1,1,0,1),TextureDate(1,1)),
	VertexData(Vertex(1.0f, -1.0f,1.0f),Color(1,0,1,1),TextureDate(1,0)),
	VertexData(Vertex(-1.0f, -1.0f, 1.0f),Color(1,1,1,1),TextureDate(0,0)),
	VertexData(Vertex(-1.0f, 1.0f, 1.0f),Color(0,1,1,1),TextureDate(0,1)),
	VertexData(Vertex(1.0f, 1.0f, -1.0f),Color(1,1,0,1),TextureDate(1,1)),
	VertexData(Vertex(1.0f, -1.0f, -1.0f),Color(1,0,1,1),TextureDate(1,0)),
	VertexData(Vertex(-1.0f, -1.0f, -1.0f),Color(1,1,1,1),TextureDate(0,0)),
	VertexData(Vertex(-1.0f, 1.0f, -1.0f),Color(0,1,1,1),TextureDate(0,1))
};

unsigned int indices[] = { // 注意索引从0开始! 
	0, 1, 3, // 第一个三角形
	1, 2, 3,  // 第二个三角形

	7,6,3,
	6,2,3,

	0,1,4,
	1,5,4,

	7,3,4,
	3,0,4,

	6,2,5,
	2,1,5,

    4,5,7,
	5,6,7,

};

glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};

int main()
{

	Windows window(800,600,"LearnOpenGL");

	Mesh mesh(vertexDatas2, sizeof(vertexDatas2) / sizeof(vertexDatas2[0]), indices, sizeof(indices) / sizeof(indices[0]));
	//Mesh mesh(vertices, sizeof(vertices) / sizeof(vertices[0]));
	//Mesh mesh(vertexDatas, sizeof(vertexDatas) / sizeof(vertexDatas[0]));

	Shader shader("./res/basicShader");

	Camera camera(window.GetWindow());

	unsigned int texture1, texture2;
	shader.LoadTexture("Penguins.jpg",texture1);
	shader.LoadTexture("Lighthouse.jpg",texture2);

	shader.Use();

	shader.BindTexture(GL_TEXTURE0, texture1, "texture1", 0);
	shader.BindTexture(GL_TEXTURE1, texture2, "texture2", 1);

	glEnable(GL_DEPTH_TEST);

	float deltaTime = 0.0f; // 当前帧与上一帧的时间差
	float lastFrame = 0.0f; // 上一帧的时间

	//主循环
	while (!window.IsClosed())
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		window.Clear(0.2f, 0.3f, 0.3f, 1.0f);

		shader.BindTransform(800,600);
		shader.BindUniform();

		camera.ProcessInput(window.GetWindow(), deltaTime);
		camera.UpdateFront();

		shader.SetMat4("view",camera.Update());

		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * (i+1);
			model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			shader.SetMat4("model", model);
			mesh.Draw(true);

		}
		//mesh.Draw(true);

		window.Update();

	}

	glfwTerminate();
	return 0;
}
