#include "Timer.h"



Timer::Timer()
{
	prev = current = clock();
	t = dt = 0;
}


Timer::~Timer()
{
}

void Timer::update()
{
	prev = current;
	current = clock();
	dt = (current - prev) / (float)CLOCKS_PER_SEC;
	t += dt;
}
