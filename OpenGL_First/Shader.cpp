#include <fstream>
#include "Shader.h"

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
	m_program = glCreateProgram();

	m_shader[0] = CreateShader(LoadShader(fileName + ".vs"), GL_VERTEX_SHADER);
	m_shader[1] = CreateShader(LoadShader(fileName + ".fs"), GL_FRAGMENT_SHADER);

	for (unsigned int i = 0; i < NUM_SHADERS; i++)
	{
		glAttachShader(m_program, m_shader[i]);
	}

	glLinkProgram(m_program);//将多个着色器链接成一个着色器程序
	CheckShaderError(m_program, GL_LINK_STATUS, true, "Error: Program Linking invalid");

	glValidateProgram(m_program);//验证program
	CheckShaderError(m_program, GL_VALIDATE_STATUS, true, "Error:Program Validate invalid");

}

void Shader::Bind()
{
	glUseProgram(m_program);	//使用着色器程序
}

void Shader::BindUniform()
{
	// 更新uniform颜色
	float timeValue = glfwGetTime();
	float greenValue = sin(timeValue) / 2.0f + 0.5f;
	int vertexColorLocation = glGetUniformLocation(m_program, "ourColor");
	glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
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
