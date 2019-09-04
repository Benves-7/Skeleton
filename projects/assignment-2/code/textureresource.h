#pragma once
#include <core/app.h>

class TextureResource
{
public:

	TextureResource();
	~TextureResource();
	void bindTexture();
	void makeTexture();
	void del();

	unsigned int texture;

private:

};
