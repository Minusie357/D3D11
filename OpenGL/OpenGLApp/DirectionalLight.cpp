#include "precomp.h"
#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat inAmbientIntensity, GLfloat inDiffuseIntensity, GLfloat xDirection, GLfloat yDirection, GLfloat zDirection)
	: Light(red, green, blue, inAmbientIntensity, inDiffuseIntensity)
	, direction(xDirection, yDirection, zDirection)
{
}

void DirectionalLight::UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation, GLuint diffuseIntensityLocation, GLuint directionLocation)
{
	glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);

	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);
}
