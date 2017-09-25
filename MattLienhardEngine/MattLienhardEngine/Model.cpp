#include "Model.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
// build a vertex out of 3 indices
struct Vertex {
	glm::vec3 location;
	glm::vec2 uv;
	glm::vec3 normal;
};

struct VertInd {
	unsigned int locInd;
	unsigned int uvInd;
	unsigned int normInd;
};


Model::Model()
{
	vertCount = vertArr = 0;
}


Model::~Model()
{
}

bool Model::buffer(std::string objFile)
{
	// 4 vector collections
	std::vector<glm::vec3> locations;
	std::vector<glm::vec2> UV;
	std::vector<glm::vec3> normals;
	std::vector<VertInd> vertInd;

	// open in normal reading mode
	std::ifstream instream(objFile);
	if (!instream.is_open()) return false;

	// read in data one line at a time
	std::string input;
	while (std::getline(instream, input))
	{
		// make a string stream of the input
		std::istringstream ss(input);

		// read the label from the line
		std::string label;
		ss >> label;

		// compare the label to see what to do
		if (label == "v")
		{
			// make a new vec3 of the floats, add it to locations
			float x, y, z;
			ss >> x >> y >> z;
			locations.push_back(glm::vec3(x, y, z));
		}
		if (label == "vt")
		{
			// read two floats and add it to UV
			float x, y;
			ss >> x >> y;
			UV.push_back(glm::vec2(x, y));
		}
		if (label == "vn")
		{
			// read 3 floats, add it to normals
			float x, y, z;
			ss >> x >> y >> z;
			normals.push_back(glm::vec3(x, y, z));
		}
		if (label == "f")
		{
			// loop three times
			for (int i = 0; i < 3; i++)
			{
				unsigned int num1, num2, num3;
				char slash;
				ss >> num1 >> slash >> num2 >> slash >> num3;
				num1--;
				num2--;
				num3--;
				
				// assign them to a vertInd
				VertInd temp;
				temp.locInd = num1;
				temp.uvInd = num2;
				temp.normInd = num3;

				// put them in the vertInd vector
				vertInd.push_back(temp);
			}
		}
	}

	// close the file
	instream.close();

	// fill the vertex buffer with vertices
	vertCount = vertInd.size();
	std::vector<Vertex> vertBufData(vertCount);
	// loop through the vertInds
	
	for (unsigned int i = 0; i < vertCount; i++)
	{
		vertBufData[i] = { locations[vertInd[i].locInd],
							UV[vertInd[i].uvInd],
							normals[vertInd[i].normInd] };
	}

	// upload the material
	/*glm::vec4 material = { 0.25, 1, 1, 32 };
	glUniform4f(6, material.x, material.y, material.z, material.w);*/

	// generate a buffer and array
	GLuint vertBuf;
	glGenVertexArrays(1, &vertArr);
	glGenBuffers(1, &vertBuf);

	// bind the buffer
	glBindVertexArray(vertArr);
	glBindBuffer(GL_ARRAY_BUFFER, vertBuf);

	// store data in the buffer
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(Vertex) * vertCount,
		&vertBufData[0],
		GL_STATIC_DRAW);

	// describe the buffer layout
	// enable the attribbute array
	glEnableVertexAttribArray(0);
	
	// Vertex Attributes
	// set up the attribute
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		0);
	
	// Model Normal Attribute
	// enable the attribbute array
	glEnableVertexAttribArray(7);
	glVertexAttribPointer(
		7,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(sizeof(glm::vec3)+sizeof(glm::vec2)));

	// UV Attribute
	// enable the attribbute array
	glEnableVertexAttribArray(8);
	glVertexAttribPointer(
		8,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)sizeof(glm::vec3));

	// unbind when finished
	glBindVertexArray(0);

	

	return true;
}

void Model::render()
{
	// render the game objects
	glBindVertexArray(vertArr);
	glDrawArrays(GL_TRIANGLES, 0, vertCount);
	// unbind when finished
	glBindVertexArray(0);
}
