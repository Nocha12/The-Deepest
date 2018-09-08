#include "DXUT.h"
#include "Boss1.h"
#include "World.h"
#include "GameScene.h"
#include "GameManager.h"

Boss1::Boss1() : rushCount(0)
{
	hp = maxHp = 4000;
	type = BOSS;
	addChild(ani[Move] = new Animation("image/boss/1/middle/move", 12, 15, true));
	rect = ani[Move]->rect;
	addChild(ani[Attack] = new Animation("image/boss/1/middle/attack", 12, 15, false));
	ani[Attack]->stop = true;
	ani[Attack]->visible = false;
	addChild(ani[SideAttack] = new Animation("image/boss/1/middle/attackSide", 12, 15, true));
	ani[SideAttack]->stop = true;
	ani[SideAttack]->visible = false;
	speed = 0;
	angle = 0;
	rect.top += 32;
	rect.bottom -= 20;
	rect.left += 70;
	rect.right -= 70;
}

Boss1::~Boss1()
{
}

void Boss1::update(float dt)
{
	Enemy::update(dt);
	
	GameScene *scene = (GameScene*)world.currentScene;

	scene->ui->bossHpBar->visibleRect.right = scene->ui->bossHpBar->rect.right * hp / maxHp;

	if (world.getKeyState('K') == 1)
		hp = 0;

	if (hp <= 0)
	{
		scene->faidOutList.push_back(scene->ui->bossHpBar);
		scene->faidOutList.push_back(scene->ui->bossHpBack);
		scene->isBoss = false;
		scene->mapScrollSpeed = 300;
		scene->backScrollSpeed = 100;
		scene->enemyManager->patternTimer = 50;
		scene->enemyManager->pattern = 9;
		gameManager.bossCount = 1;
	}

	if (rushCount == 0)
		pos.x = scene->player->center().x - 120;

	if (center().y < 0)
	{
		pos.y += 100 * dt;
	}
	else if (!isAttacking1 && !isAttacking2)
		tum += dt;

	if (tum >= 3)
	{
		tum = 0;
		if (patternCount == 0)
		{
			patternCount++;
			ani[Move]->currentFrame = 0;
			ani[Move]->stop = true;
			ani[Move]->visible = false;
			ani[Attack]->stop = false;
			ani[Attack]->visible = true;
			isAttacking1 = true;
		}
		else if (patternCount == 1)
		{
			patternCount = 0;
			isAttacking2 = true;
			rushCount = 1;
		}
	}

	if (rushCount == 1)
	{
		pos.y -= 300 * dt;
		if (center().y <= -600)
		{
			rushCount = 2;
			ani[Move]->currentFrame = 0;
			ani[Move]->stop = true;
			ani[Move]->visible = false;
			ani[SideAttack]->stop = false;
			ani[SideAttack]->visible = true;
			rect = ani[SideAttack]->rect;
			scaleCenter = rect.center();
			rect.top += 115;
			rect.bottom -= 115;
			rect.left += 75;
			rect.right -= 75;
			pos = Vec2(-612 -147, 0);
		}
	}
	else if (rushCount == 2)
	{
		pos.x += 400 * dt;
		if (pos.x >= 1727)
		{
			rushCount = 3;
			scale.x = -1;
			pos = Vec2(1727, 245);
		}
	}
	else if (rushCount == 3)
	{
		pos.x -= 400 * dt;
		if (pos.x <= -612 - 147)
		{
			rushCount = 0;

			ani[SideAttack]->currentFrame = 0;
			ani[SideAttack]->stop = true;
			ani[SideAttack]->visible = false;
			ani[Move]->stop = false;
			ani[Move]->visible = true;
			rect = ani[Move]->rect;
			scaleCenter = rect.center();
			rect.top += 32;
			rect.bottom -= 20;
			rect.left += 70;
			rect.right -= 70;
			pos = Vec2(1280 / 2, -600);
			isAttacking2 = false;
			scale.x = 1;
		}
	}

	if (((int)ani[Attack]->currentFrame == 5 || (int)ani[Attack]->currentFrame == 10) && !isAttacked1)
	{
		isAttacked1 = true;
		for (int i = 0; i < 16; ++i)
		{
			Bullet *b = new Bullet(S1_MidBullet, 300, D3DXToRadian(360) / 16 * i, 0, 0);
			((GameScene*)world.currentScene)->addChild(b);
			b->setCenter(Vec2(117, 580) + pos);
			((GameScene*)world.currentScene)->bulletList.push_back(b);
		}
	}
	else if (!((int)ani[Attack]->currentFrame == 5 || (int)ani[Attack]->currentFrame == 10))
		isAttacked1 = false;

	if (ani[Attack]->currentFrame >= ani[Attack]->textures.size() - 1)
	{
		ani[Move]->visible = true;
		ani[Move]->stop = false;
		ani[Attack]->stop = true;
		ani[Attack]->visible = false;
		ani[Attack]->currentFrame = 0;
		isAttacking1 = false;
	}
}