#include<GLFW/glfw3.h>
#include<glad/glad.h>

#include<iostream>
#include<fstream>

#include"common/Shader.h"
#include"common/Mesh.h"
#include"common/Windows.h"
#include"common/Camera.h"
#include"common/Model.h"

#define CRTDBG_MAP_ALLOC    //注意顺序不能乱
#include<stdlib.h>    
#include<crtdbg.h> 

#include<vector>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void CalculateVertexNormal(std::vector<Vertex> vertices, std::vector<int> indices);
glm::vec3 CalculateSurfaceNormal(glm::vec3 a, glm::vec3 b, glm::vec3 c);
void DrawCube(Mesh& mesh, Shader& shader, Camera& camera, glm::mat4 model, bool isReuseVertex);



VertexPosition vertices[] =
{
	VertexPosition(-0.5,-0.5,0),
	VertexPosition(0,0.5,0),
	VertexPosition(0.5,-0.5,0)
};

//不重用顶点
std::vector<Vertex> vertexDatas =
{
	Vertex(VertexPosition(1.0f, 1.0f, 1.0f),Color(1,1,0,1),TextureUV(1,1),Normal(0,0,1)),
	Vertex(VertexPosition(1.0f, -1.0f,1.0f),Color(1,0,1,1),TextureUV(1,0),Normal(0,0,1)),
	Vertex(VertexPosition(-1.0f, 1.0f, 1.0f),Color(0,1,1,1),TextureUV(0,1),Normal(0,0,1)),
	Vertex(VertexPosition(1.0f, -1.0f,1.0f),Color(1,0,1,1),TextureUV(1,0),Normal(0,0,1)),
	Vertex(VertexPosition(-1.0f, -1.0f, 1.0f),Color(1,1,1,1),TextureUV(0,0),Normal(0,0,1)),
	Vertex(VertexPosition(-1.0f, 1.0f, 1.0f),Color(0,1,1,1),TextureUV(0,1),Normal(0,0,1)),

	Vertex(VertexPosition(-1.0f, 1.0f, 1.0f),Color(0,1,1,1),TextureUV(1,1),Normal(-1,0,0)),
	Vertex(VertexPosition(-1.0f, -1.0f, 1.0f),Color(1,1,1,1),TextureUV(1,0),Normal(-1,0,0)),
	Vertex(VertexPosition(-1.0f, 1.0f, -1.0f),Color(0,1,1,1),TextureUV(0,1),Normal(-1,0,0)),
	Vertex(VertexPosition(-1.0f, -1.0f, 1.0f),Color(1,1,1,1),TextureUV(1,0),Normal(-1,0,0)),
	Vertex(VertexPosition(-1.0f, -1.0f, -1.0f),Color(1,1,1,1),TextureUV(0,0),Normal(-1,0,0)),
	Vertex(VertexPosition(-1.0f, 1.0f, -1.0f),Color(0,1,1,1),TextureUV(0,1),Normal(-1,0,0)),

	Vertex(VertexPosition(1.0f, 1.0f, -1.0f),Color(1,1,0,1),TextureUV(1,1),Normal(1,0,0)),
	Vertex(VertexPosition(1.0f, -1.0f, -1.0f),Color(1,0,1,1),TextureUV(1,0),Normal(1,0,0)),
	Vertex(VertexPosition(1.0f, 1.0f, 1.0f),Color(1,1,0,1),TextureUV(0,1),Normal(1,0,0)),
	Vertex(VertexPosition(1.0f, -1.0f, -1.0f),Color(1,0,1,1),TextureUV(1,0),Normal(1,0,0)),
	Vertex(VertexPosition(1.0f, -1.0f,1.0f),Color(1,0,1,1),TextureUV(0,0),Normal(1,0,0)),
	Vertex(VertexPosition(1.0f, 1.0f, 1.0f),Color(1,1,0,1),TextureUV(0,1),Normal(1,0,0)),

	Vertex(VertexPosition(1.0f, 1.0f, -1.0f),Color(1,1,0,1),TextureUV(1,1),Normal(0,1,0)),
	Vertex(VertexPosition(1.0f, 1.0f, 1.0f),Color(1,1,0,1),TextureUV(1,0),Normal(0,1,0)),
	Vertex(VertexPosition(-1.0f, 1.0f, -1.0f),Color(0,1,1,1),TextureUV(0,1),Normal(0,1,0)),
	Vertex(VertexPosition(1.0f, 1.0f, 1.0f),Color(1,1,0,1),TextureUV(1,0),Normal(0,1,0)),
	Vertex(VertexPosition(-1.0f, 1.0f, 1.0f),Color(0,1,1,1),TextureUV(0,0),Normal(0,1,0)),
	Vertex(VertexPosition(-1.0f, 1.0f, -1.0f),Color(0,1,1,1),TextureUV(0,1),Normal(0,1,0)),

	Vertex(VertexPosition(1.0f, -1.0f,1.0f),Color(1,0,1,1),TextureUV(1,1),Normal(0,-1,0)),
	Vertex(VertexPosition(1.0f, -1.0f, -1.0f),Color(1,0,1,1),TextureUV(1,0),Normal(0,-1,0)),
	Vertex(VertexPosition(-1.0f, -1.0f, 1.0f),Color(1,1,1,1),TextureUV(0,1),Normal(0,-1,0)),
	Vertex(VertexPosition(1.0f, -1.0f, -1.0f),Color(1,0,1,1),TextureUV(1,0),Normal(0,-1,0)),
	Vertex(VertexPosition(-1.0f, -1.0f, -1.0f),Color(1,1,1,1),TextureUV(0,0),Normal(0,-1,0)),
	Vertex(VertexPosition(-1.0f, -1.0f, 1.0f),Color(1,1,1,1),TextureUV(0,1),Normal(0,-1,0)),

	Vertex(VertexPosition(-1.0f, 1.0f, -1.0f),Color(0,1,1,1),TextureUV(1,1),Normal(0,0,-1)),
	Vertex(VertexPosition(-1.0f, -1.0f, -1.0f),Color(1,1,1,1),TextureUV(1,0),Normal(0,0,-1)),
	Vertex(VertexPosition(1.0f, 1.0f, -1.0f),Color(1,1,0,1),TextureUV(0,1),Normal(0,0,-1)),
	Vertex(VertexPosition(-1.0f, -1.0f, -1.0f),Color(1,1,1,1),TextureUV(1,0),Normal(0,0,-1)),
	Vertex(VertexPosition(1.0f, -1.0f, -1.0f),Color(1,0,1,1),TextureUV(0,0),Normal(0,0,-1)),
	Vertex(VertexPosition(1.0f, 1.0f, -1.0f),Color(1,1,0,1),TextureUV(0,1),Normal(0,0,-1)),
};

//重用顶点
std::vector<Vertex> vertexDatas2 =
{
	Vertex(VertexPosition(1.0f, 1.0f, 1.0f),Color(1,1,0,1),TextureUV(1,1)),
	Vertex(VertexPosition(1.0f, -1.0f,1.0f),Color(1,0,1,1),TextureUV(1,0)),
	Vertex(VertexPosition(-1.0f, -1.0f, 1.0f),Color(1,1,1,1),TextureUV(0,0)),
	Vertex(VertexPosition(-1.0f, 1.0f, 1.0f),Color(0,1,1,1),TextureUV(0,1)),
	Vertex(VertexPosition(1.0f, 1.0f, -1.0f),Color(1,1,0,1),TextureUV(1,1)),
	Vertex(VertexPosition(1.0f, -1.0f, -1.0f),Color(1,0,1,1),TextureUV(1,0)),
	Vertex(VertexPosition(-1.0f, -1.0f, -1.0f),Color(1,1,1,1),TextureUV(0,0)),
	Vertex(VertexPosition(-1.0f, 1.0f, -1.0f),Color(0,1,1,1),TextureUV(0,1)),

};

std::vector<int> indices = { // 注意索引从0开始! 
	0, 1, 3, 
	1, 2, 3,  

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
	bool isModel = true;  //是否加载模型
	bool isReuseVertex = false;//是否重用顶点
	Windows window(800, 600, "LearnOpenGL");
	Mesh mesh;
	Mesh lightCubeMesh;

	if (isReuseVertex)
	{
		CalculateVertexNormal(vertexDatas2, indices);
		mesh.Init(vertexDatas2, indices);
		lightCubeMesh.Init(vertexDatas2, indices);
	}
	else
	{
		mesh.Init(vertexDatas);
		lightCubeMesh.Init(vertexDatas2, indices);
	}

	//Mesh mesh(vertices, sizeof(vertices) / sizeof(vertices[0]));
	//Mesh mesh(vertexDatas, sizeof(vertexDatas) / sizeof(vertexDatas[0]));
	Shader shader;
	Shader lightShader;
	Model model;

	if (!isModel)
	{
		shader.Init("../res/shader/cubeShader");
		lightShader.Init("../res/shader/lightShader");
		unsigned int diffuseMap, specularMap;
		diffuseMap = shader.LoadTexture("../res/DiffuseMap.png");
		specularMap = shader.LoadTexture("../res/SpecularMap.png");

		shader.Use();
		shader.BindTexture(GL_TEXTURE0, diffuseMap, "material.diffuse", 0);
		shader.BindTexture(GL_TEXTURE1, specularMap, "material.specular", 1);
	}
	else
	{
		shader.Init("../res/shader/modelShader");
		lightShader.Init("../res/shader/lightShader");
		model.Init("../res/model/nanosuit/nanosuit.obj");
	}
	Camera camera(window.GetWindow());

	glEnable(GL_DEPTH_TEST);

	float deltaTime = 0.0f; // 当前帧与上一帧的时间差
	float lastFrame = 0.0f; // 上一帧的时间

	glm::vec3 lightPos(5.0f, 0.0f, 0.0f);
	glm::vec3 lightDir(-0.2f, -1.0f, -0.3f);
	glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};
	

	//主循环
	while (!window.IsClosed())
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		window.Clear(0.2f, 0.3f, 0.3f, 1.0f);


		camera.ProcessInput(window.GetWindow(), deltaTime);
		camera.UpdateFront();

#pragma region 光源信息
		shader.Use();

		shader.setVec3("viewPos", camera.cameraPos);
		// directional light
		shader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		shader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		shader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		shader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
		// point light 1
		shader.setVec3("pointLights[0].position", pointLightPositions[0]);
		shader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
		shader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
		shader.setVec3("pointLights[0].specular", 10.0f, 10.0f, 10.0f);
		shader.setFloat("pointLights[0].constant", 1.0f);
		shader.setFloat("pointLights[0].linear", 0.09);
		shader.setFloat("pointLights[0].quadratic", 0.032);
		shader.setFloat("material.shininess", 16.0f);
		// point light 2

#pragma endregion


		if (!isModel)
		{
#pragma region 自定义Mesh
			shader.Use();
			shader.setVec3("material.ambient", 0.25f, 0.4f, 0.8f);
			shader.setVec3("material.diffuse", 0.5f, 0.5f, 0.5f);
			shader.setVec3("material.specular", 0.8f, 0.8f, 0.8f);
			glm::mat4 model;
			for (int i = 0; i < 10; i++)
			{
				glm::mat4 model;
				model = glm::translate(model, glm::vec3(3, 3, 3) * cubePositions[i]);
				float angle = 20.0f * i;
				model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
				DrawCube(mesh, shader, camera, model, isReuseVertex);
			}
			lightShader.Use();
			lightShader.setVec3("lightColor", lightColor);
			for (int i = 0; i < 4; i++)
			{
				glm::mat4 lightModel;
				lightModel = glm::translate(lightModel, pointLightPositions[i]);
				DrawCube(lightCubeMesh, lightShader, camera, lightModel, true);
			}
#pragma endregion
		}
		else
		{
#pragma region 读取模型


			glm::mat4 projection;
			projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);
			glm::mat4 view = camera.Update();
			shader.setMat4("view", view);
			shader.setMat4("projection", projection);

			glm::mat4 modelMat;
			modelMat = glm::translate(modelMat, glm::vec3(0.0f, -1.75f, 0.0f));
			modelMat = glm::scale(modelMat, glm::vec3(0.2f, 0.2f, 0.2f));
			shader.setMat4("model", modelMat);

			model.Draw(shader);

			lightShader.Use();
			lightShader.setVec3("lightColor", lightColor);
			for (int i = 0; i < 1; i++)
			{
				glm::mat4 lightModel;
				lightModel = glm::translate(lightModel, pointLightPositions[i]);
				DrawCube(lightCubeMesh, lightShader, camera, lightModel, true);
			}

			

#pragma endregion
		}

		//mesh.Draw(true);

		window.Update();

	}

	glfwTerminate();
	return 0;
}

void DrawCube(Mesh& mesh,Shader& shader,Camera& camera, glm::mat4 model,bool isReuseVertex)
{
	shader.BindTransform(800, 600);
	shader.setMat4("view", camera.Update());
	shader.setMat4("model", model);
	mesh.Draw(isReuseVertex);
}

void CalculateVertexNormal(std::vector<Vertex> vertices, std::vector<int> indices)
{
	std::vector<glm::vec3> vertexNormalArray;	//顶点法向量数组
	for (int i = 0; i < vertices.size(); i++)
	{
		vertexNormalArray.push_back(glm::vec3(0, 0, 0));
	}

	for (int i = 0; i < indices.size(); i += 6)
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

	for (int i = 0; i < vertices.size(); i++)
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