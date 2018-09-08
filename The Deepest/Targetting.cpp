#include "DXUT.h"
#include "Targetting.h"
#include "GameScene.h"
#include "World.h"

Targetting::Targetting(Entity *taraget) : removeAni(false)
{
	Animation *ani = new Animation("image/player/effect/base", 4, 50, false);
	addChild(ani);
	this->target = taraget;
	rect = ani->rect;
	cout << taraget->pos.x << ' ' << this->target->pos.x << endl;
}

Targetting::~Targetting()
{
}

void Targetting::update(float dt)
{
	Entity::update(dt);

	setCenter(target->center());

	if (removeAni)
	{
		removeChild(this);
		((GameScene*)world.currentScene)->targettingList.remove(this);
	}
}
