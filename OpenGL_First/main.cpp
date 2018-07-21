#include<GLFW/glfw3.h>
#include<glad/glad.h>
#include<iostream>
#include"Shader.h"
#include"Mesh.h"
#include"Windows.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);

//float vertices[] = {
//	-0.5f, -0.5f, 0.0f,
//	0.5f, -0.5f, 0.0f,
//	0.0f,  0.5f, 0.0f
//};


Vertex vertices[] =
{
	Vertex(-0.5,-0.5,0),
	Vertex(0,0.5,0),
	Vertex(0.5,-0.5,0)
};

int main()
{

	Windows window(800,600,"LearnOpenGL");

	Mesh mesh(vertices, sizeof(vertices) / sizeof(vertices[0]));

	Shader shader("./res/basicShader");

	//Ö÷Ñ­»·
	while (!window.IsClosed())
	{
		window.Chear(0.2f, 0.3f, 0.3f, 1.0f);

		shader.Bind();

		mesh.Draw();

		window.Update();
	}

	glfwTerminate();

	return 0;
}
