#pragma once
#include <gl/glew.h>
#include <glm/glm.hpp>


class Light
{
public:
	Light() = default;
	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat intensity);
	~Light() = default;

public:
	void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation);

private:
	glm::vec3 colour{1.f, 1.f, 1.f};
	GLfloat ambientIntensity{ 1.f };
};

