#pragma once
#include "Unit.h"
#include "Animation.h"
#include "Timer.h"

class Enemy :
	public Unit
{
public:
	Enemy();
	~Enemy();

	void update(float dt);

	bool isAttacked1;
	bool isAttacked2;
	int patternCount;
	bool isAttacking1;
	bool isAttacking2;

	EnemyType type;

	float tum;

	map<AnimationType, Animation*> ani;

	float angle;
	Timer redTimer;
	void GetRed();
};

class Octo :
	public Enemy
{
public:
	Octo(float angle, float speed);
	~Octo();

	Timer attackTimer;
	Timer attackReadyTimer;

	void update(float dt) override;
};

class Rush :
	public Enemy
{
public:
	Rush();
	~Rush();

	Timer afterImageTimer;

	void update(float dt);
};

class Fish :
	public Enemy
{
public:
	Fish(float angle, float speed);
	~Fish();

	Timer attackTimer;
};