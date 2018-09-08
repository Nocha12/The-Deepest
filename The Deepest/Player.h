#pragma once
#include "Unit.h"
#include "Animation.h"
#include "AddOn.h"
#include "Timer.h"

class Player : public Unit
{
public:
	Player();
	~Player();

	Sprite *img;
	Animation *ani;

	float moveAni;
	float effTimer;
	int shootWaterZet;

	AddOn *addOn[4];

	void update(float dt);

	pair<float, float> shootTimer;
	pair<float, float> waterZetTimer;
	pair<float, float> waterZet3Timer;
	pair<float, float> homingWaterZetTimer;

	Timer afterImageTimer;
	Timer speedUpTimer;
	float bubbleTimer;

	Vec2 left, right, top, bottom;

	float currentSpeed;
	float clearSpeed;

	void GetRed();
	void GetBlack();
	Timer redTimer;
	Timer blackTimer;
};

