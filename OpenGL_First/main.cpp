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
void DrawCube(Mesh& mesh, Shader& shader, Camera& camera, glm::mat4 model, bool isReuseVertex);



Vertex vertices[] =
{
	Vertex(-0.5,-0.5,0),
	Vertex(0,0.5,0),
	Vertex(0.5,-0.5,0)
};

//不重用顶点
VertexData vertexDatas[] =
{
	VertexData(Vertex(1.0f, 1.0f, 1.0f),Color(1,1,0,1),TextureDate(1,1),Normal(0,0,1)),
	VertexData(Vertex(1.0f, -1.0f,1.0f),Color(1,0,1,1),TextureDate(1,0),Normal(0,0,1)),
	VertexData(Vertex(-1.0f, 1.0f, 1.0f),Color(0,1,1,1),TextureDate(0,1),Normal(0,0,1)),
	VertexData(Vertex(1.0f, -1.0f,1.0f),Color(1,0,1,1),TextureDate(1,0),Normal(0,0,1)),
	VertexData(Vertex(-1.0f, -1.0f, 1.0f),Color(1,1,1,1),TextureDate(0,0),Normal(0,0,1)),
	VertexData(Vertex(-1.0f, 1.0f, 1.0f),Color(0,1,1,1),TextureDate(0,1),Normal(0,0,1)),

	VertexData(Vertex(-1.0f, 1.0f, 1.0f),Color(0,1,1,1),TextureDate(1,1),Normal(-1,0,0)),
	VertexData(Vertex(-1.0f, -1.0f, 1.0f),Color(1,1,1,1),TextureDate(1,0),Normal(-1,0,0)),
	VertexData(Vertex(-1.0f, 1.0f, -1.0f),Color(0,1,1,1),TextureDate(0,1),Normal(-1,0,0)),
	VertexData(Vertex(-1.0f, -1.0f, 1.0f),Color(1,1,1,1),TextureDate(1,0),Normal(-1,0,0)),
	VertexData(Vertex(-1.0f, -1.0f, -1.0f),Color(1,1,1,1),TextureDate(0,0),Normal(-1,0,0)),
	VertexData(Vertex(-1.0f, 1.0f, -1.0f),Color(0,1,1,1),TextureDate(0,1),Normal(-1,0,0)),

	VertexData(Vertex(1.0f, 1.0f, -1.0f),Color(1,1,0,1),TextureDate(1,1),Normal(1,0,0)),
	VertexData(Vertex(1.0f, -1.0f, -1.0f),Color(1,0,1,1),TextureDate(1,0),Normal(1,0,0)),
	VertexData(Vertex(1.0f, 1.0f, 1.0f),Color(1,1,0,1),TextureDate(0,1),Normal(1,0,0)),
	VertexData(Vertex(1.0f, -1.0f, -1.0f),Color(1,0,1,1),TextureDate(1,0),Normal(1,0,0)),
	VertexData(Vertex(1.0f, -1.0f,1.0f),Color(1,0,1,1),TextureDate(0,0),Normal(1,0,0)),
	VertexData(Vertex(1.0f, 1.0f, 1.0f),Color(1,1,0,1),TextureDate(0,1),Normal(1,0,0)),

	VertexData(Vertex(1.0f, 1.0f, -1.0f),Color(1,1,0,1),TextureDate(1,1),Normal(0,1,0)),
	VertexData(Vertex(1.0f, 1.0f, 1.0f),Color(1,1,0,1),TextureDate(1,0),Normal(0,1,0)),
	VertexData(Vertex(-1.0f, 1.0f, -1.0f),Color(0,1,1,1),TextureDate(0,1),Normal(0,1,0)),
	VertexData(Vertex(1.0f, 1.0f, 1.0f),Color(1,1,0,1),TextureDate(1,0),Normal(0,1,0)),
	VertexData(Vertex(-1.0f, 1.0f, 1.0f),Color(0,1,1,1),TextureDate(0,0),Normal(0,1,0)),
	VertexData(Vertex(-1.0f, 1.0f, -1.0f),Color(0,1,1,1),TextureDate(0,1),Normal(0,1,0)),

	VertexData(Vertex(1.0f, -1.0f,1.0f),Color(1,0,1,1),TextureDate(1,1),Normal(0,-1,0)),
	VertexData(Vertex(1.0f, -1.0f, -1.0f),Color(1,0,1,1),TextureDate(1,0),Normal(0,-1,0)),
	VertexData(Vertex(-1.0f, -1.0f, 1.0f),Color(1,1,1,1),TextureDate(0,1),Normal(0,-1,0)),
	VertexData(Vertex(1.0f, -1.0f, -1.0f),Color(1,0,1,1),TextureDate(1,0),Normal(0,-1,0)),
	VertexData(Vertex(-1.0f, -1.0f, -1.0f),Color(1,1,1,1),TextureDate(0,0),Normal(0,-1,0)),
	VertexData(Vertex(-1.0f, -1.0f, 1.0f),Color(1,1,1,1),TextureDate(0,1),Normal(0,-1,0)),

	VertexData(Vertex(-1.0f, 1.0f, -1.0f),Color(0,1,1,1),TextureDate(1,1),Normal(0,0,-1)),
	VertexData(Vertex(-1.0f, -1.0f, -1.0f),Color(1,1,1,1),TextureDate(1,0),Normal(0,0,-1)),
	VertexData(Vertex(1.0f, 1.0f, -1.0f),Color(1,1,0,1),TextureDate(0,1),Normal(0,0,-1)),
	VertexData(Vertex(-1.0f, -1.0f, -1.0f),Color(1,1,1,1),TextureDate(1,0),Normal(0,0,-1)),
	VertexData(Vertex(1.0f, -1.0f, -1.0f),Color(1,0,1,1),TextureDate(0,0),Normal(0,0,-1)),
	VertexData(Vertex(1.0f, 1.0f, -1.0f),Color(1,1,0,1),TextureDate(0,1),Normal(0,0,-1)),
};

//重用顶点
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
	bool isReuseVertex = false;//是否重用顶点
	Windows window(800, 600, "LearnOpenGL");
	Mesh mesh;
	Mesh lightCubeMesh;

	if (isReuseVertex)
	{
		CalculateVertexNormal(vertexDatas2, sizeof(vertexDatas2) / sizeof(vertexDatas2[0]), indices, sizeof(indices) / sizeof(indices[0]));
		mesh.Init(vertexDatas2, sizeof(vertexDatas2) / sizeof(vertexDatas2[0]), indices, sizeof(indices) / sizeof(indices[0]));
		lightCubeMesh.Init(vertexDatas2, sizeof(vertexDatas2) / sizeof(vertexDatas2[0]), indices, sizeof(indices) / sizeof(indices[0]));
	}
	else
	{
		mesh.Init(vertexDatas, sizeof(vertexDatas) / sizeof(vertexDatas[0]));
		lightCubeMesh.Init(vertexDatas2, sizeof(vertexDatas2) / sizeof(vertexDatas2[0]), indices, sizeof(indices) / sizeof(indices[0]));
	}

	//Mesh mesh(vertices, sizeof(vertices) / sizeof(vertices[0]));
	//Mesh mesh(vertexDatas, sizeof(vertexDatas) / sizeof(vertexDatas[0]));


	Shader shader("./res/cubeShader");
	Shader lightShader("./res/lightShader");

	Camera camera(window.GetWindow());

	unsigned int diffuseMap, specularMap;
	shader.LoadTexture("DiffuseMap.png", diffuseMap);
	shader.LoadTexture("SpecularMap.png", specularMap);

	shader.Use();
	shader.BindTexture(GL_TEXTURE0, diffuseMap, "material.diffuse", 0);
	shader.BindTexture(GL_TEXTURE1, specularMap, "material.specular", 1);

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
		shader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
		shader.setFloat("pointLights[0].constant", 1.0f);
		shader.setFloat("pointLights[0].linear", 0.09);
		shader.setFloat("pointLights[0].quadratic", 0.032);
		// point light 2
		shader.setVec3("pointLights[1].position", pointLightPositions[1]);
		shader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
		shader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
		shader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
		shader.setFloat("pointLights[1].constant", 1.0f);
		shader.setFloat("pointLights[1].linear", 0.09);
		shader.setFloat("pointLights[1].quadratic", 0.032);
		// point light 3
		shader.setVec3("pointLights[2].position", pointLightPositions[2]);
		shader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
		shader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
		shader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
		shader.setFloat("pointLights[2].constant", 1.0f);
		shader.setFloat("pointLights[2].linear", 0.09);
		shader.setFloat("pointLights[2].quadratic", 0.032);
		// point light 4
		shader.setVec3("pointLights[3].position", pointLightPositions[3]);
		shader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
		shader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
		shader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
		shader.setFloat("pointLights[3].constant", 1.0f);
		shader.setFloat("pointLights[3].linear", 0.09);
		shader.setFloat("pointLights[3].quadratic", 0.032);

		shader.setVec3("material.ambient", 0.25f, 0.4f, 0.8f);
		shader.setVec3("material.diffuse", 0.5f, 0.5f, 0.5f);
		shader.setVec3("material.specular", 0.8f, 0.8f, 0.8f);
		shader.setFloat("material.shininess", 32.0f);
		glm::mat4 model;
		for (int i = 0; i < 10; i++)
		{
			glm::mat4 model;
			model = glm::translate(model, glm::vec3(3,3,3) * cubePositions[i]);
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