#include "precomp.h"
#include "Window.h"
#include <GLFW/glfw3.h>


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

GLfloat Window::EmitXChanges()
{
	GLfloat change = xChange;
	xChange = 0.0f;
	return change;
}

GLfloat Window::EmitYChanges()
{
	GLfloat change = yChange;
	yChange = 0.0f;
	return change;
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

	_InitCallbacks();
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
	glfwSetWindowUserPointer(mainWindow, this);

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

void Window::_HandleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
	if (theWindow == nullptr)
	{
		return;
	}

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
		}
	}
}

void Window::_HandleMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
	if (theWindow == nullptr)
	{
		return;
	}

	if (theWindow->mouseFirstMove)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMove = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}

void Window::_InitCallbacks()
{
	glfwSetKeyCallback(mainWindow, Window::_HandleKeys);
	glfwSetCursorPosCallback(mainWindow, Window::_HandleMouse);
}
