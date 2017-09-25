#pragma once
#include "Model.h"
#include "SkyboxShader.h"
#include "Cubemap.h"
class Skybox :
	public Model
{
private:
	SkyboxShader skyShader;
	Cubemap cMap;
	
public:
	Skybox();
	~Skybox();
	void render() override;
};

