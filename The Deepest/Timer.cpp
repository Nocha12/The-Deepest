#include "DXUT.h"
#include "Timer.h"
#include "World.h"

Timer::Timer() : active(false)
{
	onTick = [](){};
	onFinished = [](){}; 
	world.timers.push_back(this);
}

Timer::~Timer()
{
	world.timers.remove(this);
}

void Timer::reset(float duration, int count)
{
	this->duration = pair<float, float>(0, duration);
	this->count = pair<int, int>(0, count);
	active = true;
}

void Timer::update(float dt)
{
	if (!active) return;

	duration.first += dt;
	if (duration.first > duration.second)
	{
		duration.first = 0;
		onTick();
		if (++count.first == count.second)
		{
			onFinished();
			active = false;
			return;
		}
	}
}

