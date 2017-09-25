#include "Texture.h"



Texture::Texture()
{

}

Texture::Texture(std::string fname)
{
	filename = fname;
}

Texture::~Texture()
{
}

void Texture::load()
{

	// try to load a texture
	FIBITMAP* image = FreeImage_Load(FreeImage_GetFileType(filename.c_str(), 0), filename.c_str());

	// see if it worked
	if (image == nullptr)
		return; // didn't work

	// convert the image to a 32-bit image
	image32Bit = FreeImage_ConvertTo32Bits(image);

	// unload the original image
	FreeImage_Unload(image);

	// generate and bind a new texture
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	// upload the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, 
		FreeImage_GetWidth(image32Bit), // width
		FreeImage_GetHeight(image32Bit), // height
		0, GL_BGRA, GL_UNSIGNED_BYTE, 
		(void*)FreeImage_GetBits(image32Bit)); // address

	// set the min filter to linear instead of mipmap linear
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// unload the texture from RAM
	FreeImage_Unload(image32Bit);

	// unbind when done
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::unload()
{
	glDeleteTextures(1, &texID);
}

void Texture::use()
{
	glBindTexture(GL_TEXTURE_2D, texID);
}
