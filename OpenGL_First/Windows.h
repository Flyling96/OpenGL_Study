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

