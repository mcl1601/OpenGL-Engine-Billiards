#pragma once
#include <ctime>
class Timer
{
private: 
	clock_t current, prev;
public:
	float t; // time since creation
	float dt; // delta time between frames
	Timer();
	~Timer();
	void update();
};

