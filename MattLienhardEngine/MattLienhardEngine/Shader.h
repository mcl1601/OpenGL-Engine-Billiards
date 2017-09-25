#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\euler_angles.hpp>
#include <FreeImage.h>
#include <vector>
#include <string>

class Shader
{
private: 
	GLuint program, vprogram, fprogram;
	std::string filenamev = "shaders/vShader.glsl";
	std::string filenamef = "shaders/fShader.glsl";
	std::string * activeFile;
	GLuint * activeProgram;
public:
	Shader();
	~Shader();
	bool load();
	bool compile(GLenum shaderType);
	void use();
	void unload();
	char* readBinaryFile(std::string filename);
};

