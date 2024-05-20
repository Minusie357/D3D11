#include "precomp.h"
#include "Light.h"

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat inAmbientIntensity, GLfloat inDiffuseIntensity)
: colour(glm::vec3(red, green, blue))
, ambientIntensity(inAmbientIntensity)
, diffuseIntensity(inDiffuseIntensity)
{
}