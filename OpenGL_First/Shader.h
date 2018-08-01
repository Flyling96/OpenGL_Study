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
	void Use(); //ʹ����ɫ��
	void BindTexture(int textureId, int texture, std::string textureName, int value); //��������Ϣ
	void LoadTexture(std::string imagePath, unsigned int &texture);//��������
	~Shader();
private:
	static const unsigned int NUM_SHADERS = 2;
	GLuint m_shader[NUM_SHADERS];
};

