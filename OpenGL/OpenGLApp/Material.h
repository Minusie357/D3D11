#pragma once


class Material
{
public:
	Material() = default;
	Material(GLfloat inSpecularIntensity, GLfloat inShininess);
	~Material() = default;

public:
	void UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation);


private:
	GLfloat specularIntensity{};
	GLfloat shininess{};
};

