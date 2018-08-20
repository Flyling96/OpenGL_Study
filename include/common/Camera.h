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
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);

private:
	glm::mat4 view;
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	float yaw = -90;
	float pitch = 0;

};

void mouse_callback(GLFWwindow* window, double xpos, double ypos);

Camera::Camera(GLFWwindow* m_window)
{
	cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	//隐藏光标
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//监听鼠标事件
	glfwSetCursorPosCallback(m_window, mouse_callback);
}


glm::mat4 Camera::Update()
{
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	return view;
}

void Camera::ProcessInput(GLFWwindow *window, float deltaTime)
{
	float cameraSpeed = 5.0f * deltaTime; // adjust accordingly
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

float lastX = 400, lastY = 300;
float xoffset = 0, yoffset = 0;

void Camera::UpdateFront()
{
	yaw += xoffset;
	pitch += yoffset;
	xoffset = 0;
	yoffset = 0;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}

Camera::~Camera()
{
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	xoffset = xpos - lastX;
	yoffset = lastY - ypos; // 注意这里是相反的，因为y坐标是从底部往顶部依次增大的
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;
}


