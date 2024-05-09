#include "precomp.h"
#include "Texture.h"
#include "stb_image.h"


Texture::Texture(const std::string& fileName)
	: fileName(fileName)
{
}

Texture::~Texture()
{
	UseTexture();
}

void Texture::LoadTexture()
{
	unsigned char* textureData = stbi_load(fileName.c_str(), &width, &height, &bitDepth, 0);
	if (textureData == nullptr)
	{
		printf("Failed to find : %s\n", fileName.c_str());
		return;
	}

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(textureData);
}

void Texture::UseTexture()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::ClearTexture()
{
	glDeleteTextures(1, &textureID);
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileName.clear();
}
