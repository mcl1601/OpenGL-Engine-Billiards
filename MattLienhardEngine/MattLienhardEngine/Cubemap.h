#pragma once
#include "Texture.h"
class Cubemap :
	public Texture
{
private:
public:
	Cubemap();
	~Cubemap();
	void load() override;
	void use() override;
};

