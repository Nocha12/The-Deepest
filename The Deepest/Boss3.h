#pragma once
#include "Enemy.h"
#include "Timer.h"
class Boss3 :
	public Enemy
{
public:
	Boss3();
	~Boss3();

	void update(float dt);
	bool isAttacked3;

	Timer armDropTimer;
};



