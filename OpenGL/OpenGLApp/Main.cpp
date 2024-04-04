#include "precomp.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mesh.h"
#include "Shader.h"

constexpr GLint WIDTH = 800;
constexpr GLint HEIGHT = 600;
constexpr float toRadians = M_PI / 180.f;

std::vector<std::unique_ptr<Mesh>> meshes;
std::vector<std::unique_ptr<Shader>> shaders;

bool direction = true;
float triangleOffset = 0.f;
float triangleMaxOffset = 0.7f;
float triangleIncrement = 0.005f;

bool sizeDirection = true;
float curSize = 0.4f;
float maxSize = 0.8f;
float minSize = 0.1f;

float curAngle = 0.f;

// Vertex Shader
static const char* vertexShader = 
"#version 330												\n\
                                                            \n\
layout (location = 0) in vec3 pos;							\n\
out vec4 vertColor;											\n\
                                                            \n\
uniform mat4 modelMatrix;									\n\
uniform mat4 projectionMatrix;								\n\
void main()													\n\
{															\n\
	gl_Position = projectionMatrix * modelMatrix * vec4(pos, 1.0);\n\
	vertColor = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);			\n\
}";

static const char* fragmentShader = 
"#version 330												\n\
															\n\
in vec4 vertColor;											\n\
out vec4 colour;											\n\
															\n\
void main()													\n\
{															\n\
	colour = vertColor;										\n\
}";

void CreateObjects()
{
	unsigned int indices[] =
	{
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2,
	};
	GLfloat vertices[] =
	{
		-1.0f, -1.0f, 0.0f,
		0.0F, -1.0f, 1.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	std::unique_ptr<Mesh> mesh1 = std::unique_ptr<Mesh>(new Mesh());
	mesh1->CreateMesh(vertices, indices, sizeof(vertices) / sizeof(vertices[0]), sizeof(indices) / sizeof(indices[0]));
	meshes.push_back(std::move(mesh1));

	std::unique_ptr<Mesh> mesh2 = std::unique_ptr<Mesh>(new Mesh());
	mesh2->CreateMesh(vertices, indices, sizeof(vertices) / sizeof(vertices[0]), sizeof(indices) / sizeof(indices[0]));
	meshes.push_back(std::move(mesh2));
}

void CreateShaders()
{
	std::unique_ptr<Shader> shader1 = std::make_unique<Shader>();
	shader1->CreateFromString(vertexShader, fragmentShader);
	shaders.push_back(std::move(shader1));
}

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

	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, bufferWidth, bufferHeight);
	CreateObjects();
	CreateShaders();

	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	glm::mat4 projection = glm::perspective(45.f, static_cast<GLfloat>(bufferWidth) / static_cast<GLfloat>(bufferHeight), 0.1f, 100.f);

	while (glfwWindowShouldClose(mainWindow) == false)
	{
		glfwPollEvents();

		if (direction)
		{
			triangleOffset += triangleIncrement;
		}
		else
		{
			triangleOffset -= triangleIncrement;
		}

		if (std::abs(triangleOffset) >= triangleMaxOffset)
		{
			direction = !direction;
		}

		curAngle += 1.0f;
		if (curAngle >= 360.f)
		{
			curAngle -= 360.f;
		}

		if (sizeDirection)
		{
			curSize += 0.01f;
		}
		else
		{
			curSize -= 0.01f;
		}

		if (curSize >= maxSize || curSize <= minSize)
		{
			sizeDirection = !sizeDirection;
		}

		// set color when screen(actually color buffer) is cleared
		glClearColor(0.f, 0.f, 0.f, 1.f);
		// parameter available : GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT, GL_STENCIL_BUFFER_BIT
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaders[0]->UseShader();
		uniformModel = shaders[0]->GetModelLocation();
		uniformProjection = shaders[0]->GetProjectionLocation();

		glm::mat4 modelMatrix{ 1.0f };
		modelMatrix = glm::translate(modelMatrix, glm::vec3(triangleOffset, 0.f, -2.5f));
		modelMatrix = glm::rotate(modelMatrix, curAngle * toRadians, glm::vec3(0, 1, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMatrix));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshes[0]->RenderMesh();

		modelMatrix = glm::mat4{ 1.0f };
		modelMatrix = glm::translate(modelMatrix, glm::vec3(triangleOffset, 0.5f, -2.5f));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMatrix));
		meshes[1]->RenderMesh();

		glUseProgram(0);

		glfwSwapBuffers(mainWindow);
	}

	return 0;
}