#pragma once
#include<iostream>
#include<string>
#include<GLFW/glfw3.h>
#include<glad/glad.h>
#include"stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	GLuint m_program;
	Shader(const std::string & fileName);
	void BindUniform();
	void BindTransform(float screenWidth, float screenHeight);
	void Use(); //使用着色器
	void BindTexture(int textureId, int texture, std::string textureName, int value); //绑定纹理信息
	void LoadTexture(std::string imagePath, unsigned int &texture);//加载纹理
	void SetMat4(char* name, glm::mat4 mat4);
	~Shader();
private:
	static const unsigned int NUM_SHADERS = 2;
	GLuint m_shader[NUM_SHADERS];
};

