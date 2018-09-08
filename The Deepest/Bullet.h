#pragma once
#include "Entity.h"
#include "Animation.h"
#include "Timer.h"

class Bullet :
	public Entity
{
public:
	Bullet(BulletType type, float speed, float angle, float speedRate, float angleRate, float damage = 10, Entity *target = nullptr);
	~Bullet(); 

	float damage;
	Timer afterImageTimer;
	bool isPlayerBullet;
	BulletType type;
	float speed; 
	float angle;
	float speedRate;
	float angleRate;
	Entity *target;
	float homingTimer;
	void update(float dt);
};

