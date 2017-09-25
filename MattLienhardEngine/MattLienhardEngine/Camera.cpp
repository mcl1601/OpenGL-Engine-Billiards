#include "Camera.h"
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\euler_angles.hpp>
#include <FreeImage.h>
#include <vector>


Camera::Camera()
{
}

Camera::Camera(GLFWwindow* window)
{
	// get the width and height of the screen
	width = 0;
	height = 0;
	glfwGetWindowSize(window, &width, &height);

	// set the window to the middle of the screen
	glfwSetCursorPos(window, width / 2, height / 2);
	
	// set initial values
	camMat = glm::mat4();
	FoV = aspectRatio = nearPlane = farPlane = 0;
	rotation = glm::vec3(0, -3.14159265f / 2.0f, 0);
	location = glm::vec3(-3.5f, 2, 0);
	velocity = glm::vec3(0, 0, 0);
	current = previous = dt = 0;
}


Camera::~Camera()
{
}

void Camera::update(GLFWwindow* window)
{

	// get the time
	previous = current;
	current = (float)glfwGetTime();
	dt = current - previous;

	// calculate the new matrix
	camMat = calcPerspMat() * calcLookAtMat();

	// upload the matrix
	glUniformMatrix4fv(1, 1, GL_FALSE, &camMat[0][0]);

	// upload the camera position and light position
	glUniform3f(5, location.x, location.y, location.z);
	glUniform3f(4, 10, 10, 10);

	// update the FPS
	float sens = 0.005f;
	double x, y;
	glfwGetCursorPos(window, &x, &y);

	rotation.y -= sens * (x - width * 0.5f); // yaw
	rotation.x -= sens * (y - height * 0.5f); // pitch
	rotation.x = glm::clamp(rotation.x, -0.5f * 3.1415926f, 0.5f * 3.1415926f);

	// move with arrow keys
	glm::mat3 R = (glm::mat3)glm::yawPitchRoll(rotation.y, rotation.x, rotation.z);
	
	// stop moving if you don't press anything
	velocity = glm::vec3();

	if (glfwGetKey(window, GLFW_KEY_A))
		velocity += R * glm::vec3(-1, 0, 0);

	if (glfwGetKey(window, GLFW_KEY_D))
		velocity += R * glm::vec3(1, 0, 0);

	if (glfwGetKey(window, GLFW_KEY_W))
		velocity += R * glm::vec3(0, 0, -1);

	if (glfwGetKey(window, GLFW_KEY_S))
		velocity += R * glm::vec3(0, 0, 1);

	/*if (glfwGetKey(window, GLFW_KEY_Q))
		velocity += R * glm::vec3(0, 1, 0);

	if (glfwGetKey(window, GLFW_KEY_E))
		velocity += R * glm::vec3(0, -1, 0);
		*/
	// limit velocity
	if (velocity != glm::vec3())
		velocity = glm::normalize(velocity);

	// add velocity to position
	location += velocity * dt;

	// set the cursor
	glfwSetCursorPos(window, width * 0.5f, height * 0.5f);
}

glm::mat4 Camera::calcLookAtMat()
{
	// find the rotation matrix using the rotation vector
	glm::mat3 rotMat = (glm::mat3)glm::yawPitchRoll(rotation.y, rotation.x, rotation.z);

	// calculate the vectors for the lookAt method
	glm::vec3 eye = location;
	glm::vec3 center = eye + rotMat * glm::vec3(0, 0, -1);
	glm::vec3 up = rotMat * glm::vec3(0, 1, 0);

	// return the resulting mat4
	return glm::lookAt(eye, center, up);
}

glm::mat4 Camera::calcPerspMat()
{
	// declare the field of view
	FoV = 3.14159f * .4f;

	// calculate the aspect ratio
	aspectRatio = (float)width / (float)height;

	// declare the near and far planes
	nearPlane = 0.01f;
	farPlane = 1000.0f;

	// return the perspective matrix
	return glm::perspective(FoV, aspectRatio, nearPlane, farPlane);
}


