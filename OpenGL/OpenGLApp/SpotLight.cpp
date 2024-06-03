#include "precomp.h"
#include "SpotLight.h"

SpotLight::SpotLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat inAmbientIntensity, GLfloat inDiffuseIntensity, GLfloat xPosition, GLfloat yPosition, GLfloat zPosition, GLfloat xDirection, GLfloat yDirection, GLfloat zDirection, GLfloat inConstant, GLfloat inLinear, GLfloat inExponent, GLfloat inEdge)
	: PointLight(red, green, blue, inAmbientIntensity, inDiffuseIntensity, xPosition, yPosition, zPosition, inConstant, inLinear, inExponent)
	, direction(glm::normalize(glm::vec3(xDirection, yDirection, zDirection)))
	, edge(inEdge)
	, processedEdge(cosf(glm::radians(edge)))
{
}

void SpotLight::UseLight(GLint ambientIntensityLocation, GLint ambientColourLocation, GLint diffuseIntensityLocation, GLint positionLocation, GLint directionLocation, GLint constantLocation, GLint linearLocation, GLint exponentLocation, GLint edgeLocation)
{
	glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);

	glUniform3f(positionLocation, position.x, position.y, position.z);
	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(constantLocation, constant);
	glUniform1f(linearLocation, linear);
	glUniform1f(exponentLocation, exponent);
	glUniform1f(edgeLocation, processedEdge);
}

void SpotLight::SetFlash(glm::vec3 inPosition, glm::vec3 inDirection)
{
	position = inPosition;
	direction = inDirection;
}
