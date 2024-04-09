#pragma once
#include <GL/glew.h>


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

	GLuint GetProjectionLocation() const { return uniformProjection; }
	GLuint GetModelLocation() const { return uniformModel; }
	GLuint GetViewLocation() const { return uniformView; }

private:
	void _CompileShader(const std::string& vertexCode, const std::string& fragmentCode);
	void _AddShader(GLuint program, const std::string& shaderCode, GLenum shaderType);

private:
	GLuint shaderId{};
	GLuint uniformProjection{};
	GLuint uniformModel{};
	GLuint uniformView{};
};

