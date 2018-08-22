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

void DrawCube(Mesh& mesh, Shader& shader, Camera& camera, glm::mat4 model, bool isReuseVertex);

int main()
{
	Windows window(800, 600, "LearnOpenGL");
	Model model;
	Mesh lightCubeMesh;
	Shader shader;
	Shader lightShader;



	shader.Init("../res/shader/boneAnimShader");
	lightShader.Init("../res/shader/lightShader");
	model.Init("../res/model/guard/boblampclean.md5mesh");

	lightCubeMesh.Init(vertexDatas2, indices);

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
	float startTime = glfwGetTime();
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

		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);
		glm::mat4 view = camera.Update();
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);

		glm::mat4 modelMat;
		modelMat = glm::translate(modelMat, glm::vec3(0.0f, -1.75f, 0.0f));
		modelMat = glm::scale(modelMat, glm::vec3(0.02f, 0.02f, 0.02f));
		modelMat = glm::rotate(modelMat, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		shader.setMat4("model", modelMat);

		float runningTime = (currentFrame - startTime);
		vector<glm::mat4> Transforms;
		model.BoneTransform(runningTime, Transforms);
		for (int i = 0; i < Transforms.size(); i++) {
			string name = "gBones[";
			stringstream ss;
			ss << name << i << "]";
			name = ss.str();
			shader.setMat4t(name, Transforms[i]);
		}


		model.Draw(shader);

		lightShader.Use();
		lightShader.setVec3("lightColor", lightColor);
		for (int i = 0; i < 1; i++)
		{
			glm::mat4 lightModel;
			lightModel = glm::translate(lightModel, pointLightPositions[i]);
			DrawCube(lightCubeMesh, lightShader, camera, lightModel, true);
		}

		window.Update();
	}
	glfwTerminate();
	return 0;
}

void DrawCube(Mesh& mesh, Shader& shader, Camera& camera, glm::mat4 model, bool isReuseVertex)
{
	shader.BindTransform(800, 600);
	shader.setMat4("view", camera.Update());
	shader.setMat4("model", model);
	mesh.Draw(isReuseVertex);
}