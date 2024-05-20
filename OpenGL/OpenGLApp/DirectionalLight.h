#pragma once
#include "Light.h"


class DirectionalLight : public Light
{
public:
	DirectionalLight() = default;
	DirectionalLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat inAmbientIntensity, GLfloat inDiffuseIntensity, GLfloat xDirection, GLfloat yDirection, GLfloat zDirection);
	~DirectionalLight() = default;

public:
	void UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation, GLuint diffuseIntensityLocation, GLuint directionLocation);

private:
	glm::vec3 direction{ 0.f, -1.f, 0.f };
};

