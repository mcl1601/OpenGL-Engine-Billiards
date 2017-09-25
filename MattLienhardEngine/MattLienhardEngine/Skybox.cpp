#include "Skybox.h"
#include "GLError.h"
Skybox::Skybox()
{
	skyShader.load();
	cMap.load();
}


Skybox::~Skybox()
{
}

void Skybox::render()
{
	CHECK_GL_ERR;
	// disable backface culling
	glDisable(GL_CULL_FACE);
	// use the skybox shader
	skyShader.use();
	// render the object
	// use the cubemap
	cMap.use();
	
	// call the base render function
	Model::render();

	// clear the depth buffer
	glClear(GL_DEPTH_BUFFER_BIT);

	// reenable backface culling
	glEnable(GL_CULL_FACE);
}
