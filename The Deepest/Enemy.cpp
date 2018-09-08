#include "DXUT.h"
#include "Enemy.h"
#include "GameManager.h"
#include "World.h"
#include "GameScene.h"
#include "Bullet.h"
#include "Item.h"
#include "Particle.h"

Enemy::Enemy() : isAttacked1(false), isAttacked2(false), isAttacking1(false), isAttacking2(false), tum(0), patternCount(0), type(NORMAL)
{
	speed = 0;
	angle = 0;
}

Enemy::~Enemy()
{
}

void Enemy::GetRed()
{
	for_iter(iter, ani)
	{
		(*iter).second->color.b = 0.3f;
		(*iter).second->color.g = 0.3f;
	}
	redTimer.reset(0.01f, 25);
	redTimer.onTick = [=](){
		for_iter(iter, ani)
		{
			(*iter).second->color.b += 0.028f;
			(*iter).second->color.g += 0.028f;
		}
	};
}

void Enemy::update(float dt)
{
	GameScene *scene = (GameScene*)world.currentScene;

	Unit::update(dt);

	if (type == NORMAL && (center().x < -200 || center().x > 1280 + 200 || center().y < -200 || center().y > 720 + 200))
	{
		removeChild(this);
		scene->enemyList.remove(this);
	}
	
	if (hp <= 0)
	{
		Sprite *dieEff = new Sprite("image/mob/effect/die/0.png");
		dieEff->scale = Vec2(0.5f, 0.5f);
		dieEff->scaleCenter = dieEff->rect.center();
		dieEff->rotationCenter = dieEff->rect.center();
		scene->addChild(dieEff);
		scene->dieEffectList.push_back(dieEff);
		dieEff->setCenter(center());

		for (int i = 0; i < 20; ++i)
		{
			Particle *p = new Particle(D3DXToRadian(random(0, 360)), random(200, 400), scene->player);
			scene->addChild(p);
			scene->particleList.push_back(p);
			p->setCenter(center());
		}

		if (type == BOSS)
		{
			for (int i = 0; i < 40; ++i)
			{
				Particle *p = new Particle(D3DXToRadian(random(0, 360)), random(400, 800), scene->player);
				scene->addChild(p);
				scene->particleList.push_back(p);
				p->setCenter(center());
			}

			for (int i = 0; i < 5; ++i)
			{
				Item *item = new Item((ItemType)random(0, 7));
				scene->addChild(item);
				scene->itemList.push_back(item);
				item->setCenter(Vec2(center().x - 200 + 100 * i, center().y));
			}
			Animation *eff = new Animation("image/boss/effect/die", 33, 12, false);
			scene->addChild(eff);
			scene->effectList.push_back(eff);
			eff->setCenter(center() + Vec2(0, 100));
		}
		else if (random(0, 2) == 0)
		{
			Item *item = new Item((ItemType)random(0, 7));
			scene->addChild(item);
			scene->itemList.push_back(item);
			item->setCenter(center());
		}

		for_iter(iter, scene->bulletList)
		{
			if ((*iter)->target == this)
				(*iter)->target = nullptr;
		}

		removeChild(this);
		scene->enemyList.remove(this);
	}

	pos += Vec2(cosf(angle), sinf(angle)) * speed * dt;
}

Octo::Octo(float angle, float speed)
{
	hp = maxHp = 100;
	this->angle = angle;
	this->speed = speed;
	addChild(ani[Move] = new Animation("image/mob/" + to_string(gameManager.stage) + "/octopus/move", 8, 10, true));
	rect = ani[Move]->rect;
	rect.curOff(25);
	addChild(ani[Attack] = new Animation("image/mob/" + to_string(gameManager.stage) + "/octopus/attack", 8, 10, true));
	ani[Attack]->stop = true;
	ani[Attack]->visible = false;
	attackReadyTimer.reset(3, 1);
	attackReadyTimer.onFinished = [=](){
		ani[Attack]->stop = false;
		ani[Attack]->visible = true; 
		ani[Move]->visible = false;
		this->speed /= 5;
		attackTimer.reset(10, 1);
		attackTimer.onFinished = [=](){
			ani[Move]->visible = true;
			ani[Attack]->stop = true;
			ani[Attack]->visible = false;
			ani[Attack]->currentFrame = 0;
			this->speed *= 5;
		};
	};
}

Octo::~Octo()
{
}

void Octo::update(float dt)
{
	Enemy::update(dt);
	
	if ((int)ani[Attack]->currentFrame == 5 && !isAttacked1)
	{
		isAttacked1 = true;
		Bullet *b;
		if (gameManager.stage == 1)
			b = new Bullet(S1_OctoBullet, 300, calAngle(center(), ((GameScene*)world.currentScene)->player->center()), 0, 0);
		else 
			b = new Bullet(S2_OctoBullet, 300, calAngle(center(), ((GameScene*)world.currentScene)->player->center()), 0, 0);
		((GameScene*)world.currentScene)->addChild(b);
		b->setCenter(Vec2(73, 110) + pos);
		((GameScene*)world.currentScene)->bulletList.push_back(b);
	}
	else if (!((int)ani[Attack]->currentFrame == 5))
		isAttacked1 = false;
}


Rush::Rush()
{
	hp = maxHp = 70;
	angle = D3DXToRadian(90);
	speed = 150;
	addChild(ani[Move] = new Animation("image/mob/" + to_string(gameManager.stage) + "/rush/move", 6, 10, true));
	rect = ani[Move]->rect;
	rect.curOff(25);
	addChild(ani[Attack] = new Animation("image/mob/" + to_string(gameManager.stage) + "/rush/attack", 6, 10, true));
	ani[Attack]->stop = true;
	ani[Attack]->visible = false;
	addChild(ani[AttackReady] = new Animation("image/mob/" + to_string(gameManager.stage) + "/rush/ready", 5, 10, false));
	ani[AttackReady]->stop = true;
	ani[AttackReady]->visible = false;
}

Rush::~Rush()
{
}

void Rush::update(float dt)
{
	Enemy::update(dt);

	if (center().y >= 100 && ani[Move]->visible)
	{
		ani[Move]->visible = false;
		ani[AttackReady]->stop = false;
		ani[AttackReady]->visible = true;
		speed = 0;
	}

	if (ani[AttackReady]->currentFrame >= ani[AttackReady]->textures.size() - 1)
	{
		ani[AttackReady]->visible = false;
		ani[AttackReady]->stop = true;
		ani[AttackReady]->currentFrame = 0;
		ani[Attack]->stop = false;
		ani[Attack]->visible = true;
		speed = 500;
		angle = calAngle(center(), ((GameScene*)world.currentScene)->player->center());
		
		afterImageTimer.reset(0.05, 100);
		afterImageTimer.onTick = [=](){
			Sprite *t = new Sprite();
			t->setTexture(ani[Attack]->texture);
			AfterImage *img = new AfterImage(t);
			img->rotation = rotation;
			world.currentScene->addChild(img);
			img->setCenter(center());
			((GameScene*)world.currentScene)->afterImageList.push_back(img);
		};
	}
}

Fish::Fish(float angle, float speed)
{
	hp = maxHp = 5;
	this->angle = angle;
	rotation = angle - D3DXToRadian(90);
	this->speed = speed;
	addChild(ani[Move] = new Animation("image/mob/" + to_string(gameManager.stage) + "/fishGroup", 12, 15, true));
	rect = ani[Move]->rect;
	rotationCenter = rect.center();
	rect.curOff(28);
	attackTimer.reset(3, 1);
	attackTimer.onFinished = [=](){
		Bullet *b;
		if (gameManager.stage == 1)
			b = new Bullet(S1_FishBullet, 300, calAngle(center(), ((GameScene*)world.currentScene)->player->center()), 0, 0);
		else
			b = new Bullet(S1_FishBullet, 300, calAngle(center(), ((GameScene*)world.currentScene)->player->center()), 0, 0);
		((GameScene*)world.currentScene)->addChild(b);
		b->setCenter(center());
		((GameScene*)world.currentScene)->bulletList.push_back(b);
	};
}

Fish::~Fish()
{
}