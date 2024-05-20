#include "precomp.h"
#include "Shader.h"

Shader::~Shader()
{
	ClearShader();
}

void Shader::CreateFromString(const std::string& vertexCode, const std::string& fragmentCode)
{
	_CompileShader(vertexCode, fragmentCode);
}

void Shader::CreateFromFiles(const std::string& vertexFileName, const std::string& fragmentFileName)
{
	std::string vertexCode = ReadFile(vertexFileName);
	std::string fragmentCode = ReadFile(fragmentFileName);

	_CompileShader(vertexCode, fragmentCode);
}

std::string Shader::ReadFile(const std::string& fileName)
{
	std::stringstream contentStream;
	std::ifstream fileStream(fileName, std::ios::in);

	if (fileStream.is_open() == false)
	{
		std::cout << "Failed to read " << fileName << "! File doesn't exist." << std::endl;
		return {};
	}

	std::string line{};
	while (fileStream.eof() == false)
	{
		std::getline(fileStream, line);
		contentStream << line << std::endl;
	}

	fileStream.close();
	return contentStream.str();
}

void Shader::UseShader()
{
	glUseProgram(shaderId);
}

void Shader::ClearShader()
{
	if (shaderId == 0)
	{
		return;
	}

	glDeleteProgram(shaderId);
	shaderId = 0;
	uniformModel = 0;
	uniformProjection = 0;
	uniformView = 0;
}

void Shader::SetDirectionalLight(DirectionalLight* light)
{
	if (light == nullptr)
	{
		return;
	}

	light->UseLight(uniformDirectinoalLight.uniformAmbientIntensity, uniformDirectinoalLight.uniformColour, uniformDirectinoalLight.uniformDiffuseIntensity, uniformDirectinoalLight.uniformDirection);
}

void Shader::SetPointLight(PointLight* light, size_t numsOfLight)
{
	if (light == nullptr)
	{
		return;
	}

	numsOfLight = std::min(numsOfLight, MAX_NUMS_OF_POINT_LIGHT);
	glUniform1i(uniformnumsOfPointLight, numsOfLight);

	for (auto index = 0; index < numsOfLight; ++index)
	{
		auto uniformPointLight = uniformPointLights[index];
		light[index].UseLight(
			uniformPointLight.uniformAmbientIntensity, 
			uniformPointLight.uniformColour, 
			uniformPointLight.uniformDiffuseIntensity, 
			uniformPointLight.uniformPosition, 
			uniformPointLight.uniformConstant, 
			uniformPointLight.uniformLinear, 
			uniformPointLight.uniformExponent);
	}
}

void Shader::_CompileShader(const std::string& vertexCode, const std::string& fragmentCode)
{
	shaderId = glCreateProgram();
	if (shaderId == 0)
	{
		std::cout << "Error creating shader program" << std::endl;
		return;
	}

	_AddShader(shaderId, vertexCode, GL_VERTEX_SHADER);
	_AddShader(shaderId, fragmentCode, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0, };
	glLinkProgram(shaderId);
	glGetProgramiv(shaderId, GL_LINK_STATUS, &result);
	if (result == 0)
	{
		glGetProgramInfoLog(shaderId, sizeof(eLog), NULL, eLog);
		std::cout << "error linking program : " << eLog << "\n";
		return;
	}

	glValidateProgram(shaderId);
	glGetProgramiv(shaderId, GL_VALIDATE_STATUS, &result);
	if (result == 0)
	{
		glGetProgramInfoLog(shaderId, sizeof(eLog), NULL, eLog);
		std::cout << "Error validating program : " << eLog << "\n";
		return;
	}

	uniformModel = glGetUniformLocation(shaderId, "modelMatrix");
	uniformProjection = glGetUniformLocation(shaderId, "projectionMatrix");
	uniformView = glGetUniformLocation(shaderId, "viewMatrix");

	uniformDirectinoalLight.uniformColour = glGetUniformLocation(shaderId, "directionalLight.base.colour");
	uniformDirectinoalLight.uniformAmbientIntensity = glGetUniformLocation(shaderId, "directionalLight.base.ambientIntensity");
	uniformDirectinoalLight.uniformDirection = glGetUniformLocation(shaderId, "directionalLight.direction");
	uniformDirectinoalLight.uniformDiffuseIntensity = glGetUniformLocation(shaderId, "directionalLight.base.diffuseIntensity");

	uniformSpecularIntensity = glGetUniformLocation(shaderId, "material.specularIntensity");
	uniformShininess = glGetUniformLocation(shaderId, "material.shininess");
	uniformEyePosition = glGetUniformLocation(shaderId, "eyePosition");

	uniformnumsOfPointLight = glGetUniformLocation(shaderId, "pointLightCount");
	for (auto index = 0; index < MAX_NUMS_OF_POINT_LIGHT; ++index)
	{
		uniformPointLights[index].uniformColour = glGetUniformLocation(shaderId, std::format("pointLights[{}].base.colour", index).c_str());
		uniformPointLights[index].uniformAmbientIntensity = glGetUniformLocation(shaderId, std::format("pointLights[{}].base.ambientIntensity", index).c_str());
		uniformPointLights[index].uniformDiffuseIntensity = glGetUniformLocation(shaderId, std::format("pointLights[{}].base.diffuseIntensity", index).c_str());
		uniformPointLights[index].uniformPosition = glGetUniformLocation(shaderId, std::format("pointLights[{}].position", index).c_str());
		uniformPointLights[index].uniformConstant = glGetUniformLocation(shaderId, std::format("pointLights[{}].constant", index).c_str());
		uniformPointLights[index].uniformLinear = glGetUniformLocation(shaderId, std::format("pointLights[{}].linear", index).c_str());
		uniformPointLights[index].uniformExponent = glGetUniformLocation(shaderId, std::format("pointLights[{}].exponent", index).c_str());
	}
}

void Shader::_AddShader(GLuint program, const std::string& shaderCode, GLenum shaderType)
{
	GLuint theShader = glCreateShader(shaderType);
	const GLchar* codes[1];
	GLint codeLengths[1];

	codes[0] = shaderCode.c_str();
	codeLengths[0] = static_cast<GLint>(shaderCode.length());

	glShaderSource(theShader, 1, codes, codeLengths);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[1024] = { 0, };
	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (result == 0)
	{
		glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
		std::cout << "Error Compiling program : " << eLog << "\n";
		return;
	}

	glAttachShader(program, theShader);
}
