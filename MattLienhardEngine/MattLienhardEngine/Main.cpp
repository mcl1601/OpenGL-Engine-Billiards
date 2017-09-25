#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\euler_angles.hpp>
#include <FreeImage.h>
#include <vector>
#include <string>
#include "Shader.h"
#include "Timer.h"
#include <iostream>
#include "Camera.h"
#include "Model.h"
#include "Texture.h"
#include "Skybox.h"
#include "GLDebug.h"
#include "GLError.h"

// structs for Objects and Transforms


struct Transform {
	glm::vec3 location;
	glm::vec3 rotation;
	glm::mat4 rMat;
	glm::vec3 scale;
	glm::mat4 objWorldTrans;
	glm::vec3 velocity;
	float mass;
	void ApplyForce(glm::vec3 force, Timer time);
	bool UpdatePhysics(Timer time, int i);
	void CheckWallCollision();
	void BallCollision(glm::vec3 *targetLoc, glm::vec3 *targetVel);
	void LineUpdate(glm::vec3 *cueLoc);
	bool CheckPocketCollision(int i);
	void RotateBall(Timer time);
};

struct Object {
	Transform tran;
	Texture texture;
	Model model;
	void calcMatrix();
	void calcLineMatrix();
};

int main()
{
	// initialize the window library
	if (glfwInit() == GL_FALSE)
		return -1;
	
	// create a windowed mode window
	GLFWwindow* GLFWwindowPtr = glfwCreateWindow(800, 600, "Matt's DSA1 Engine", NULL, NULL);

	// make it the currently active window, or quit
	if (GLFWwindowPtr != nullptr)
		glfwMakeContextCurrent(GLFWwindowPtr);
	else
	{
		glfwTerminate();
		return -1;
	}

	// initialize GLEW or quit
	if (glewInit() != GLEW_OK)
	{
		glfwTerminate();
		return -1;
	}
	// enable the depth buffer
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	// set the screen clear color
	glClearColor(0.392f, 0.584f, 0.929, 1.0f);
	
	// hide the cursor
	glfwSetInputMode(GLFWwindowPtr, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	// set up the camera
	Camera cam = Camera(GLFWwindowPtr);

	// Set up the timer
	Timer myTimer = Timer();

	//make a collection of objects
	std::vector<Object> balls;

	// cue ball placement boolean
	bool scratch = false;

	//first object
	Object table;
	Texture tex1 = Texture("textures/PoolTableTexture.tga");
	tex1.load();
	table.texture = tex1;
	table.tran.location = glm::vec3(0, -1, 0);
	table.tran.scale = glm::vec3(0.15f, 0.15f, 0.15f);
	table.tran.rotation = glm::vec3(0,0,0);
	table.tran.objWorldTrans = glm::mat4();
	Model cube1;
	cube1.buffer("models/PoolTable.obj");
	table.model = cube1;

	Object cue;
	Texture cueTex = Texture("textures/cue.png");
	cueTex.load();
	cue.texture = cueTex;
	cue.tran.velocity = glm::vec3();
	cue.tran.mass = 1;
	cue.tran.location = glm::vec3(-1.75f, -0.82f, 0);
	cue.tran.scale = glm::vec3(0.1, 0.1, 0.1);
	cue.tran.rotation = glm::vec3(0, 0, 0);
	cue.tran.objWorldTrans = glm::mat4();
	cue.tran.rMat = glm::mat4();
	Model sphere;
	sphere.buffer("models/sphere.obj");
	cue.model = sphere;

#pragma region BallLoading
	// ball objects
	Object eight;
	Texture eightTex = Texture("textures/8ball.jpg");
	eightTex.load();
	eight.texture = eightTex;
	eight.tran.velocity = glm::vec3();
	eight.tran.mass = 1;
	eight.tran.location = glm::vec3(1.75f, -0.82f, 0);
	eight.tran.scale = glm::vec3(0.1, 0.1, 0.1);
	eight.tran.rotation = glm::vec3(0, 0, 0);
	eight.tran.objWorldTrans = glm::mat4();
	eight.tran.rMat = glm::mat4();
	eight.model = sphere;
	balls.push_back(eight);

	Object one;
	Texture oneTex = Texture("textures/one.jpg");
	oneTex.load();
	one.texture = oneTex;
	one.tran.velocity = glm::vec3();
	one.tran.mass = 1;
	one.tran.location = glm::vec3(1.35, -0.82f, 0);
	one.tran.scale = glm::vec3(0.1, 0.1, 0.1);
	one.tran.rotation = glm::vec3(0, 0, 0);
	one.tran.objWorldTrans = glm::mat4();
	one.tran.rMat = glm::mat4();
	one.model = sphere;
	balls.push_back(one);
	
	Object two;
	Texture twoTex = Texture("textures/two.jpg");
	twoTex.load();
	two.texture = twoTex;
	two.tran.velocity = glm::vec3();
	two.tran.mass = 1;
	two.tran.location = glm::vec3(1.55, -0.82f, 0.125f);
	two.tran.scale = glm::vec3(0.1, 0.1, 0.1);
	two.tran.rotation = glm::vec3(0, 0, 0);
	two.tran.objWorldTrans = glm::mat4();
	two.tran.rMat = glm::mat4();
	two.model = sphere;
	balls.push_back(two);

	Object three;
	Texture threeTex = Texture("textures/three.jpg");
	threeTex.load();
	three.texture = threeTex;
	three.tran.velocity = glm::vec3();
	three.tran.mass = 1;
	three.tran.location = glm::vec3(1.55f, -0.82f, -0.125f);
	three.tran.scale = glm::vec3(0.1, 0.1, 0.1);
	three.tran.rotation = glm::vec3(0, 0, 0);
	three.tran.objWorldTrans = glm::mat4();
	three.tran.rMat = glm::mat4();
	three.model = sphere;
	balls.push_back(three);
	
	Object four;
	Texture fourTex = Texture("textures/four.jpg");
	fourTex.load();
	four.texture = fourTex;
	four.tran.velocity = glm::vec3();
	four.tran.mass = 1;
	four.tran.location = glm::vec3(1.75f, -0.82f, 0.25f);
	four.tran.scale = glm::vec3(0.1, 0.1, 0.1);
	four.tran.rotation = glm::vec3(0, 0, 0);
	four.tran.objWorldTrans = glm::mat4();
	four.tran.rMat = glm::mat4();
	four.model = sphere;
	balls.push_back(four);

	Object five;
	Texture fiveTex = Texture("textures/five.jpg");
	fiveTex.load();
	five.texture = fiveTex;
	five.tran.velocity = glm::vec3();
	five.tran.mass = 1;
	five.tran.location = glm::vec3(1.75f, -0.82f, -0.25f);
	five.tran.scale = glm::vec3(0.1, 0.1, 0.1);
	five.tran.rotation = glm::vec3(0, 0, 0);
	five.tran.objWorldTrans = glm::mat4();
	five.tran.rMat = glm::mat4();
	five.model = sphere;
	balls.push_back(five);

	Object six;
	Texture sixTex = Texture("textures/six.jpg");
	sixTex.load();
	six.texture = sixTex;
	six.tran.velocity = glm::vec3();
	six.tran.mass = 1;
	six.tran.location = glm::vec3(1.95f, -0.82f, 0.375f);
	six.tran.scale = glm::vec3(0.1, 0.1, 0.1);
	six.tran.rotation = glm::vec3(0, 0, 0);
	six.tran.objWorldTrans = glm::mat4();
	six.tran.rMat = glm::mat4();
	six.model = sphere;
	balls.push_back(six);

	Object seven;
	Texture sevenTex = Texture("textures/seven.jpg");
	sevenTex.load();
	seven.texture = sevenTex;
	seven.tran.velocity = glm::vec3();
	seven.tran.mass = 1;
	seven.tran.location = glm::vec3(1.95f, -0.82f, 0.125f);
	seven.tran.scale = glm::vec3(0.1, 0.1, 0.1);
	seven.tran.rotation = glm::vec3(0, 0, 0);
	seven.tran.objWorldTrans = glm::mat4();
	seven.tran.rMat = glm::mat4();
	seven.model = sphere;
	balls.push_back(seven);

	Object nine;
	Texture nineTex = Texture("textures/nine.jpg");
	nineTex.load();
	nine.texture = nineTex;
	nine.tran.velocity = glm::vec3();
	nine.tran.mass = 1;
	nine.tran.location = glm::vec3(1.95f, -0.82f, -0.125f);
	nine.tran.scale = glm::vec3(0.1, 0.1, 0.1);
	nine.tran.rotation = glm::vec3(0, 0, 0);
	nine.tran.objWorldTrans = glm::mat4();
	nine.tran.rMat = glm::mat4();
	nine.model = sphere;
	balls.push_back(nine);

	Object ten;
	Texture tenTex = Texture("textures/ten.jpg");
	tenTex.load();
	ten.texture = tenTex;
	ten.tran.velocity = glm::vec3();
	ten.tran.mass = 1;
	ten.tran.location = glm::vec3(1.95f, -0.82f, -0.375f);
	ten.tran.scale = glm::vec3(0.1, 0.1, 0.1);
	ten.tran.rotation = glm::vec3(0, 0, 0);
	ten.tran.objWorldTrans = glm::mat4();
	ten.tran.rMat = glm::mat4();
	ten.model = sphere;
	balls.push_back(ten);

	Object eleven;
	Texture elevenTex = Texture("textures/eleven.jpg");
	elevenTex.load();
	eleven.texture = elevenTex;
	eleven.tran.velocity = glm::vec3();
	eleven.tran.mass = 1;
	eleven.tran.location = glm::vec3(2.15f, -0.82, 0.5f);
	eleven.tran.scale = glm::vec3(0.1, 0.1, 0.1);
	eleven.tran.rotation = glm::vec3(0, 0, 0);
	eleven.tran.objWorldTrans = glm::mat4();
	eleven.tran.rMat = glm::mat4();
	eleven.model = sphere;
	balls.push_back(eleven);

	Object twelve;
	Texture twelveTex = Texture("textures/twelve.jpg");
	twelveTex.load();
	twelve.texture = twelveTex;
	twelve.tran.velocity = glm::vec3();
	twelve.tran.mass = 1;
	twelve.tran.location = glm::vec3(2.15f, -0.82f, 0.25f);
	twelve.tran.scale = glm::vec3(0.1, 0.1, 0.1);
	twelve.tran.rotation = glm::vec3(0, 0, 0);
	twelve.tran.objWorldTrans = glm::mat4();
	twelve.tran.rMat = glm::mat4();
	twelve.model = sphere;
	balls.push_back(twelve);

	Object thirteen;
	Texture thirteenTex = Texture("textures/thirteen.jpg");
	thirteenTex.load();
	thirteen.texture = thirteenTex;
	thirteen.tran.velocity = glm::vec3();
	thirteen.tran.mass = 1;
	thirteen.tran.location = glm::vec3(2.15f, -0.82f, 0);
	thirteen.tran.scale = glm::vec3(0.1, 0.1, 0.1);
	thirteen.tran.rotation = glm::vec3(0, 0, 0);
	thirteen.tran.objWorldTrans = glm::mat4();
	thirteen.tran.rMat = glm::mat4();
	thirteen.model = sphere;
	balls.push_back(thirteen);

	Object fourteen;
	Texture fourteenTex = Texture("textures/fourteen.jpg");
	fourteenTex.load();
	fourteen.texture = fourteenTex;
	fourteen.tran.velocity = glm::vec3();
	fourteen.tran.mass = 1;
	fourteen.tran.location = glm::vec3(2.15f, -0.82f, -0.25f);
	fourteen.tran.scale = glm::vec3(0.1, 0.1, 0.1);
	fourteen.tran.rotation = glm::vec3(0, 0, 0);
	fourteen.tran.objWorldTrans = glm::mat4();
	fourteen.tran.rMat = glm::mat4();
	fourteen.model = sphere;
	balls.push_back(fourteen);

	Object fifteen;
	Texture fifteenTex = Texture("textures/fifteen.jpg");
	fifteenTex.load();
	fifteen.texture = fifteenTex;
	fifteen.tran.velocity = glm::vec3();
	fifteen.tran.mass = 1;
	fifteen.tran.location = glm::vec3(2.15f, -0.82f, -0.5f);
	fifteen.tran.scale = glm::vec3(0.1, 0.1, 0.1);
	fifteen.tran.rotation = glm::vec3(0, 0, 0);
	fifteen.tran.objWorldTrans = glm::mat4();
	fifteen.tran.rMat = glm::mat4();
	fifteen.model = sphere;
	balls.push_back(fifteen);
	
#pragma endregion

	Object line;
	Texture lineTex = Texture("textures/blue.png");
	lineTex.load();
	line.texture = lineTex;
	line.tran.mass = 1;
	line.tran.velocity = glm::vec3();
	line.tran.location = glm::vec3(0, 0, 0);
	line.tran.scale = glm::vec3(150, 2, 0.002f);
	line.tran.rotation = glm::vec3(0, 0, 0);
	line.tran.objWorldTrans = glm::mat4();
	Model line1;
	line1.buffer("models/line.obj");
	line.model = line1;
	
	Object winner;
	Texture winTex = Texture("textures/win.jpg");
	winTex.load();
	winner.texture = winTex;
	winner.tran.location = glm::vec3(100, 100, 100);
	winner.tran.scale = glm::vec3(1, 1, 1);
	winner.tran.mass = 1;
	winner.tran.velocity = glm::vec3();
	winner.tran.rotation = glm::vec3();
	winner.tran.objWorldTrans = glm::mat4();
	Model cube;
	cube.buffer("models/cube.obj");
	winner.model = cube;

	Texture red = Texture("textures/red.jpg");
	red.load();

	// load the skybox
	Skybox sky = Skybox();
	sky.buffer("models/cube.obj");

	// load the texture
	Texture candle;
	candle.load();

	// wireframe model renderer
	bool wireframe = false;
	bool currentPress = false;
	bool prevPress = false;
	
	// load the shader
	Shader shade = Shader();
	bool result = shade.load();
	if (!result)
	{
		// didn't load. Keep console window open
		std::cin.get();
	}
	else
	// loop until user closes window
	while (!glfwWindowShouldClose(GLFWwindowPtr))
	{
		// update physical simulation
		myTimer.update();
		
		
		if (myTimer.t > 0.5f)
			myTimer.t = 0;
		
		// clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		if (wireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		// use the shader
		shade.use();

		// render the table
		table.calcMatrix();
		glUniformMatrix4fv(15, 1, GL_FALSE, &table.tran.objWorldTrans[0][0]);
		// use the texture
		table.texture.use();
		// render the model
		table.model.render();

#pragma region Scratch
// scratch placement
		if (scratch)
		{
			if (glfwGetKey(GLFWwindowPtr, GLFW_KEY_UP))
			{
				cue.tran.location.x += 0.05f;
			}
			if (glfwGetKey(GLFWwindowPtr, GLFW_KEY_DOWN))
			{
				cue.tran.location.x -= 0.05f;
			}
			if (glfwGetKey(GLFWwindowPtr, GLFW_KEY_RIGHT))
			{
				cue.tran.location.z += 0.05f;
			}
			if (glfwGetKey(GLFWwindowPtr, GLFW_KEY_LEFT))
			{
				cue.tran.location.z -= 0.05f;
			}

			if (glfwGetKey(GLFWwindowPtr, GLFW_KEY_ENTER))
				scratch = false;
		}
#pragma endregion

#pragma region LineRotation
else
		{

			if (glfwGetKey(GLFWwindowPtr, GLFW_KEY_Q))
			{
				if (glfwGetKey(GLFWwindowPtr, GLFW_KEY_LEFT_SHIFT))
					line.tran.rotation.x += 0.075f;

				else if (glfwGetKey(GLFWwindowPtr, GLFW_KEY_LEFT_CONTROL))
					line.tran.rotation.x += 0.005f;
				
				else
					line.tran.rotation.x += 0.025f;
			}

			if (glfwGetKey(GLFWwindowPtr, GLFW_KEY_E))
			{
				if (glfwGetKey(GLFWwindowPtr, GLFW_KEY_LEFT_SHIFT))
					line.tran.rotation.x -= 0.075f;

				else if (glfwGetKey(GLFWwindowPtr, GLFW_KEY_LEFT_CONTROL))
					line.tran.rotation.x -= 0.005f;

				else
					line.tran.rotation.x -= 0.025f;
			}

			if (glfwGetKey(GLFWwindowPtr, GLFW_KEY_UP))
			{
				
				if (glfwGetKey(GLFWwindowPtr, GLFW_KEY_LEFT_SHIFT))
					line.tran.scale.x += 2.0f;

				else if (glfwGetKey(GLFWwindowPtr, GLFW_KEY_LEFT_CONTROL))
					line.tran.scale.x += 0.5f;

				else
					line.tran.scale.x++;
			}
			if (glfwGetKey(GLFWwindowPtr, GLFW_KEY_DOWN))
			{
				if (glfwGetKey(GLFWwindowPtr, GLFW_KEY_LEFT_SHIFT))
					line.tran.scale.x -= 2.0f;

				else if (glfwGetKey(GLFWwindowPtr, GLFW_KEY_LEFT_CONTROL))
					line.tran.scale.x -= 0.5f;

				else
					line.tran.scale.x--;
			}
		}
#pragma endregion

#pragma region LineRender
		line.tran.LineUpdate(&cue.tran.location);

		if (!(cue.tran.velocity.x < 0.01f && cue.tran.velocity.x > -0.01f) && !(cue.tran.velocity.z < 0.01f && cue.tran.velocity.z > -0.01f) || scratch)
			line.tran.location.y = 100.0f;

		else if(glfwGetKey(GLFWwindowPtr, GLFW_KEY_SPACE))
			cue.tran.ApplyForce((line.tran.location - cue.tran.location) * line.tran.scale.x * 5.0f, myTimer);

		line.calcLineMatrix();
		glUniformMatrix4fv(15, 1, GL_FALSE, &line.tran.objWorldTrans[0][0]);
		// use the texture
		line.texture.use();
		// render the model
		line.model.render();
#pragma endregion

#pragma region CueRender
		// render the cueball
		cue.calcMatrix();
		glUniformMatrix4fv(15, 1, GL_FALSE, &cue.tran.objWorldTrans[0][0]);
				
		if (scratch)
		{
			if (myTimer.t < 0.25f)
				cue.texture = red;
			else
				cue.texture = cueTex;
		}
		else
			cue.texture = cueTex;
		// use the texture
		cue.texture.use();
		// render the model
		cue.model.render();
		if(!scratch)
			scratch = cue.tran.UpdatePhysics(myTimer, 1);
		else
			cue.tran.UpdatePhysics(myTimer, 0);
		if(!scratch)
		{
			for (int i = 0; i < balls.size(); i++)
			{
				cue.tran.BallCollision(&balls[i].tran.location, &balls[i].tran.velocity);
				
			}
		}
		
#pragma endregion

#pragma region BallUpdate
// update the models transforms
		for (int i = 0; i < balls.size(); i++)
		{
			for (int j = 0; j < balls.size(); j++)
			{
				if (i != j) {
					balls[i].tran.BallCollision(&balls[j].tran.location, &balls[j].tran.velocity);
				}
			}

			if (balls[i].tran.UpdatePhysics(myTimer, 0))
			{
				balls.erase(balls.begin() + i);
				continue;
			}
				
			balls[i].calcMatrix();
			
			glUniformMatrix4fv(15, 1, GL_FALSE, &balls[i].tran.objWorldTrans[0][0]);

			// use the texture
			balls[i].texture.use();
			
			// render the model
			balls[i].model.render();
		}
#pragma endregion

		
		// check for a win
		if (balls.size() == 0)
		{
			// winner, move the camera
			winner.calcMatrix();
			glUniformMatrix4fv(15, 1, GL_FALSE, &winner.tran.objWorldTrans[0][0]);
			// use the texture
			winner.texture.use();
			// render the model
			winner.model.render();
			cam.location = glm::vec3(98.85f, 100, 100);
			cam.rotation = glm::vec3(0, -3.14159265f / 2.0f, 0);
		}

		// use the shader
		shade.use();
		// update the camera
		cam.update(GLFWwindowPtr);
		
		// swap the front and back buffers
		glfwSwapBuffers(GLFWwindowPtr);

		// process queued window, mouse & keyboard callback events
		glfwPollEvents();

		if (glfwGetKey(GLFWwindowPtr, GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(GLFWwindowPtr, GL_TRUE);

		// swap between wireframe modes
		currentPress = false;
		if (glfwGetKey(GLFWwindowPtr, GLFW_KEY_Z))
		{
			currentPress = true;
			if (currentPress && !prevPress)
			wireframe = !wireframe;
		}
		prevPress = currentPress;
	}

	table.texture.unload();
	cue.texture.unload();
	for (int i = 0; i < balls.size(); i++)
	{
		balls[i].texture.unload();
	}
	glfwTerminate();
	return 0;
}

void Object::calcMatrix()
{
	// calculate the model-world matrix
	tran.objWorldTrans = glm::translate(tran.location) * // translation
						 tran.rMat * // rotation
						 glm::scale(tran.scale); // scale
}

void Object::calcLineMatrix()
{
	// calculate the model-world matrix
	tran.objWorldTrans = glm::translate(tran.location) * // translation
						 glm::yawPitchRoll(tran.rotation.x, tran.rotation.y, tran.rotation.z) * // rotation
						 glm::scale(tran.scale); // scale
}

void Transform::ApplyForce(glm::vec3 force, Timer time)
{
	// F= m / a
	glm::vec3 accel = force / mass;
	
	// a = dv / dt
	glm::vec3 dv = accel * time.dt;
	velocity += dv;

	// v = dr / dt
	glm::vec3 dr = velocity * time.dt;
	location += dr;
}

bool Transform::UpdatePhysics(Timer time, int i)
{
	bool cuePocket = false;

	CheckWallCollision();

	if (CheckPocketCollision(i))
		cuePocket = true;

	if (velocity != glm::vec3())
	{
		// slow down
		ApplyForce(-velocity * 0.5f, time);
		if (glm::length(velocity) < 0.1f)
			velocity = glm::vec3(0, 0, 0);
	}

	RotateBall(time);

	return cuePocket;
}

void Transform::CheckWallCollision()
{
	if (location.x > 2.95 && location.z > -1.35 && location.z < 1.35)
	{
		location = glm::vec3(2.95, location.y, location.z);
		velocity = glm::vec3(-velocity.x, velocity.y, velocity.z)  * 0.8f;
	}
	if (location.x < -2.95 && location.z > -1.35 && location.z < 1.35)
	{
		location = glm::vec3(-2.95, location.y, location.z);
		velocity = glm::vec3(-velocity.x, velocity.y, velocity.z)  * 0.8f;
	}
	if ((location.z < -1.45 && location.x > -2.9 && location.x < -0.2f) || ((location.z < -1.45 && location.x < 2.9 && location.x > 0.2f)))
	{
		location = glm::vec3(location.x, location.y, -1.45);
		velocity = glm::vec3(velocity.x, velocity.y, -velocity.z)  * 0.8f;
	}
	if ((location.z > 1.45 && location.x > -2.9 && location.x < -0.2f) || ((location.z > 1.45 && location.x < 2.9 && location.x > 0.2f)))
	{
		location = glm::vec3(location.x, location.y, 1.45);
		velocity = glm::vec3(velocity.x, velocity.y, -velocity.z) * 0.8f;
	}
}
void Transform::BallCollision(glm::vec3 * targetLoc, glm::vec3 * targetVel)
{
	// check for a collision
	float distance = glm::length(*targetLoc - location);
	if (distance < 0.2f)
	{
		glm::vec3 between = glm::normalize(*targetLoc - location);
		float vProj = glm::dot(velocity, between);
		*targetVel += vProj * glm::normalize(between);
		velocity -= vProj * glm::normalize(between);

		// move the ball so it's no longer colliding
		// vector thats overalapping added/subtracted to location / 2
		glm::vec3 radius = glm::normalize(*targetLoc - location) * 0.1f;
		glm::vec3 difference = (location + radius) - (*targetLoc - radius);

		// adjust locations
		location -= difference / 2.0f;
		location.y = -0.82f;
	}
}
void Transform::LineUpdate(glm::vec3 *cueLoc)
{
	// limit scale
	if (scale.x > 400.0f)
		scale.x = 400.0f;
	if (scale.x < 1.0f)
		scale.x = 1.0f;

	// move the line to the cue
	glm::vec3 unit = glm::vec3(1.0f * sin(rotation.x + (3.1415926f / 2.0f)), 0, 1.0f * cos(rotation.x + (3.1415926f / 2.0f))) * (scale.x / 155.0f);
	location = *cueLoc + (unit - (unit * 0.55f));
}

bool Transform::CheckPocketCollision(int i)
{
	if ((location.x < 0.2f && location.x > -0.2f && location.z > 1.5f) || (location.x < 0.2f && location.x > -0.2f && location.z < -1.5f) ||
		((location.x > 3.0f && location.z > 1.4f) || (location.x < -3.0f && location.z > 1.4f)) || ((location.x > 3.0f && location.z < -1.4f) || (location.x < -3.0f && location.z < -1.4f)))
	{
		velocity = glm::vec3();
		location.y = 100.0f;

		if (i == 1)
		{
			location = glm::vec3(-1.75f, -0.82f, 0);
			
		}
		return true;
	}
	return false;
}

void Transform::RotateBall(Timer time)
{
	// make sure it has velocity
	if (velocity == glm::vec3()) return;

	// find the angle
	float angle = glm::length(velocity) * 0.1f;

	// get the axis of rotation
	glm::vec3 axis = glm::normalize(glm::cross(glm::vec3(0, 1, 0), velocity));

	// rotate the object
	rMat = glm::rotate(angle, axis) * rMat;
}
