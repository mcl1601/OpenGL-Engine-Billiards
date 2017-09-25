#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\euler_angles.hpp>
#include <FreeImage.h>
#include <vector>
class Camera
{
private:
	glm::mat4 camMat;
	float FoV;
	float aspectRatio;
	float nearPlane;
	float farPlane;
	
	glm::vec3 velocity;
	int width;
	int height;

	// time variables
	float current;
	float previous;
	float dt;
public:
	Camera();
	Camera(GLFWwindow* window);
	~Camera();
	void update(GLFWwindow* window);
	glm::mat4 calcLookAtMat();
	glm::mat4 calcPerspMat();
	glm::vec3 rotation;
	glm::vec3 location;
};

