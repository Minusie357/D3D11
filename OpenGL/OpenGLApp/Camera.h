#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>


class Camera
{
public:
	Camera() = default;
	Camera(glm::vec3 startPosition, glm::vec3 startUpVector, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed);
	~Camera();

public:
	glm::mat4 GetViewMatrix() const;

public:
	void HandleKeyBoard(const std::array<bool, 1024>& keys, GLfloat deltaTime);
	void HandleMouse(GLfloat xChange, GLfloat yChange);

private:
	void _Update();

private:
	glm::vec3 position{};
	glm::vec3 frontVector{};
	glm::vec3 upVector{};
	glm::vec3 rightVector{};

	glm::vec3 worldUpVector{};

	GLfloat yaw{};
	GLfloat pitch{};

	GLfloat moveSpeed{};
	GLfloat turnSpeed{};
};

