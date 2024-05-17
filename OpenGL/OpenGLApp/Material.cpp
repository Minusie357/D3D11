#include "precomp.h"
#include "Material.h"

Material::Material(GLfloat inSpecularIntensity, GLfloat inShininess)
	: specularIntensity(inSpecularIntensity)
	, shininess(inShininess)
{
}

void Material::UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation)
{
	glUniform1f(specularIntensityLocation, specularIntensity);
	glUniform1f(shininessLocation, shininess);
}
