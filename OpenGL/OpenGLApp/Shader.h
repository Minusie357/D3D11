#pragma once


class Shader
{
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

	GLint GetAmbientIntensityLocation() const { return uniformAmbientIntensity; }
	GLint GetAmbientColourLocation() const { return uniformAmbientColour; }
	GLint GetDiffuseIntensity() const { return uniformDiffuseIntensity; }
	GLint GetDirection() const { return uniformDirection; }

	GLint GetSpecularIntensity() const { return uniformSpecularIntensity; }
	GLint GetShininess() const { return uniformShininess; }


private:
	void _CompileShader(const std::string& vertexCode, const std::string& fragmentCode);
	void _AddShader(GLuint program, const std::string& shaderCode, GLenum shaderType);

private:
	GLint shaderId{};
	GLint uniformProjection{};
	GLint uniformModel{};
	GLint uniformView{};
	GLint uniformEyePosition{};

	GLint uniformAmbientIntensity{};
	GLint uniformAmbientColour{};
	GLint uniformDiffuseIntensity{};
	GLint uniformDirection{};

	GLint uniformSpecularIntensity{};
	GLint uniformShininess{};
};

