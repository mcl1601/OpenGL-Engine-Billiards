#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\euler_angles.hpp>
#include <FreeImage.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
class SkyboxShader
{
private:
	GLuint program, vprogram, fprogram;
	std::string filenamev = "shaders/skyvShader.glsl";
	std::string filenamef = "shaders/skyfShader.glsl";
public:
	SkyboxShader();
	~SkyboxShader();
	bool load();
	bool compile(GLenum shaderType);
	void use();
	void unload();
	char* readBinaryFile(std::string filename);
};

