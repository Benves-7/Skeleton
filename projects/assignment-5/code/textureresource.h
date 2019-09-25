#pragma once
#include <core/app.h>
#include "stb_image.h"

class TextureResource
{
public:

	TextureResource();
	~TextureResource();
	void bindTexture();
	void makeTexture(const char* textureFile);
	void del();

	unsigned int texture;

private:

};
inline TextureResource::TextureResource()
{

}
inline TextureResource::~TextureResource()
{
	glDeleteTextures(1, &texture);
}
/// Binds the texture handle.
inline void TextureResource::bindTexture()
{
	glBindTexture(GL_TEXTURE_2D, texture);
}
/// Loads the texture from the file.
inline void TextureResource::makeTexture(const char* textureFile)
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D,)
	// load and generate the texture
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(1);
	unsigned char *data = stbi_load(textureFile, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		printf("Failed to load texture\n");
	}
	stbi_image_free(data);
}
/// Needs a Delete func.
inline void TextureResource::del()
{
	
}
