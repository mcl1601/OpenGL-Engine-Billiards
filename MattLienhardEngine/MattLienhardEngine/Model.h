#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\euler_angles.hpp>
#include <FreeImage.h>
#include <string>
class Model
{
private:
	unsigned int vertCount;
	GLuint vertArr;
public:
	Model();
	~Model();
	bool buffer(std::string objFile);
	virtual void render();
};

