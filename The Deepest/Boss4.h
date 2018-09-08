#pragma once
#include "Enemy.h"
class Boss4 :
	public Enemy
{
public:
	Boss4();
	~Boss4();
	
	void update(float dt);
};

