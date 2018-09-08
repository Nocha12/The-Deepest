#include "DXUT.h"
#include "Particle.h"
#include "GameScene.h"
#include "World.h"
#include "GameManager.h"

Particle::Particle(float angle, float speed, Entity *target) : angle(angle), target(target), timer(0), speed(speed)
{
	Sprite *sprite;
	addChild(sprite = new Sprite("image/mob/effect/die/1.png"));
	rect = sprite->rect;
	rotationCenter = center();
}

Particle::~Particle()
{
}

void Particle::update(float dt)
{
	Entity::update(dt);

	timer += dt;

	if (target && timer > 0.4f)
	{
		angle = calAngle(center(), target->center());
		speed = 1200;
	}

	if (sqrt(pow(center().x - target->center().x, 2) + pow(center().y - target->center().y, 2)) < 10)
	{
		gameManager.score += 5 + (int)(gameManager.combo / 10);
		removeChild(this);
		((GameScene*)world.currentScene)->particleList.remove(this);
	}

	pos += Vec2(cosf(angle), sinf(angle)) * speed * dt;
}