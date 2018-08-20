#pragma once
#include<string>
#include<GLFW/glfw3.h>
#include<glad/glad.h>
#include<iostream>


class Windows
{
public:
	Windows(int width, int height, const std::string& title);
	void Update();
	bool IsClosed();
	void Clear(float r, float g, float b, float a);
	~Windows();

	GLFWwindow* GetWindow();
private:
	GLFWwindow* m_window;

};

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

Windows::Windows(int width, int height, const std::string& title)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	m_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if (m_window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}

	glfwMakeContextCurrent(m_window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	//自适应窗口大小
	glViewport(0, 0, 800, 600);

	glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);

}

void Windows::Update()
{
	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(m_window, true);

	glfwSwapBuffers(m_window);
	glfwPollEvents();
}

bool Windows::IsClosed()
{
	return glfwWindowShouldClose(m_window);
}

void Windows::Clear(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清除上一帧的深度缓冲
}

Windows::~Windows()
{
	glfwDestroyWindow(m_window);
}

GLFWwindow* Windows::GetWindow()
{
	return m_window;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

