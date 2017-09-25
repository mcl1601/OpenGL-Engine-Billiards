#include "Cubemap.h"
Cubemap::Cubemap()
{
}


Cubemap::~Cubemap()
{
}

void Cubemap::load()
{
	// generate and bind a new texture
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texID);

	// prevent seams from being visible
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	// loop through each texture face and load it
	// Order: right, left, top, bottom, back, front
	const char* textures[6] = { "textures/right.jpg", "textures/left.jpg", "textures/top.jpg", "textures/bottom.jpg", "textures/back.jpg", "textures/front.jpg" };
	for (int i = 0; i < 6; i++)
	{
		// try to load a texture
		FIBITMAP* image = FreeImage_Load(FreeImage_GetFileType(textures[i], 0), textures[i]);

		// see if it worked
		if (image == nullptr)
			return; // didn't work

		// convert the image to a 32-bit image
		FIBITMAP* image32Bit = FreeImage_ConvertTo32Bits(image);

		// flip if necessary
		FreeImage_FlipVertical(image32Bit);

		// unload the original image
		FreeImage_Unload(image);

		// upload to the cubemap binding point
		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,	//target texture
			0,	//mipmap n (0 for base image)
			GL_SRGB_ALPHA, //format of texture data in shader
			FreeImage_GetWidth(image32Bit),	//w
			FreeImage_GetHeight(image32Bit),	//h
			0,	//border must be 0
			GL_BGRA, //format of pixel data
			GL_UNSIGNED_BYTE,	//type of pixel data
			(void*)FreeImage_GetBits(image32Bit));	//address of pixel data

		// unload the texture from RAM
		FreeImage_Unload(image32Bit);
	}

	// unbind when done
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Cubemap::use()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, texID);
}
