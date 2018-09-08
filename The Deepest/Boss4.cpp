#include "DXUT.h"
#include "Boss4.h"
#include "World.h"
#include "GameScene.h"
#include "Bullet.h"

Boss4::Boss4()
{
	hp = maxHp = 7000;
	type = BOSS;
	addChild(ani[Move] = new Animation("image/boss/2/final/stop", 6, 10, true));
	rect = ani[Move]->rect;
	addChild(ani[StormAttack] = new Animation("image/boss/2/final/attack2", 8, 10, false));
	ani[StormAttack]->stop = true;
	ani[StormAttack]->visible = false;
	addChild(ani[SwardAttack] = new Animation("image/boss/2/final/attack1", 15, 15, false));
	ani[SwardAttack]->stop = true;
	ani[SwardAttack]->visible = false;
	speed = 0;
	angle = 0;

	rect.top += 332;
	rect.bottom -= 627;
	rect.left += 621;
	rect.right -= 605;
}

Boss4::~Boss4()
{
}

void Boss4::update(float dt)
{
	Enemy::update(dt);

	GameScene *scene = (GameScene*)world.currentScene;

	scene->ui->bossHpBar->visibleRect.right = scene->ui->bossHpBar->rect.right * hp / maxHp;

	if (world.getKeyState('K') == 1)
		hp = 0;

	if (hp <= 0 && !scene->isClear)
		scene->moveUIList.push_back(scene->ui->stageClear);

	if (hp <= 0)
	{
		scene->faidOutList.push_back(scene->ui->bossHpBar);
		scene->faidOutList.push_back(scene->ui->bossHpBack);
		scene->isClear = true;
	}

	if (pos.y < -330)
	{
		pos.y += 100 * dt;
	}
	else if (!isAttacking1 && !isAttacking2)
		tum += dt;

	if (tum >= 4)
	{
		tum = 0;
		if (patternCount == 0)
		{
			patternCount++;
			ani[Move]->currentFrame = 0;
			ani[Move]->stop = true;
			ani[Move]->visible = false;
			ani[StormAttack]->stop = false;
			ani[StormAttack]->visible = true;
			isAttacking1 = true;
		}
		else if (patternCount == 1)
		{
			patternCount = 0;
			isAttacking2 = true;
			ani[Move]->currentFrame = 0;
			ani[Move]->stop = true;
			ani[Move]->visible = false;
			ani[SwardAttack]->stop = false;
			ani[SwardAttack]->visible = true;
		}
	}

	if (((int)ani[StormAttack]->currentFrame == 4) && !isAttacked1)
	{
		isAttacked1 = true;
		
		Bullet *b = new Bullet(S2_FinalStorm, 70, D3DXToRadian(360), 0.1f, 0, 0.5f, scene->player);
		((GameScene*)world.currentScene)->addChild(b);
		b->setCenter(Vec2(853, 661) + pos);
		((GameScene*)world.currentScene)->bulletList.push_back(b);
	}
	else if (!((int)ani[StormAttack]->currentFrame == 4))
		isAttacked1 = false;

	if (((int)ani[SwardAttack]->currentFrame == 4 || (int)ani[SwardAttack]->currentFrame == 11) && !isAttacked2)
	{
		isAttacked2 = true;
		
		Bullet *b = new Bullet(S2_FinalSword, 300, calAngle(center(), ((GameScene*)world.currentScene)->player->center()), 10, 0, 0.4f);
		((GameScene*)world.currentScene)->addChild(b);
		b->setCenter(Vec2(842, 700) + pos);
		((GameScene*)world.currentScene)->bulletList.push_back(b);
	}
	else if (!((int)ani[SwardAttack]->currentFrame == 4 || (int)ani[SwardAttack]->currentFrame == 11))
		isAttacked2 = false;

	if (ani[StormAttack]->currentFrame >= ani[StormAttack]->textures.size() - 1)
	{
		ani[Move]->visible = true;
		ani[Move]->stop = false;
		ani[StormAttack]->stop = true;
		ani[StormAttack]->visible = false;
		ani[StormAttack]->currentFrame = 0;
		isAttacking1 = false;
	}

	if (ani[SwardAttack]->currentFrame >= ani[SwardAttack]->textures.size() - 1)
	{
		ani[Move]->visible = true;
		ani[Move]->stop = false;
		ani[SwardAttack]->stop = true;
		ani[SwardAttack]->visible = false;
		ani[SwardAttack]->currentFrame = 0;
		isAttacking2 = false;
	}
}