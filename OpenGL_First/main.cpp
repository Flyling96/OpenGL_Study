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
void CalculateVertexNormal(VertexData * vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices);
glm::vec3 CalculateSurfaceNormal(glm::vec3 a, glm::vec3 b, glm::vec3 c);
void DrawCube(Mesh& mesh, Shader& shader, Camera& camera, glm::mat4 model);



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
	VertexData(Vertex(-1.0f, 1.0f, -1.0f),Color(0,1,1,1),TextureDate(0,1)),

};

unsigned int indices[] = { // 注意索引从0开始! 
	0, 1, 3, // 第一个三角形
	1, 2, 3,  // 第二个三角形

	3,2,7,
	2,6,7,

	4,5,0,
	5,1,0,

	4,0,7,
	0,3,7,

	1,5,2,
	5,6,2,

    7,6,4,
	6,5,4,

};

glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(3.0f,  0.0f, 0.0f),
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

	CalculateVertexNormal(vertexDatas2, sizeof(vertexDatas2) / sizeof(vertexDatas2[0]), indices, sizeof(indices) / sizeof(indices[0]));

	Windows window(800,600,"LearnOpenGL");

	Mesh mesh(vertexDatas2, sizeof(vertexDatas2) / sizeof(vertexDatas2[0]), indices, sizeof(indices) / sizeof(indices[0]));
	Mesh lightCubeMesh(vertexDatas2, sizeof(vertexDatas2) / sizeof(vertexDatas2[0]), indices, sizeof(indices) / sizeof(indices[0]));
	//Mesh mesh(vertices, sizeof(vertices) / sizeof(vertices[0]));
	//Mesh mesh(vertexDatas, sizeof(vertexDatas) / sizeof(vertexDatas[0]));


	Shader shader("./res/cubeShader");
	Shader lightShader("./res/lightShader");

	Camera camera(window.GetWindow());

	unsigned int texture1, texture2;
	shader.LoadTexture("Penguins.jpg",texture1);
	shader.LoadTexture("Lighthouse.jpg",texture2);


	//shader.BindTexture(GL_TEXTURE0, texture1, "texture1", 0);
	//shader.BindTexture(GL_TEXTURE1, texture2, "texture2", 1);

	glEnable(GL_DEPTH_TEST);

	float deltaTime = 0.0f; // 当前帧与上一帧的时间差
	float lastFrame = 0.0f; // 上一帧的时间

	glm::vec3 lightPos(5.0f, 0.0f, 0.0f);
	//主循环
	while (!window.IsClosed())
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		window.Clear(0.2f, 0.3f, 0.3f, 1.0f);


		camera.ProcessInput(window.GetWindow(), deltaTime);
		camera.UpdateFront();

		shader.Use();
		shader.setVec3("objectColor", 0.25f, 0.4f, 0.8f);
		shader.setVec3("lightColor", 1.0f,1.0f,1.0f);
		shader.setVec3("lightPos", lightPos);
		shader.setVec3("viewPos", camera.cameraPos);
		glm::mat4 model;
		DrawCube(mesh, shader, camera, model);

		lightShader.Use();
		glm::mat4 lightModel;
		lightModel = glm::translate(lightModel, lightPos);
		DrawCube(lightCubeMesh, lightShader, camera, lightModel);

		//mesh.Draw(true);

		window.Update();

	}

	glfwTerminate();
	return 0;
}

void DrawCube(Mesh& mesh,Shader& shader,Camera& camera, glm::mat4 model)
{
	shader.BindTransform(800, 600);
	shader.setMat4("view", camera.Update());
	shader.setMat4("model", model);
	mesh.Draw(true);
}

void CalculateVertexNormal(VertexData * vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices)
{
	std::vector<glm::vec3> vertexNormalArray;	//顶点法向量数组
	for (int i = 0; i < numVertices; i++)
	{
		vertexNormalArray.push_back(glm::vec3(0, 0, 0));
	}

	for (int i = 0; i < numIndices; i += 6)
	{
		glm::vec3 temp = CalculateSurfaceNormal(vertices[indices[i]].GetVertex(), vertices[indices[i + 1]].GetVertex(), vertices[indices[i + 2]].GetVertex());
		vertexNormalArray[indices[i]] += temp;
		vertexNormalArray[indices[i + 1]] += temp;
		vertexNormalArray[indices[i + 2]] += temp;
		for (int j = 3; j < 6; j++)
		{
			if (indices[i + j] != indices[i] && indices[i + j] != indices[i + 1] && indices[i + j] != indices[i + 2])
			{
				vertexNormalArray[indices[i + j]] += temp;
			}
		}
	}

	for (int i = 0; i < vertexNormalArray.size(); i++)
	{
		//归一化
		vertexNormalArray[i] = glm::normalize(vertexNormalArray[i]);
	}

	for (int i = 0; i < numVertices; i++)
	{
		vertices[i].ChangeNormal(vertexNormalArray[i].x, vertexNormalArray[i].y, vertexNormalArray[i].z);
	}


}

glm::vec3 CalculateSurfaceNormal(glm::vec3 a, glm::vec3 b, glm::vec3 c)
{
	glm::vec3 v1 = b - a;
	glm::vec3 v2 = b - c;
	return glm::cross(v1, v2);
}