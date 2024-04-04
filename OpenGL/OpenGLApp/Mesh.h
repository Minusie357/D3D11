#pragma once

#include <gl/glew.h>

class Mesh
{
public:
	Mesh();
	~Mesh();

public:
	void CreateMesh(GLfloat* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices);
	void RenderMesh();
	void ClearMesh();

private:
	GLuint VAO{};
	GLuint VBO{};
	GLuint IBO{};
	GLsizei _NumOfIndices{};
};

