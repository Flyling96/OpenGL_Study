#pragma once
#include<iostream>
#include<string>
#include<GLFW/glfw3.h>
#include<glad/glad.h>
#include"stb_image.h"

class Shader
{
public:
	GLuint m_program;
	Shader(const std::string & fileName);
	void BindUniform();
	void Use(); //使用着色器
	void BindTexture(int textureId, int texture, std::string textureName, int value); //绑定纹理信息
	void LoadTexture(std::string imagePath, unsigned int &texture);//加载纹理
	~Shader();
private:
	static const unsigned int NUM_SHADERS = 2;
	GLuint m_shader[NUM_SHADERS];
};

