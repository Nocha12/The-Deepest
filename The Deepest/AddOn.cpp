#include "DXUT.h"
#include "AddOn.h"
#include "World.h"
#include "GameScene.h"
#include "Bullet.h"

AddOn::AddOn() : angle(0), canShoot(false)
{
	addChild(ani = new Animation("image/player/addOn", 10, 10, true));
	shootDelay = pair<float, float>(0, 1);
	rect = ani->rect;
}

AddOn::~AddOn()
{
}

void AddOn::update(float dt)
{
	Entity::update(dt);

	setCenter(Vec2(cosf(angle), sinf(angle)) * 100 + parent->rect.center() + Vec2(38, 32));
	angle += dt;

	shootDelay.first += dt;

	if (shootDelay.first > shootDelay.second && canShoot)
	{
		shootDelay.first = 0;
		Bullet *b = new Bullet(PlayerBullet, 300, D3DXToRadian(-90), 0, 0);
		((GameScene*)world.currentScene)->addChild(b);
		b->setCenter(Vec2(20, 8) + pos + parent->pos);
		((GameScene*)world.currentScene)->bulletList.push_back(b);
		b->isPlayerBullet = true;
	}
}
