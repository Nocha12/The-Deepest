#include "DXUT.h"
#include "Bullet.h"
#include "World.h"
#include "GameScene.h"

Bullet::Bullet(BulletType type, float speed, float angle, float speedRate, float angleRate, float damage, Entity *target) : type(type), speed(speed), angle(angle), speedRate(speedRate), angleRate(angleRate), target(target), isPlayerBullet(false), damage(damage), homingTimer(0)
{
	Sprite *t;

	if (type == PlayerBullet)
		addChild(t = new Sprite("image/player/bullet/0.png"));
	if (type == PlayerHoming)
	{
		addChild(t = new Sprite("image/player/bullet/homing.png"));
		afterImageTimer.reset(0.05, 100);
		afterImageTimer.onTick = [=](){
			AfterImage *img = new AfterImage(new Sprite("image/player/bullet/homing.png"));
			img->rotation = rotation;
			world.currentScene->addChild(img);
			img->setCenter(center());
			((GameScene*)world.currentScene)->afterImageList.push_back(img);
		};
	}
	if (type == PlayerWaterZet)
		addChild(t = new Sprite("image/player/bullet/torpedo.png"));
	if (type == Nuclear)
	{
		Animation *ani = new Animation("image/player/skill", 9, 12, true);
		addChild(ani);
		Animation *ani2 = new Animation("image/player/effect/nuclear/go", 5, 12, true);
		addChild(ani2);
		rect = ani->rect;
		ani2->setCenter(Vec2(rect.center().x, 50));
	}
	if (type == S1_OctoBullet)
		addChild(t = new Sprite("image/mob/bullet/1/octopus.png"));
	if (type == S2_OctoBullet)
		addChild(t = new Sprite("image/mob/bullet/2/octopus.png"));
	if (type == S1_FishBullet)
		addChild(t = new Sprite("image/mob/bullet/1/fish.png"));
	if (type == S2_FishBullet)
		addChild(t = new Sprite("image/mob/bullet/2/fish.png"));
	if (type == S1_MidBullet)
		addChild(t = new Sprite("image/boss/bullet/1/middle.png"));
	if (type == S1_FinalBullet)
		addChild(t = new Sprite("image/boss/bullet/1/final1.png"));
	if (type == S1_FinalDropBullet)
	{
		addChild(t = new Sprite("image/boss/bullet/1/final2.png"));
		afterImageTimer.reset(0.1f, 100);
		afterImageTimer.onTick = [=](){
			AfterImage *img = new AfterImage(new Sprite("image/boss/bullet/1/final2.png"));
			img->rotation = rotation;
			world.currentScene->addChild(img);
			img->setCenter(center());
			((GameScene*)world.currentScene)->afterImageList.push_back(img);
		};
	}
	if (type == S2_MidBullet)
		addChild(t = new Sprite("image/boss/bullet/2/middle.png"));
	if (type == S2_FinalStorm)
	{
		addChild(t = new Sprite("image/boss/bullet/2/final2.png"));
		afterImageTimer.reset(0.05, 1000);
		afterImageTimer.onTick = [=](){
			AfterImage *img = new AfterImage(new Sprite("image/boss/bullet/2/final2.png"));
			img->rotation = rotation;
			world.currentScene->addChild(img);
			img->setCenter(center());
			((GameScene*)world.currentScene)->afterImageList.push_back(img);
		};
	}
	if (type == S2_FinalSword)
	{
		addChild(t = new Sprite("image/boss/bullet/2/final1.png"));
		afterImageTimer.reset(0.05, 100);
		afterImageTimer.onTick = [=](){
			AfterImage *img = new AfterImage(new Sprite("image/boss/bullet/2/final1.png"));
			img->rotation = rotation;
			world.currentScene->addChild(img);
			img->setCenter(center());
			((GameScene*)world.currentScene)->afterImageList.push_back(img);
		};
	}

	if (type != Nuclear)
		rect = t->rect;

	rotationCenter = rect.center();
}

Bullet::~Bullet()
{
}

void Bullet::update(float dt)
{
	Entity::update(dt);
	
	GameScene *scene = (GameScene*)world.currentScene;

	homingTimer += dt;
	speed += speedRate;
	angle += angleRate;

	if (type == S2_FinalStorm)
		rotation += 10 * dt;
	else
		rotation = angle + D3DXToRadian(90);

	if (target)
		angle = calAngle(center(), target->center());

	pos += Vec2(cosf(angle), sinf(angle)) * speed * dt;

	if (homingTimer > 5)
		target = 0;

	bool dump = false;
	bool eff = false;

	if (scene->map[0]->pos.y < center().y)
	{
		if (scene->hitMap[0]->map[(int)(center().x + 147)][(int)(center().y - scene->map[0]->pos.y)])
		{
			scene->shake(0.01, 5, 3, false);
			
			eff = true;

			if (type == Nuclear)
			{
				scene->shake(0.01, 30, 30, true);
				for_iter(iter, scene->enemyList)
					(*iter)->hp -= 500;
				for_iter(iter, scene->bulletList)
				{
					if (!(*iter)->isPlayerBullet)
					{
						removeChild(*iter);
						iter = scene->bulletList.erase(iter);
						if (iter == scene->bulletList.end())
							break;
					}
				}
			}
			dump = true;
		}
	}
	else if (scene->hitMap[1]->map[(int)(center().x + 147)][(int)(center().y - scene->map[1]->pos.y)])
	{
		scene->shake(0.01, 5, 3, false);

		eff = true;
		dump = true;

		if (type == Nuclear)
		{
			scene->shake(0.01, 30, 30, true);
			for_iter(iter, scene->enemyList)
				(*iter)->hp -= 500;
			for_iter(iter, scene->bulletList)
			{
				if (!(*iter)->isPlayerBullet)
				{
					removeChild(*iter);
					iter = scene->bulletList.erase(iter);
					if (iter == scene->bulletList.end())
						break;
				}
			}
		}
	}

	if (dump || center().x < 0 || center().x > 1280 || center().y < -200 || center().y > 750)
	{
		if (eff)
		{
			Animation *eff;

			if (type == PlayerBullet)
				eff = new Animation("image/player/effect/base", 5, 15, false);
			if (type == PlayerHoming)
				eff = new Animation("image/player/effect/homing", 5, 15, false);
			if (type == PlayerWaterZet)
				eff = new Animation("image/player/effect/torpedo", 5, 15, false);
			if (type == Nuclear)
				eff = new Animation("image/player/effect/nuclear/peobeong", 24, 15, false);
			if (type == S1_OctoBullet || type == S1_FishBullet)
				eff = new Animation("image/mob/effect/1", 4, 15, false);
			else if (type == S1_MidBullet || type == S1_FinalDropBullet || type == S1_FinalBullet)
				eff = new Animation("image/boss/effect/1", 4, 15, false);
			else if (type == S2_OctoBullet || type == S2_FishBullet)
				eff = new Animation("image/mob/effect/2", 4, 15, false);
			else if (type == S2_MidBullet)
				eff = new Animation("image/boss/effect/2", 4, 15, false);
			if (!(type == S2_FinalStorm || type == S2_FinalSword))
			{
				scene->addChild(eff);
				scene->effectList.push_back(eff);
				eff->setCenter(center());
			}
		}

		removeChild(this);
		scene->bulletList.remove(this);
	}
}
