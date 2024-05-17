#pragma once


class Light
{
public:
	Light() = default;
	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat inAmbientIntensity, GLfloat xDirection, GLfloat yDirection, GLfloat zDirection, GLfloat inDiffuseIntensity);
	~Light() = default;

public:
	void UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation, GLuint diffuseIntensityLocation, GLuint directionLocation);

private:
	glm::vec3 colour{ 1.f, 1.f, 1.f };
	GLfloat ambientIntensity{ 1.f };

	glm::vec3 direction{ 0.f, -1.f, 0.f };
	GLfloat diffuseIntensity{};
};

