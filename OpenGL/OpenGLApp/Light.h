#pragma once


class Light
{
public:
	Light() = default;
	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat inAmbientIntensity, GLfloat inDiffuseIntensity);
	~Light() = default;

public:

protected:
	glm::vec3 colour{ 1.f, 1.f, 1.f };
	GLfloat ambientIntensity{ 1.f };
	GLfloat diffuseIntensity{};
};

