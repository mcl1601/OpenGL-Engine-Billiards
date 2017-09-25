#include "Shader.h"
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\euler_angles.hpp>
#include <FreeImage.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
Shader::Shader()
{
}


Shader::~Shader()
{
}

bool Shader::load()
{
	// call compile for the vertex shader
	if (!compile(GL_VERTEX_SHADER))
	{
		// FAILED
		return false;
	}

	// call compile for the fragment shader
	if (!compile(GL_FRAGMENT_SHADER))
	{
		// FAILED
		return false;
	}

	// both shaders compiled
	// create shader program
	program = glCreateProgram();

	// attach the shaders
	glAttachShader(program, vprogram);
	glAttachShader(program, fprogram);

	// link the program
	glLinkProgram(program);

	// determine if it linked correctly
	GLint result;
	glGetProgramiv(program, GL_LINK_STATUS, &result);
	if (result == 0)
	{
		// FAILED	
		// get the log length
		GLint length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

		// make a GLchar array of that length
		GLchar* elog = new GLchar[length];

		// get the error info log
		glGetProgramInfoLog(program, length, 0, elog);

		// print the error log
		std::cout << elog << std::endl;

		// delete the program and array
		glDeleteProgram(program);
		delete[] elog;
	}
	else
		return true; // SUCCEEDED

	return false;
}

bool Shader::compile(GLenum shaderType)
{
	// text from shader files
	char* sourcecode;
	
	// used for error logging
	GLint result;
	GLint length;

	// load the vertex shader
	if (shaderType == GL_VERTEX_SHADER)
	{
		// read the sourcecode
		sourcecode = readBinaryFile(filenamev);

		// make sure it worked
		if (sourcecode == nullptr)
		{
			vprogram = 0;
			return false;
		}

		// call glCreateShader
		vprogram = glCreateShader(shaderType);

		// set the source code
		glShaderSource(vprogram, 1, &sourcecode, 0);

		// compile the shader
		glCompileShader(vprogram);

		// check if it compiled
		glGetShaderiv(vprogram, GL_COMPILE_STATUS, &result);
		if (result != 0)
		{
			return true; // succeeded
		}
		else
		{
			// get the log length
			glGetShaderiv(vprogram, GL_INFO_LOG_LENGTH, &length);

			// make a GLchar array of that length
			GLchar* elog = new GLchar[length];

			// get error info
			glGetShaderInfoLog(vprogram, length, 0, elog);

			// print the error log
			std::cout << elog << std::endl;

			// delete the shader and log array
			glDeleteShader(vprogram);
			delete[] elog;
		}
	}

	// load the fragment shader
	if (shaderType == GL_FRAGMENT_SHADER)
	{
		// read the sourcecode
		sourcecode = readBinaryFile(filenamef);

		// make sure it worked
		if (sourcecode == nullptr)
		{ 
			fprogram = 0;
			return false;
		}
			

		// call glCreateShader
		fprogram = glCreateShader(shaderType);

		// set the source code
		glShaderSource(fprogram, 1, &sourcecode, 0);

		// compile the shader
		glCompileShader(fprogram);

		// check if it compiled
		glGetShaderiv(fprogram, GL_COMPILE_STATUS, &result);
		if (result != 0)
		{
			return true; // succeeded
		}
		else
		{
			// get the log length
			glGetShaderiv(fprogram, GL_INFO_LOG_LENGTH, &length);

			// make a GLchar array of that length
			GLchar* elog = new GLchar[length];

			// get error info
			glGetShaderInfoLog(fprogram, length, 0, elog);

			// print the error log
			std::cout << elog << std::endl;

			// delete the shader and log array
			glDeleteShader(fprogram);
			delete[] elog;
		}
	}
	return false;
}

void Shader::use()
{
	 glUseProgram(program);
}

void Shader::unload()
{
	glDeleteProgram(program);
	glDeleteShader(vprogram);
	glDeleteShader(fprogram);
	program = 0;
	vprogram = 0;
	fprogram = 0;
}


char* Shader::readBinaryFile(std::string filename)
{
	// open in binary reading mode
	std::ifstream instream(filename, std::ios::binary);
	if (!instream.is_open())
	{
		// error opening
		std::cout << "Error opening shader file";
		return nullptr;
	}

	// get length
	instream.seekg(0, std::ios::end); // go to the end
	int length = instream.tellg();    // get the number of bytes (length)
	instream.seekg(0, std::ios::beg); // go to the beginning

	// read
	char* bytes = new char[length + 1];
	bytes[length] = '\0';
	instream.read(bytes, length);

	// close
	instream.close();

	return bytes;
}
