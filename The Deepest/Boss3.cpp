#include "DXUT.h"
#include "World.h"
#include "Boss3.h"
#include "GameScene.h"
#include "Bullet.h"
#include "GameManager.h"

Boss3::Boss3() : isAttacked3(false)
{
	hp = maxHp = 5000;
	type = BOSS;
	addChild(ani[Move] = new Animation("image/boss/2/middle/stop", 6, 10, true));
	rect = ani[Move]->rect;
	addChild(ani[LeftAttack] = new Animation("image/boss/2/middle/attack1/left", 9, 12, false));
	ani[LeftAttack]->stop = true;
	ani[LeftAttack]->visible = false;
	addChild(ani[RightAttack] = new Animation("image/boss/2/middle/attack1/right", 9, 12, false));
	ani[RightAttack]->stop = true;
	ani[RightAttack]->visible = false;
	addChild(ani[DropArmAttack] = new Animation("image/boss/2/middle/attack2", 12, 15, false));
	ani[DropArmAttack]->stop = true;
	ani[DropArmAttack]->visible = false;
	speed = 0;
	angle = 0;
	rect.top += 193;
	rect.bottom -= 314;
	rect.left += 256;
	rect.right -= 276;
}

Boss3::~Boss3()
{
}

void Boss3::update(float dt)
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
		scene->enemyManager->patternTimer = 50;
		scene->enemyManager->pattern = 9;
		scene->isBoss = false;
		scene->mapScrollSpeed = 300;
		scene->backScrollSpeed = 100;
		for_iter(arm, scene->bossArmList)
		{
			removeChild(*arm);
			arm = scene->bossArmList.erase(arm);
			if (arm == scene->bossArmList.end())
				break;
		}
		gameManager.bossCount = 1;
	}

	if (pos.y < -190)
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
			ani[LeftAttack]->stop = false;
			ani[LeftAttack]->visible = true;
			isAttacking1 = true;
		}
		else if (patternCount == 1)
		{
			patternCount++;
			isAttacking2 = true;
			ani[Move]->currentFrame = 0;
			ani[Move]->stop = true;
			ani[Move]->visible = false;
			ani[DropArmAttack]->stop = false;
			ani[DropArmAttack]->visible = true;
			scene->shake(0.05, 15, 20, true);
		}
		else if (patternCount == 2)
		{
			patternCount++;
			ani[Move]->currentFrame = 0;
			ani[Move]->stop = true;
			ani[Move]->visible = false;
			ani[RightAttack]->stop = false;
			ani[RightAttack]->visible = true;
			isAttacking1 = true;
		}
		else if (patternCount == 3)
		{
			patternCount = 0;
			isAttacking2 = true;
			ani[Move]->currentFrame = 0;
			ani[Move]->stop = true;
			ani[Move]->visible = false;
			ani[DropArmAttack]->stop = false;
			ani[DropArmAttack]->visible = true;
			scene->shake(0.05, 15, 20, true);
		}
	}

	if (((int)ani[LeftAttack]->currentFrame == 6) && !isAttacked1)
	{
		isAttacked1 = true;
		for (int i = 0; i < 20; ++i)
		{
			Bullet *b = new Bullet(S2_MidBullet, -300, D3DXToRadian(360) / 20 * i, 10, 0);
			((GameScene*)world.currentScene)->addChild(b);
			b->setCenter(Vec2(270, 430) + pos);
			((GameScene*)world.currentScene)->bulletList.push_back(b);
		}
	}
	else if (!((int)ani[LeftAttack]->currentFrame == 6))
		isAttacked1 = false;

	if (((int)ani[RightAttack]->currentFrame == 6) && !isAttacked2)
	{
		isAttacked2 = true;
		for (int i = 0; i < 20; ++i)
		{
			Bullet *b = new Bullet(S2_MidBullet, -300, D3DXToRadian(360) / 20 * i, 10, 0, 3);
			((GameScene*)world.currentScene)->addChild(b);
			b->setCenter(Vec2(590, 470) + pos);
			((GameScene*)world.currentScene)->bulletList.push_back(b);
		}
	}
	else if (!((int)ani[RightAttack]->currentFrame == 6))
		isAttacked2 = false;

	if (ani[LeftAttack]->currentFrame >= ani[LeftAttack]->textures.size() - 1)
	{
		ani[Move]->visible = true;
		ani[Move]->stop = false;
		ani[LeftAttack]->stop = true;
		ani[LeftAttack]->visible = false;
		ani[LeftAttack]->currentFrame = 0;
		isAttacking1 = false;
	}

	if (ani[RightAttack]->currentFrame >= ani[RightAttack]->textures.size() - 1)
	{
		ani[Move]->visible = true;
		ani[Move]->stop = false;
		ani[RightAttack]->stop = true;
		ani[RightAttack]->visible = false;
		ani[RightAttack]->currentFrame = 0;
		isAttacking1 = false;
	}

	if (ani[DropArmAttack]->currentFrame >= ani[DropArmAttack]->textures.size() - 1)
	{
		armDropTimer.reset(0.7f, 5);
		armDropTimer.onTick = [=](){
			Animation *arm;
			if (scene->player->center().x < center().x)
				arm = new Animation("image/boss/2/middle/rightArm", 8, 15, false);
			else
				arm = new Animation("image/boss/2/middle/leftArm", 8, 15, false);
			scene->bossArmList.push_back(arm);
			scene->addChild(arm);
			arm->setCenter(scene->player->center() - Vec2(0, 250));
		};
		armDropTimer.onFinished = [=](){
			isAttacking2 = false;

		};
		ani[Move]->visible = true;
		ani[Move]->stop = false;
		ani[DropArmAttack]->stop = true;
		ani[DropArmAttack]->visible = false;
		ani[DropArmAttack]->currentFrame = 0;
	}

	for_iter(arm, scene->bossArmList)
	{
		if ((int)(*arm)->currentFrame == 4 && !isAttacked3)
		{
			scene->shake(0.001, 15, 10, true);
			for (int i = 0; i < 6; ++i)
			{
				Bullet *b = new Bullet(S2_MidBullet, -300, D3DXToRadian(360) / 6 * i, 10, 0);
				((GameScene*)world.currentScene)->addChild(b);
				b->setCenter(Vec2(106, 570) + (*arm)->pos);
				((GameScene*)world.currentScene)->bulletList.push_back(b);
			}
			isAttacked3 = true;
		}
		else if (!((int)(*arm)->currentFrame == 4))
			isAttacked3 = false;

		if ((*arm)->currentFrame >= (*arm)->textures.size() - 1)
		{
			removeChild(*arm);
			arm = scene->bossArmList.erase(arm);
			if (arm == scene->bossArmList.end())
				break;
		}
	}
}