#include "DXUT.h"
#include "Scene.h"

Scene::Scene() : target(nullptr)
{
}

Scene::~Scene()
{
}

void Scene::follow(Entity *t, Vec2 offset)
{
	target = t;
	this->offset = offset;
}

void Scene::shake(float duration, int count, float range, bool reset)
{
	if (reset || !shakeTimer.active)
	{
		shakeTimer.reset(duration, count);
		shakeTimer.onTick = [=](){
			pos += Vec2(random(-range, range), random(-range, range));
		};
	}
}

void Scene::unfollow()
{
	target = nullptr;
}

void Scene::update(float dt)
{
	Entity::update(dt);
	if (target)
		pos += (-Vec2(target->center().x, 0) + Vec2(offset.x, 0) - pos) * 0.15;
}
