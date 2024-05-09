#include "precomp.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "Light.h"


constexpr float toRadians = M_PI / 180.f;

Window mainWindow;
std::vector<std::unique_ptr<Mesh>> meshes;
std::vector<std::unique_ptr<Shader>> shaders;
Camera camera;

Texture brickTexture;
Texture dirtTexture;

Light mainLight;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

static const std::string vertexShaderFileName = "Shaders/shader.vert";
static const std::string fragmentShaderFileName = "Shaders/shader.frag";


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
		// x, y, z, u, v
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 1.0f, 0.5f, 0.0f,
		1.0f, -1.0f, 0.0f, 1.f, 0.f,
		0.0f, 1.0f, 0.0f, 0.5f, 1.f,
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
	shader1->CreateFromFiles(vertexShaderFileName, fragmentShaderFileName);
	shaders.push_back(std::move(shader1));
}

int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialize();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f), -90.f, 0.f, 5.f, 0.4f);

	brickTexture = Texture("../Resources/Textures/brick.png");
	brickTexture.LoadTexture();
	dirtTexture = Texture("../Resources/Textures/dirt.png");
	dirtTexture.LoadTexture();

	mainLight = Light(0.f, 1.f, 0.f, 0.2f);

	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	GLuint uniformAmbientIntensity = 0;
	GLuint uniformAmbientColour = 0;

	glm::mat4 projection = glm::perspective(45.f, static_cast<GLfloat>(mainWindow.GetBufferWidth()) / static_cast<GLfloat>(mainWindow.GetBufferHeight()), 0.1f, 100.f);

	while (mainWindow.GetShouldClose() == false)
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		glfwPollEvents();
		camera.HandleKeyBoard(mainWindow.GetKeys(), deltaTime);
		camera.HandleMouse(mainWindow.EmitXChanges(), mainWindow.EmitYChanges());

		// set color when screen(actually color buffer) is cleared
		glClearColor(0.f, 0.f, 0.f, 1.f);
		// parameter available : GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT, GL_STENCIL_BUFFER_BIT
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaders[0]->UseShader();
		uniformModel = shaders[0]->GetModelLocation();
		uniformProjection = shaders[0]->GetProjectionLocation();
		uniformView = shaders[0]->GetViewLocation();
		uniformAmbientIntensity = shaders[0]->GetAmbientIntensityLocation();
		uniformAmbientColour = shaders[0]->GetAmbientColourLocation();
			
		mainLight.UseLight(uniformAmbientIntensity, uniformAmbientColour);

		glm::mat4 modelMatrix{ 1.0f };
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.f, -2.5f));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMatrix));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
		brickTexture.UseTexture();
		meshes[0]->RenderMesh();

		modelMatrix = glm::mat4{ 1.0f };
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.5f, -2.5f));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMatrix));
		dirtTexture.UseTexture();
		meshes[1]->RenderMesh();

		glUseProgram(0);

		mainWindow.SwapBuffers();
	}

	return 0;
}