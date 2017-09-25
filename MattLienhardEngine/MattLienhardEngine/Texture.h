#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\euler_angles.hpp>
#include <FreeImage.h>
#include <string>
class Texture
{
protected:
	FIBITMAP* image32Bit;
	GLuint texID;
	std::string filename;
public:
	Texture();
	Texture(std::string fname);
	virtual ~Texture();
	virtual void load();
	void unload();
	virtual void use();
};

