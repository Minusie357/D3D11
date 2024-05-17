#include "precomp.h"
#include "Light.h"

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat inAmbientIntensity, GLfloat xDirection, GLfloat yDirection, GLfloat zDirection, GLfloat inDiffuseIntensity)
: colour(glm::vec3(red, green, blue))
, ambientIntensity(inAmbientIntensity)
, direction(xDirection, yDirection, zDirection)
, diffuseIntensity(inDiffuseIntensity)
{
}

void Light::UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation, GLuint diffuseIntensityLocation, GLuint directionLocation)
{
	glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);

	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);
}
