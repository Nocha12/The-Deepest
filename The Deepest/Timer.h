#pragma once
#include "Define.h"

class Timer
{
public:
	Timer();
	~Timer();

	void reset(float duration, int count);
	void update(float dt);

	function<void()> onTick;
	function<void()> onFinished;
	bool active;
	pair<float, float> duration;
	pair<int, int> count;
};

