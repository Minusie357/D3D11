#include "precomp.h"
#include "Window.h"

Window::Window()
	: windowSize(800, 600)
{
}

Window::Window(GLint windowWidth, GLint windowHeight)
	: windowSize(windowWidth, windowHeight)
{
}

Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}

bool Window::GetShouldClose() const
{
	if (mainWindow == nullptr)
	{
		return true;
	}

	return glfwWindowShouldClose(mainWindow);
}

int Window::Initialize()
{
	// Initialize GLFW
	if (glfwInit() == false)
	{
		std::cout << "GLFW initialization failed" << std::endl;
		glfwTerminate();
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

	mainWindow = glfwCreateWindow(windowSize.first, windowSize.second, "Test Window", nullptr, nullptr);
	if (mainWindow == nullptr)
	{
		std::cout << "GLFW window creation failed" << std::endl;
		glfwTerminate();
		return 1;
	}

	glfwGetFramebufferSize(mainWindow, &bufferSize.first, &bufferSize.second);
	glfwMakeContextCurrent(mainWindow);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW initailization failed" << std::endl;
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, bufferSize.first, bufferSize.second);

	return 0;
}

void Window::SwapBuffers()
{
	if (mainWindow == nullptr)
	{
		return;
	}

	glfwSwapBuffers(mainWindow);
}
