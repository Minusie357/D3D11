#include "precomp.h"

#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"


constexpr float toRadians = M_PI / 180.f;

Window mainWindow;
std::vector<std::unique_ptr<Mesh>> meshes;
std::vector<std::unique_ptr<Shader>> shaders;
Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;

DirectionalLight mainLight;
std::vector<PointLight> pointLights;
std::vector<SpotLight> spotLights;

Material shinyMaterial;
Material dullMaterial;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

static const std::string vertexShaderFileName = "Shaders/shader.vert";
static const std::string fragmentShaderFileName = "Shaders/shader.frag";

void CalcAverageNormals(unsigned int* indices, size_t indexCount, GLfloat* vertices, size_t vertexCount, size_t vertexLength, size_t normalOffset)
{
	for (auto index = 0; index < indexCount; index += 3)
	{
		auto in0 = indices[index] * vertexLength;
		auto in1 = indices[index + 1] * vertexLength;
		auto in2 = indices[index + 2] * vertexLength;

		// make vector using p0(in0) - p1(in1) 
		glm::vec3 v1(
			vertices[in1] - vertices[in0], 
			vertices[in1 + 1] - vertices[in0 + 1], 
			vertices[in1 + 2] - vertices[in0 + 2]);
		// make the other vector using p0(in0) - p2(in2)
		glm::vec3 v2(
			vertices[in2] - vertices[in0], 
			vertices[in2 + 1] - vertices[in0 + 1], 
			vertices[in2 + 2] - vertices[in0 + 2]);
		// make normal vector using v1 and v2
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		// push result to normal section in vertices array
		for (auto in : { in0, in1, in2 })
		{
			in += normalOffset;
			vertices[in] += normal.x;
			vertices[in + 1] += normal.y;
			vertices[in + 2] += normal.z;
		}
	}

	for (auto index = 0; index < vertexCount / vertexLength; ++index)
	{
		auto offset = index * vertexLength + normalOffset;
		glm::vec3 vec(vertices[offset], vertices[offset + 1], vertices[offset + 2]);
		vec = glm::normalize(vec);
		vertices[offset] = vec.x;
		vertices[offset + 1] = vec.y;
		vertices[offset + 2] = vec.z;
	}
}


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
		// x,	 y,		z,			u,		v,			nx,		ny,		nz
		-1.0f,	-1.0f,	-0.6f,		0.0f,	0.0f,		0.f,	0.f,	0.f,
		0.0f,	-1.0f,	1.0f,		0.5f,	0.0f,		0.f,	0.f,	0.f,
		1.0f,	-1.0f,	-0.6f,		1.f,	0.f,		0.f,	0.f,	0.f,
		0.0f,	1.0f,	0.0f,		0.5f,	1.f,		0.f,	0.f,	0.f,
	};

	unsigned int floorIndices[] =
	{
		0, 2, 1,
		1, 2, 3,
	};

	GLfloat floorVertices[] =
	{
		-10.0f,	0.0f,	-10.0f,		0.0f,	0.0f,		0.0f,	-1.0f,	0.0f,
		10.0f,	0.0f,	-10.0f,		10.0f,	0.0f,		0.0f,	-1.0f,	0.0f,
		-10.0f,	0.0f,	10.0f,		0.0f,	10.0f,		0.0f,	-1.0f,	0.0f,
		10.0f,	0.0f,	10.0f,		10.0f,	10.0f,		0.0f,	-1.0f,	0.0f,
	};

	auto numOfVertices = sizeof(vertices) / sizeof(vertices[0]);
	auto numOfIndices = sizeof(indices) / sizeof(indices[0]);

	auto numsOfFloorVertices = sizeof(floorVertices) / sizeof(floorVertices[0]);
	auto numsOfFloorIndices = sizeof(floorIndices) / sizeof(floorIndices[0]);

	CalcAverageNormals(indices, numOfIndices, vertices, numOfVertices, 8, 5);

	std::unique_ptr<Mesh> mesh1 = std::unique_ptr<Mesh>(new Mesh());
	mesh1->CreateMesh(vertices, indices, numOfVertices, numOfIndices);
	meshes.push_back(std::move(mesh1));

	std::unique_ptr<Mesh> mesh2 = std::unique_ptr<Mesh>(new Mesh());
	mesh2->CreateMesh(vertices, indices, numOfVertices, numOfIndices);
	meshes.push_back(std::move(mesh2));

	std::unique_ptr<Mesh> mesh3 = std::unique_ptr<Mesh>(new Mesh());
	mesh3->CreateMesh(floorVertices, floorIndices, numsOfFloorVertices, numsOfFloorIndices);
	meshes.push_back(std::move(mesh3));
}

void CreateShaders()
{
	std::unique_ptr<Shader> shader1 = std::make_unique<Shader>();
	shader1->CreateFromFiles(vertexShaderFileName, fragmentShaderFileName);
	shaders.push_back(std::move(shader1));
}

int main()
{
	mainWindow = Window(1366, 768);
	mainWindow.Initialize();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f), -90.f, 0.f, 5.f, 0.4f);

	brickTexture = Texture("../Resources/Textures/brick.png");
	brickTexture.LoadTexture();
	dirtTexture = Texture("../Resources/Textures/dirt.png");
	dirtTexture.LoadTexture();
	plainTexture = Texture("../Resources/Textures/plain.png");
	plainTexture.LoadTexture();

	shinyMaterial = Material(4.f, 256);
	dullMaterial = Material(0.3f, 4);
	mainLight = DirectionalLight(
		1.0f, 1.0f, 1.0f, 
		0.1f, 0.1f, 
		0.0f, 0.0f, -1.0f);

	pointLights.push_back(
		PointLight(
			0.0f, 0.0f, 1.0f,
			0.0f, 0.1f,
			0.0f, 0.0f, 0.0f,
			0.3f, 0.2f, 0.1f));
	pointLights.push_back(
		PointLight(
			0.0f, 1.0f, 0.0f,
			0.0f, 0.1f,
			-4.0f, 2.0f, 0.0f,
			0.3f, 0.1f, 0.1f));

	spotLights.push_back(
		SpotLight(
			1.0f, 1.0f, 1.0f,
			0.0f, 2.0f,
			0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 0.0f,
			1.0f, 0.0f, 0.0f, 20.0f));
	spotLights.push_back(
		SpotLight(
			1.0f, 1.0f, 1.0f,
			0.0f, 1.0f,
			0.0f, 1.5f, 0.0f,
			-100.0f, -1.0f, 0.0f,
			1.0f, 0.0f, 0.0f, 20.0f));


	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	GLuint uniformEyePosition = 0;
	GLuint uniformSpecularIntensity = 0;
	GLuint uniformShininess = 0;

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
		glClearColor(0.f, 0.0f, 0.f, 1.f);
		// parameter available : GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT, GL_STENCIL_BUFFER_BIT
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaders[0]->UseShader();
		uniformModel = shaders[0]->GetModelLocation();
		uniformProjection = shaders[0]->GetProjectionLocation();
		uniformView = shaders[0]->GetViewLocation();
		uniformEyePosition = shaders[0]->GetEyePosition();
		uniformSpecularIntensity = shaders[0]->GetSpecularIntensity();
		uniformShininess = shaders[0]->GetShininess();

		glm::vec3 lowerLight = camera.GetPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.GetDirection());
		
		shaders[0]->SetDirectionalLight(&mainLight);
		shaders[0]->SetPointLight(pointLights.data(), pointLights.size());
		shaders[0]->SetSpotLight(spotLights.data(), spotLights.size());

		auto cameraPosition = camera.GetPosition();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
		glUniform3f(uniformEyePosition, cameraPosition.x, cameraPosition.y, cameraPosition.z);
		
		glm::mat4 modelMatrix{ 1.0f };
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.f, -2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMatrix));
		brickTexture.UseTexture();
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshes[0]->RenderMesh();

		modelMatrix = glm::mat4{ 1.0f };
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 4.0f, -2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMatrix));
		dirtTexture.UseTexture();
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshes[1]->RenderMesh();

		modelMatrix = glm::mat4{ 1.0f };
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, -2.0f, -0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMatrix));
		dirtTexture.UseTexture();
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshes[2]->RenderMesh();

		glUseProgram(0);

		mainWindow.SwapBuffers();
	}

	return 0;
}