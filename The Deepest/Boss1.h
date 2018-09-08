#pragma once
#include "Enemy.h"
class Boss1 :
	public Enemy
{
public:
	Boss1();
	~Boss1();

	float rushCount;

	void update(float dt);
};

