#pragma once
#include "Enemy.h"
#include "Timer.h"

class Boss2 :
	public Enemy
{
public:
	Boss2();
	~Boss2();
	
	Timer dropBulletTimer;

	void update(float dt);
};

