#pragma once
#include "Entity.h"
#include "Timer.h"

class EnemyManager :
	public Entity
{
public:
	EnemyManager();
	~EnemyManager();

	void update(float dt);

	Timer groupTimer1;
	Timer groupTimer2;
	Timer groupTimer3;

	float patternTimer;
	int pattern;
};

