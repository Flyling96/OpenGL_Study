#pragma once
#include<iostream>
#include<string>
#include<GLFW/glfw3.h>
#include<glad/glad.h>
#include"stb_image.h"

class Shader
{
public:
	Shader(const std::string & fileName);
	void BindUniform();
	void Bind(); //绑定着色器
	void LoadTexture(std::string imagePath);//加载纹理
	~Shader();
private:
	static const unsigned int NUM_SHADERS = 2;
	GLuint m_program;
	GLuint m_shader[NUM_SHADERS];
};

