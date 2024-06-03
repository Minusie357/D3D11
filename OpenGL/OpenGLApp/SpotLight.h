#pragma once
#include "PointLight.h"


class SpotLight : public PointLight
{
public:
	SpotLight() = default;
	SpotLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat inAmbientIntensity, GLfloat inDiffuseIntensity, GLfloat xPosition, GLfloat yPosition, GLfloat zPosition, GLfloat xDirection, GLfloat yDirection, GLfloat zDirection, GLfloat inConstant, GLfloat inLinear, GLfloat inExponent, GLfloat inEdge);
	~SpotLight() = default;

public:
	void UseLight(GLint ambientIntensityLocation, GLint ambientColourLocation, GLint diffuseIntensityLocation, GLint positionLocation, GLint directionLocation, GLint constantLocation, GLint linearLocation, GLint exponentLocation, GLint edgeLocation);

	void SetFlash(glm::vec3 inPosition, glm::vec3 inDirection);

private:
	glm::vec3 direction{ 0.f, -1.f, 0.f };

	GLfloat edge{};
	GLfloat processedEdge{ cosf(glm::radians(edge)) };
};

