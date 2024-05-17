#version 330

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 norm;

out vec4 vertColor;
out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

void main()
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(pos, 1.0);
	vertColor = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);

	TexCoord = tex;

	Normal = mat3(transpose(inverse(modelMatrix))) * norm;

	FragPos = (modelMatrix * vec4(pos, 1.0)).xyz;
}