#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>


class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	~Window();

public:
	GLfloat GetBufferWidth() const { return bufferSize.first; }
	GLfloat GetBufferHeight() const { return bufferSize.second; }

	bool GetShouldClose() const;

public:
	int Initialize();
	void SwapBuffers();

private:
	GLFWwindow* mainWindow{};

	std::pair<GLint, GLint> windowSize{};
	std::pair<GLint, GLint> bufferSize{};
};

