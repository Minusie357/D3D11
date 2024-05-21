#pragma once
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"


class Shader
{
private:
	struct UniformDirectionalLight
	{
		GLint uniformColour;
		GLint uniformAmbientIntensity;
		GLint uniformDiffuseIntensity;

		GLint uniformDirection;
	};

	struct UniformPointLight
	{
		GLint uniformColour;
		GLint uniformAmbientIntensity;
		GLint uniformDiffuseIntensity;

		GLint uniformPosition;
		GLint uniformConstant;
		GLint uniformLinear;
		GLint uniformExponent;
	};

	struct UniformSpotLight
	{
		GLint uniformColour;
		GLint uniformAmbientIntensity;
		GLint uniformDiffuseIntensity;

		GLint uniformPosition;
		GLint uniformDirection;

		GLint uniformConstant;
		GLint uniformLinear;
		GLint uniformExponent;
		GLint uniformEdge;
	};


public:
	Shader() = default;
	~Shader();

public:
	void CreateFromString(const std::string& vertexCode, const std::string& fragmentCode);
	void CreateFromFiles(const std::string& vertexFileName, const std::string& fragmentFileName);
	std::string ReadFile(const std::string& fileName);

	void UseShader();
	void ClearShader();

	GLint GetProjectionLocation() const { return uniformProjection; }
	GLint GetModelLocation() const { return uniformModel; }
	GLint GetViewLocation() const { return uniformView; }
	GLint GetEyePosition() const { return uniformEyePosition; }

	GLint GetAmbientIntensityLocation() const { return uniformDirectinoalLight.uniformAmbientIntensity; }
	GLint GetAmbientColourLocation() const { return uniformDirectinoalLight.uniformColour; }
	GLint GetDiffuseIntensity() const { return uniformDirectinoalLight.uniformDiffuseIntensity; }
	GLint GetDirection() const { return uniformDirectinoalLight.uniformDirection; }

	GLint GetSpecularIntensity() const { return uniformSpecularIntensity; }
	GLint GetShininess() const { return uniformShininess; }

	void SetDirectionalLight(DirectionalLight* light);
	void SetPointLight(PointLight* light, size_t numsOfLight);
	void SetSpotLight(SpotLight* light, size_t numsOfLight);


private:
	void _CompileShader(const std::string& vertexCode, const std::string& fragmentCode);
	void _AddShader(GLuint program, const std::string& shaderCode, GLenum shaderType);

private:
	GLint shaderId{};
	GLint uniformProjection{};
	GLint uniformModel{};
	GLint uniformView{};
	GLint uniformEyePosition{};

	UniformDirectionalLight uniformDirectinoalLight{};
	GLint uniformNumsOfPointLight{};
	std::array<UniformPointLight, MAX_NUMS_OF_POINT_LIGHT> uniformPointLights{};

	GLint uniformNumsOfSpotLight{};
	std::array<UniformSpotLight, MAX_NUMS_OF_SPOT_LIGHT> uniformSpotLights{};

	GLint uniformSpecularIntensity{};
	GLint uniformShininess{};


private:
	size_t numsOfPointLight{};
	size_t numsOfSpotLight{};
};

