#include "precomp.h"
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

Camera::Camera(glm::vec3 startPosition, glm::vec3 startWorldUpVector, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
	: position(startPosition)
	, worldUpVector(startWorldUpVector)
	, yaw(startYaw)
	, pitch(startPitch)
	, moveSpeed(startMoveSpeed)
	, turnSpeed(startTurnSpeed)
{
	_Update();
}

Camera::~Camera()
{
}

glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(position, position + frontVector, upVector);;
}

void Camera::HandleKeyBoard(const std::array<bool, 1024>& keys, GLfloat deltaTime)
{
	auto velocity = moveSpeed * deltaTime;
	if (keys[GLFW_KEY_W])
	{
		position += frontVector * velocity;
	}
	else if (keys[GLFW_KEY_S])
	{
		position -= frontVector * velocity;
	}
	else if (keys[GLFW_KEY_A])
	{
		position -= rightVector * velocity;
	}
	else if (keys[GLFW_KEY_D])
	{
		position += rightVector * velocity;
	}
	else if (keys[GLFW_KEY_Q])
	{
		position -= worldUpVector * velocity;
	}
	else if (keys[GLFW_KEY_E])
	{
		position += worldUpVector * velocity;
	}
}

void Camera::HandleMouse(GLfloat xChange, GLfloat yChange)
{
	xChange *= turnSpeed;
	yChange *= turnSpeed;
	
	yaw += xChange;
	pitch += yChange;

	if (pitch > 89.f)
	{
		pitch = 89.f;
	}
	else if (pitch < -89.f)
	{
		pitch = -89.f;
	}

	_Update();
}

void Camera::_Update()
{
	frontVector.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	frontVector.y = sin(glm::radians(pitch));
	frontVector.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	frontVector = glm::normalize(frontVector);

	rightVector = glm::normalize(glm::cross(frontVector, worldUpVector));
	upVector = glm::normalize(glm::cross(rightVector, frontVector));
}
