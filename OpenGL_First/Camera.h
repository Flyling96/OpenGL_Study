#pragma once

#include<iostream>
#include<string>
#include<GLFW/glfw3.h>
#include<glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Camera
{
public:
	Camera(GLFWwindow* m_window);
	glm::mat4 Update();
	void ProcessInput(GLFWwindow *window, float dateTime);
	void UpdateFront();
	~Camera();

private:
	glm::mat4 view;
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	float yaw = -90;
	float pitch = 0;

};

