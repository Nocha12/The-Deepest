#include "DXUT.h"
#include "GameManager.h"
#include "GameScene.h"
#include "World.h"
#include "EnemyManager.h"
#include "Enemy.h"
#include "World.h"
#include "Boss1.h"
#include "Boss2.h"
#include "Boss3.h"
#include "Boss4.h"

EnemyManager::EnemyManager() : pattern(-1), patternTimer(0)
{
}

EnemyManager::~EnemyManager()
{
}

void EnemyManager::update(float dt)
{
	Entity::update(dt);

	GameScene *scene = (GameScene*)world.currentScene;

	if (world.getKeyState('B') == 1 && !scene->isBoss)
	{
		if (pattern > 6)
		{
			patternTimer = 82;
			pattern = 14;
		}
		else{
			patternTimer = 40;
			pattern = 6;
		}	
	}
	if (world.getKeyState('N') == 1 && !scene->isBoss)
	{
		patternTimer = 82;
		pattern = 14;
	}

	patternTimer += dt;

	if (patternTimer >= 0 && pattern == -1)
	{
		++pattern;
	}
	else if (patternTimer >= 3 && pattern == 0)
	{
		++pattern;
		Octo *octo = new Octo(D3DXToRadian(180), 150);
		octo->setCenter(Vec2(1280, 720 / 3));
		scene->addChild(octo);
		scene->enemyList.push_back(octo);
		Octo *octo2 = new Octo(D3DXToRadian(0), 150);
		octo2->setCenter(Vec2(-100, 720 / 3));
		scene->addChild(octo2);
		scene->enemyList.push_back(octo2);

		Sprite *seaWeed = new Sprite("image/map/seaweed.png");
		seaWeed->setCenter(Vec2(1280 / 2, -200));
		scene->addChild(seaWeed);
		scene->seaWeedList.push_back(seaWeed);
	}
	else if (patternTimer >= 8 && pattern == 1)
	{
		++pattern;
		groupTimer1.reset(1, 5);
		groupTimer1.onTick = [=](){
			Fish *fish = new Fish(D3DXToRadian(-10), 150);
			fish->setCenter(Vec2(-100, 720 / 2));
			scene->addChild(fish);
			scene->enemyList.push_back(fish);
		};

		Octo *octo = new Octo(D3DXToRadian(180), 200);
		octo->setCenter(Vec2(1280, 720 / 2 - 200));
		scene->addChild(octo);
		scene->enemyList.push_back(octo);
	}
	else if (patternTimer >= 16 && pattern == 2)
	{
		++pattern;
		groupTimer1.reset(1, 5);
		groupTimer1.onTick = [=](){
			Fish *fish = new Fish(D3DXToRadian(-10), 150);
			fish->setCenter(Vec2(-100, 720 / 2));
			scene->addChild(fish);
			scene->enemyList.push_back(fish);
		};

		Octo *octo = new Octo(D3DXToRadian(180), 200);
		octo->setCenter(Vec2(1280, 720 / 2 - 100));
		scene->addChild(octo);
		scene->enemyList.push_back(octo);

		Sprite *seaWeed = new Sprite("image/map/seaweed.png");
		seaWeed->setCenter(Vec2(1280 / 3, -200));
		scene->addChild(seaWeed);
		scene->seaWeedList.push_back(seaWeed);
	}
	else if (patternTimer >= 24 && pattern == 3)
	{
		++pattern;
		groupTimer2.reset(1, 5);
		groupTimer2.onTick = [=](){
			Fish *fish = new Fish(D3DXToRadian(170), 150);
			fish->setCenter(Vec2(1280, 720 / 2));
			scene->addChild(fish);
			scene->enemyList.push_back(fish);
		};

		Rush *rush = new Rush();
		rush->setCenter(Vec2(1280 / 3, -50));
		scene->addChild(rush);
		scene->enemyList.push_back(rush);
	}
	else if (patternTimer >= 30 && pattern == 4)
	{
		++pattern;
		for (int i = 0; i < 4; i++)
		{
			Octo *octo = new Octo(D3DXToRadian(180 - (i % 2 * 180)), 200);
			octo->setCenter(Vec2(1280 - (1380 * (i % 2)), 400 - 100 * i));
			scene->addChild(octo);
			scene->enemyList.push_back(octo);
		}

		Sprite *seaWeed = new Sprite("image/map/seaweed.png");
		seaWeed->setCenter(Vec2(1280 / 2 + 1280 / 3, -200));
		scene->addChild(seaWeed);
		scene->seaWeedList.push_back(seaWeed);
	}
	else if (patternTimer >= 35 && pattern == 5)
	{
		++pattern;

		groupTimer2.reset(1, 5);
		groupTimer2.onTick = [=](){
			Rush *rush = new Rush();
			rush->setCenter(Vec2(1280 / 2 - 200 + 100 * groupTimer2.count.first, -50));
			scene->addChild(rush);
			scene->enemyList.push_back(rush);
		};
	}
	else if (patternTimer >= 40 && pattern == 6)
	{
		++pattern; 
		scene->warning = true;
		scene->mapScrollSpeed = 600;
		scene->backScrollSpeed = 200;
		scene->isBoss = true;
	}
	else if (patternTimer >= 46 && pattern == 7)
	{	
		++pattern; //보스
		scene->warning = false;
		scene->mapScrollSpeed = 0;
		scene->backScrollSpeed = 0;

		scene->faidInList.push_back(scene->ui->bossHpBack);
		scene->faidInList.push_back(scene->ui->bossHpBar);

		if (gameManager.stage == 1)
		{
			Boss1 *boss1 = new Boss1();
			boss1->pos = (Vec2(1280 / 2, -600));
			scene->addChild(boss1);
			scene->enemyList.push_back(boss1);
		}
		else
		{
			Boss3 *boss3 = new Boss3();
			boss3->pos = Vec2(180, -510);
			scene->addChild(boss3);
			scene->enemyList.push_back(boss3);
		}
	}
	else if (patternTimer >= 50 && pattern == 9)
	{//보스 클리어 후 진행
		++pattern;
		groupTimer2.reset(1, 4);
		groupTimer2.onTick = [=](){
			Octo *octo = new Octo(D3DXToRadian(180 - (groupTimer2.count.first % 2 * 180)), 200);
			octo->setCenter(Vec2(1280 - (1380 * (groupTimer2.count.first % 2)), 400 - 100 * groupTimer2.count.first));
			scene->addChild(octo);
			scene->enemyList.push_back(octo);
		};
		Sprite *seaWeed = new Sprite("image/map/seaweed.png");
		seaWeed->setCenter(Vec2(1280 / 2, -200));
		scene->addChild(seaWeed);
		scene->seaWeedList.push_back(seaWeed);
	}
	else if (patternTimer >= 57 && pattern == 10)
	{
		++pattern;

		groupTimer1.reset(1, 5);
		groupTimer1.onTick = [=](){
			Fish *fish = new Fish(D3DXToRadian(-15), 150);
			fish->setCenter(Vec2(-100, 720 / 2));
			scene->addChild(fish);
			scene->enemyList.push_back(fish);
		};

		groupTimer2.reset(1, 5);
		groupTimer2.onTick = [=](){
			Fish *fish = new Fish(D3DXToRadian(165), 150);
			fish->setCenter(Vec2(1280, 720 / 2));
			scene->addChild(fish);
			scene->enemyList.push_back(fish);
		};

		Sprite *seaWeed = new Sprite("image/map/seaweed.png");
		seaWeed->setCenter(Vec2(1280 / 3, -300));
		scene->addChild(seaWeed);
		scene->seaWeedList.push_back(seaWeed);
	}
	else if (patternTimer >= 67 && pattern == 11)
	{
		++pattern;
		for (int i = 0; i < 3; ++i)
		{
			Rush *rush = new Rush();
			rush->setCenter(Vec2(1280 / 2 - 200 + 200 * i, -50));
			scene->addChild(rush);
			scene->enemyList.push_back(rush);
		}
	}
	else if (patternTimer >= 73 && pattern == 12)
	{
		++pattern;
		groupTimer2.reset(1, 4);
		groupTimer2.onTick = [=](){
			Octo *octo = new Octo(D3DXToRadian(180 - (groupTimer2.count.first % 2 * 180)), 200);
			octo->setCenter(Vec2(1280 - (1380 * (groupTimer2.count.first % 2)), 400 - 100 * groupTimer2.count.first));
			scene->addChild(octo);
			scene->enemyList.push_back(octo);
		};
		Sprite *seaWeed = new Sprite("image/map/seaweed.png");
		seaWeed->setCenter(Vec2(1280 / 2, -300));
		scene->addChild(seaWeed);
		scene->seaWeedList.push_back(seaWeed);
	}
	else if (patternTimer >= 78 && pattern == 13)
	{
		++pattern;
		for (int i = 0; i < 5; ++i)
		{
			Rush *rush = new Rush();
			rush->setCenter(Vec2(1280 / 2 - 400 + 200 * i, -50));
			scene->addChild(rush);
			scene->enemyList.push_back(rush);
		}
	}
	else if (patternTimer >= 82 && pattern == 14)
	{
		++pattern;
		scene->mapScrollSpeed = 600;
		scene->backScrollSpeed = 200;
		scene->isBoss = true;
		scene->warning = true;
		//워링;
	}
	else if (patternTimer >= 88 && pattern == 15)
	{
		scene->warning = false;
		++pattern;
		//보스
		scene->mapScrollSpeed = 0;
		scene->backScrollSpeed = 0;
		
		scene->faidInList.push_back(scene->ui->bossHpBack);
		scene->faidInList.push_back(scene->ui->bossHpBar);

		if (gameManager.stage == 1)
		{
			Boss2 *boss2 = new Boss2();
			boss2->pos = Vec2(0, -400);
			scene->addChild(boss2);
			scene->enemyList.push_back(boss2);
		}
		else
		{
			Boss4 *boss4 = new Boss4();
			boss4->pos = Vec2(-215, -630);
			scene->addChild(boss4);
			scene->enemyList.push_back(boss4);
		}
	}
}