#include "DXUT.h"
#include "Boss2.h"
#include "World.h"
#include "GameScene.h"
#include "Bullet.h"

Boss2::Boss2() 
{
	hp = maxHp = 6000;
	type = BOSS;
	addChild(ani[Move] = new Animation("image/boss/1/final/stop", 6, 10, true));
	rect = ani[Move]->rect;
	addChild(ani[Attack] = new Animation("image/boss/1/final/attack", 10, 12, false));
	ani[Attack]->stop = true;
	ani[Attack]->visible = false;
	addChild(ani[BulletDropAttack] = new Animation("image/boss/1/final/attack2", 12, 15, false));
	ani[BulletDropAttack]->stop = true;
	ani[BulletDropAttack]->visible = false;
	speed = 0;
	angle = 0;
	rect.bottom -= 475;
	rect.left += 278;
	rect.right -= 312;
}

Boss2::~Boss2()
{
}

void Boss2::update(float dt)
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

	if (pos.y < -30)
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
			ani[Move]->currentFrame = 0;
			ani[Move]->stop = true;
			ani[Move]->visible = false;
			ani[BulletDropAttack]->stop = false;
			ani[BulletDropAttack]->visible = true;		
			scene->shake(0.05, 15, 20, true);
		}
	}

	if (((int)ani[Attack]->currentFrame == 6 || (int)ani[Attack]->currentFrame == 8) && !isAttacked1)
	{
		isAttacked1 = true;
		for (int i = 0; i < 16; ++i)
		{
			Bullet *b = new Bullet(S1_FinalBullet, 300, D3DXToRadian(360) / 16 * i, 0, 0);
			((GameScene*)world.currentScene)->addChild(b);
			b->setCenter(Vec2(402 + ((int)ani[Attack]->currentFrame - 6) * 212, 228) + pos);
			((GameScene*)world.currentScene)->bulletList.push_back(b);
		}
	}
	else if (!((int)ani[Attack]->currentFrame == 6 || (int)ani[Attack]->currentFrame == 8))
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
	
	if (ani[BulletDropAttack]->currentFrame >= ani[BulletDropAttack]->textures.size() - 1)
	{
		dropBulletTimer.reset(0.2f, 20);
		dropBulletTimer.onTick = [=](){
			Bullet *b = new Bullet(S1_FinalDropBullet, 0, D3DXToRadian(90), 15, 0);
			scene->addChild(b);
			b->setCenter(Vec2(random(0, 1280),-50));
			scene->bulletList.push_back(b);
		};
		dropBulletTimer.onFinished = [=](){
			isAttacking2 = false;

		};
		ani[Move]->visible = true;
		ani[Move]->stop = false;
		ani[BulletDropAttack]->stop = true;
		ani[BulletDropAttack]->visible = false;
		ani[BulletDropAttack]->currentFrame = 0;
	}
}