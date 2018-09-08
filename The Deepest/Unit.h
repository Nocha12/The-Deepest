#pragma once
#include "Entity.h"
class Unit :
	public Entity
{
public:
	Unit();
	~Unit();

	float hp;
	float maxHp;
	float speed;
};

