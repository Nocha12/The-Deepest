#pragma once
#include "Entity.h"
#include "Sprite.h"

class Particle :
	public Entity
{
public:
	Particle(float angle, float speed, Entity *target);
	~Particle();

	float angle;
	Entity *target;
	float timer;
	float speed;

	void update(float dt);
};

