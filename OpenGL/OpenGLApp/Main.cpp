#include "precomp.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


constexpr GLint WIDTH = 800;
constexpr GLint HEIGHT = 600;
constexpr float toRadians = M_PI / 180.f;

GLuint VAO{};
GLuint VBO{};
GLuint IBO{};
GLuint shader{};
GLuint uniformModel{};
GLuint uniformProjection{};

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

void CreateTriangle()
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

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void AddShader(GLuint program, const std::string& shaderCode, GLenum shaderType)
{
	GLuint theShader = glCreateShader(shaderType);
	const GLchar* codes[1];
	GLint codeLengths[1];
	
	codes[0] = shaderCode.c_str();
	codeLengths[0] = static_cast<GLint>(shaderCode.length());

	glShaderSource(theShader, 1, codes, codeLengths);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[1024] = { 0, };
	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (result == 0)
	{
		glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
		std::cout << "Error Compiling program : " << eLog << "\n";
		return;
	}

	glAttachShader(program, theShader);
}

void CompileShaders()
{
	shader = glCreateProgram();
	if (shader == 0)
	{
		std::cout << "Error creating shader program" << std::endl;
		return;
	}

	AddShader(shader, vertexShader, GL_VERTEX_SHADER);
	AddShader(shader, fragmentShader, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0, };
	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (result == 0)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		std::cout << "error linking program : " << eLog << "\n";
		return;
	}

	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (result == 0)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		std::cout << "Error validating program : " << eLog << "\n";
		return;
	}

	uniformModel = glGetUniformLocation(shader, "modelMatrix");
	uniformProjection = glGetUniformLocation(shader, "projectionMatrix");
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
	CreateTriangle();
	CompileShaders();

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

		glUseProgram(shader);

		glm::mat4 modelMatrix{ 1.0f };
		modelMatrix = glm::translate(modelMatrix, glm::vec3(triangleOffset, 0.f, -2.5f));
		modelMatrix = glm::rotate(modelMatrix, curAngle * toRadians, glm::vec3(0, 1, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.4f, 0.4f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMatrix));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glUseProgram(0);

		glfwSwapBuffers(mainWindow);
	}

	return 0;
}