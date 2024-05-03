#pragma once

#include <GL/glew.h>

class Texture
{
public:
	Texture() = default;
	Texture(const std::string& fileName);
	~Texture();

public:
	void LoadTexture();
	void UseTexture();
	void ClearTexture();

private:
	GLuint _TextureID{};
	int _Width{};
	int _Height{};
	int _BitDepth{};

	std::string _FileName{};
};

