#pragma once
#include<iostream>
#include<string>
#include<GLFW/glfw3.h>
#include<glad/glad.h>
#include"common/stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	GLuint m_program;
	Shader() {};
	Shader(const std::string & fileName);
	void Init(const std::string & fileName);
	void BindUniform();
	void BindTransform(float screenWidth, float screenHeight);
	void Use(); //使用着色器
	void BindTexture(int textureId, int texture, std::string textureName, int value); //绑定纹理信息
	unsigned int Shader::LoadTexture(std::string imagePath);//加载纹理
	~Shader();

	void setBool(const std::string &name, bool value) const
	{
		glUniform1i(glGetUniformLocation(m_program, name.c_str()), (int)value);
	}
	// ------------------------------------------------------------------------
	void setInt(const std::string &name, int value) const
	{
		glUniform1i(glGetUniformLocation(m_program, name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	void setFloat(const std::string &name, float value) const
	{
		glUniform1f(glGetUniformLocation(m_program, name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	void setVec2(const std::string &name, const glm::vec2 &value) const
	{
		glUniform2fv(glGetUniformLocation(m_program, name.c_str()), 1, &value[0]);
	}
	void setVec2(const std::string &name, float x, float y) const
	{
		glUniform2f(glGetUniformLocation(m_program, name.c_str()), x, y);
	}
	// ------------------------------------------------------------------------
	void setVec3(const std::string &name, const glm::vec3 &value) const
	{
		glUniform3fv(glGetUniformLocation(m_program, name.c_str()), 1, &value[0]);
	}
	void setVec3(const std::string &name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(m_program, name.c_str()), x, y, z);
	}
	// ------------------------------------------------------------------------
	void setVec4(const std::string &name, const glm::vec4 &value) const
	{
		glUniform4fv(glGetUniformLocation(m_program, name.c_str()), 1, &value[0]);
	}
	void setVec4(const std::string &name, float x, float y, float z, float w)
	{
		glUniform4f(glGetUniformLocation(m_program, name.c_str()), x, y, z, w);
	}
	// ------------------------------------------------------------------------
	void setMat2(const std::string &name, const glm::mat2 &mat) const
	{
		glUniformMatrix2fv(glGetUniformLocation(m_program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	void setMat3(const std::string &name, const glm::mat3 &mat) const
	{
		glUniformMatrix3fv(glGetUniformLocation(m_program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	void setMat4(const std::string &name, const glm::mat4 &mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(m_program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
private:
	static const unsigned int NUM_SHADERS = 2;
	GLuint m_shader[NUM_SHADERS];
};

static GLuint CreateShader(const std::string& text, GLenum shaderType);
static void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
//加载着色器文件并返回着色器的所有内容
static std::string LoadShader(const std::string & fileName);


static GLuint CreateShader(const std::string& text, GLenum shaderType)
{
	GLuint shader = glCreateShader(shaderType);

	if (shader == 0)
	{
		std::cerr << "Error:Shader create failture" << std::endl;
	}

	const GLchar * shaderSourceString[1];

	GLint shaderSourceStringLength[1];

	shaderSourceString[0] = text.c_str();
	shaderSourceStringLength[0] = text.length();

	glShaderSource(shader, 1, shaderSourceString, shaderSourceStringLength);
	glCompileShader(shader);

	CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error:Shader compilation failed");

	return shader;

}

static void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };
	if (isProgram)
	{
		glGetProgramiv(shader, flag, &success);
	}
	else
	{
		glGetShaderiv(shader, flag, &success);
	}

	if (success == GL_FALSE)
	{
		if (isProgram)
		{
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		}
		else
		{
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);
		}
		std::cerr << errorMessage << ":" << error << std::endl;
	}

}

static std::string LoadShader(const std::string & fileName)
{
	std::ifstream file;
	file.open((fileName).c_str());

	std::string output;
	std::string line;

	if (file.is_open())
	{
		while (file.good())
		{
			std::getline(file, line);
			output.append(line + "\n");
		}
	}
	else
	{
		std::cerr << "Unable to load shader:" << fileName << std::endl;
	}
	return output;
}


Shader::Shader(const std::string & fileName)
{
	Init(fileName);
}

void Shader::Init(const std::string & fileName)
{
	m_shader[0] = CreateShader(LoadShader(fileName + ".vs"), GL_VERTEX_SHADER);
	m_shader[1] = CreateShader(LoadShader(fileName + ".fs"), GL_FRAGMENT_SHADER);
	m_program = glCreateProgram();

	for (unsigned int i = 0; i < NUM_SHADERS; i++)
	{
		glAttachShader(m_program, m_shader[i]);
	}

	glLinkProgram(m_program);//将多个着色器链接成一个着色器程序
	CheckShaderError(m_program, GL_LINK_STATUS, true, "Error: Program Linking invalid");

	glValidateProgram(m_program);//验证program
	CheckShaderError(m_program, GL_VALIDATE_STATUS, true, "Error:Program Validate invalid");
}

void Shader::Use()
{
	glUseProgram(m_program);	//使用着色器程序
}

//传入shader矩阵相关
void Shader::BindTransform(float screenWidth, float screenHeight)
{

	//glm::mat4 model;
	//model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
	//unsigned int modelLocation = glGetUniformLocation(m_program, "model");
	//glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	//glm::mat4 view;
	//// 注意，我们将矩阵向我们要进行移动场景的反方向移动。
	//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	//unsigned int viewLocation = glGetUniformLocation(m_program, "view");
	//glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

	//float radius = 10.0f;
	//float camX = sin(glfwGetTime()) * radius;
	//float camZ = cos(glfwGetTime()) * radius;
	//glm::mat4 view;
	//view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	////LookAt(摄像机位置，目标位置，世界空间的上向量)
	//unsigned int viewLocation = glGetUniformLocation(m_program, "view");
	//glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), screenWidth / screenHeight, 0.1f, 100.0f);
	unsigned int projectionLocation = glGetUniformLocation(m_program, "projection");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
}



void Shader::BindUniform()
{
	// 更新uniform颜色
	float timeValue = glfwGetTime();
	float whileValue = sin(timeValue) / 2.0f + 0.5f;
	int vertexColorLocation = glGetUniformLocation(m_program, "ourColor");
	glUniform4f(vertexColorLocation, whileValue, whileValue, whileValue, 1.0f);
}

void Shader::BindTexture(int textureId, int texture, std::string textureName, int value)
{
	glActiveTexture(textureId);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(m_program, textureName.c_str()), value);
}

unsigned int Shader::LoadTexture(std::string imagePath)
{
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// 为当前绑定的纹理对象设置环绕、过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	// 加载并生成纹理
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(imagePath.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		GLenum format;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	return texture;
}

Shader::~Shader()
{
	for (unsigned int i = 0; i < NUM_SHADERS; i++)
	{
		glDetachShader(m_program, m_shader[i]);
		glDeleteShader(m_shader[i]);
	}
	glDeleteProgram(m_program);
}

