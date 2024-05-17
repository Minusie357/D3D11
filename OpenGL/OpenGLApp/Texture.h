#pragma once


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
	GLuint textureID{};
	int width{};
	int height{};
	int bitDepth{};

	std::string fileName{};
};

