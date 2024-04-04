#version 330

layout (location = 0) in vec3 pos;
out vec4 vertColor;

uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;
void main()
{
	gl_Position = projectionMatrix * modelMatrix * vec4(pos, 1.0);
	vertColor = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);
}