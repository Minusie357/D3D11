#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

constexpr GLint WIDTH = 800;
constexpr GLint HEIGHT = 600;

int main()
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

	auto mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", nullptr, nullptr);
	if (mainWindow == nullptr)
	{
		std::cout << "GLFW window creation failed" << std::endl;
		glfwTerminate();
		return 1;
	}

	int bufferWidth{};
	int bufferHeight{};
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);
	glfwMakeContextCurrent(mainWindow);
	
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW initailization failed" << std::endl;
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glViewport(0, 0, bufferWidth, bufferHeight);


	while (glfwWindowShouldClose(mainWindow) == false)
	{
		glfwPollEvents();

		glClearColor(1.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(mainWindow);
	}

	return 0;
}