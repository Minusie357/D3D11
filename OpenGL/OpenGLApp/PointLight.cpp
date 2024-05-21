#include "precomp.h"
#include "PointLight.h"

PointLight::PointLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat inAmbientIntensity, GLfloat inDiffuseIntensity, GLfloat xPosition, GLfloat yPosition, GLfloat zPosition, GLfloat inConstant, GLfloat inLinear, GLfloat inExponent)
	: Light(red, green, blue, inAmbientIntensity, inDiffuseIntensity)
	, position(glm::vec3(xPosition, yPosition, zPosition))
	, constant(inConstant)
	, linear(inLinear)
	, exponent(inExponent)
{
}

void PointLight::UseLight(GLint ambientIntensityLocation, GLint ambientColourLocation, GLint diffuseIntensityLocation, GLint positionLocation, GLint constantLocation, GLint linearLocation, GLint exponentLocation)
{
	glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);

	glUniform3f(positionLocation, position.x, position.y, position.z);
	glUniform1f(constantLocation, constant);
	glUniform1f(linearLocation, linear);
	glUniform1f(exponentLocation, exponent);
}