#pragma once
#include "Entity.h"
#include "Timer.h"

class Scene :
	public Entity
{
public:
	Scene();
	~Scene();
	
	void follow(Entity *t, Vec2 offset = Vec2(1280, 720) / 2);
	void unfollow();
	void shake(float duration, int count, float range, bool reset = false);

	void update(float dt);

	Entity *target;
	Vec2 offset;
	Timer shakeTimer;
};

