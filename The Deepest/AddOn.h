#pragma once
#include "Entity.h"
#include "Animation.h"

class AddOn :
	public Entity
{
public:
	AddOn();
	~AddOn();

	Animation *ani;
	void update(float dt);

	bool canShoot;
	float angle;
	pair<float, float> shootDelay;
};

