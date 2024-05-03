#include "precomp.h"
#include "Texture.h"
#include "stb_image.h"


Texture::Texture(const std::string& fileName)
	: _FileName(fileName)
{
}

Texture::~Texture()
{
	UseTexture();
}

void Texture::LoadTexture()
{
	unsigned char* textureData = stbi_load(_FileName.c_str(), &_Width, &_Height, &_BitDepth, 0);
	if (textureData == nullptr)
	{
		printf("Failed to find : %s\n", _FileName.c_str());
		return;
	}

	glGenTextures(1, &_TextureID);
	glBindTexture(GL_TEXTURE_2D, _TextureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _Width, _Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(textureData);
}

void Texture::UseTexture()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _TextureID);
}

void Texture::ClearTexture()
{
	glDeleteTextures(1, &_TextureID);
	_TextureID = 0;
	_Width = 0;
	_Height = 0;
	_BitDepth = 0;
	_FileName.clear();
}
