#pragma once
#include "Entity.h"
#include "Animation.h"

class Targetting :
	public Entity
{
public:
	Targetting(Entity *taraget);
	~Targetting();

	Entity *target;
	bool removeAni;

	void update(float dt);
};

