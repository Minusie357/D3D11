#pragma once
#include "Light.h"


class PointLight : public Light
{
public:
	PointLight() = default;
	PointLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat inAmbientIntensity, GLfloat inDiffuseIntensity, GLfloat xPosition, GLfloat yPosition, GLfloat zPosition, GLfloat inConstant, GLfloat inLinear, GLfloat inExponent);
	~PointLight() = default;

public:
	void UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation, GLuint diffuseIntensityLocation, GLuint positionLocation, GLfloat constantLocation, GLfloat linearLocation, GLfloat exponentLocation);

private:
	glm::vec3 position{};

	// ax^2 + bx + c
	GLfloat constant{ 1.0f };
	GLfloat linear{ 0.0f };
	GLfloat exponent{ 0.0f };
};

