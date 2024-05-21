#pragma once
#include "Light.h"


class PointLight : public Light
{
public:
	PointLight() = default;
	PointLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat inAmbientIntensity, GLfloat inDiffuseIntensity, GLfloat xPosition, GLfloat yPosition, GLfloat zPosition, GLfloat inConstant, GLfloat inLinear, GLfloat inExponent);
	~PointLight() = default;

public:
	void UseLight(GLint ambientIntensityLocation, GLint ambientColourLocation, GLint diffuseIntensityLocation, GLint positionLocation, GLint constantLocation, GLint linearLocation, GLint exponentLocation);

protected:
	glm::vec3 position{};

	// ax^2 + bx + c
	GLfloat constant{ 1.0f };
	GLfloat linear{ 0.0f };
	GLfloat exponent{ 0.0f };
};

