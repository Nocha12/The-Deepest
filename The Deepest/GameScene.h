#pragma once
#include "Scene.h"
#include "Player.h"
#include "Bullet.h"
#include "AfterImage.h"
#include "Enemy.h"
#include "EnemyManager.h"
#include "HitMap.h"
#include "Item.h"
#include "UI.h"
#include "Targetting.h"
#include "Particle.h"

class GameScene : public Scene
{
public:
	GameScene();
	~GameScene();
	Player *player;

	void update(float dt);
	void render();

	list<Bullet*> bulletList;
	list<AfterImage*> afterImageList;
	list<Enemy*> enemyList;
	list<Enemy*>::iterator enemyListIter;
	list<Sprite*> bubbleList;
	list<Animation*> bossArmList;
	list<Item*> itemList;
	list<Sprite*> seaWeedList;
	list<Animation*> effectList;
	list<Sprite*> moveUIList;
	list<Sprite*> faidInList;
	list<Sprite*> faidOutList;
	list<Sprite*> itemTextList;
	list<Particle*> particleList;
	list<Sprite*> dieEffectList;
	list<Targetting*> targettingList;

	float backScrollSpeed;
	float mapScrollSpeed;
	float gameClearTimer;

	UI *ui;
	Sprite *blue;

	bool isBoss;
	bool isClear;
	bool gameOver;
	bool warning;
	bool itsHighNoon;

	Sprite *map[2];
	Sprite *back[2];
	HitMap *hitMap[2];

	void ChangeCombo();

	Timer comboVisibleTimer;
	Timer comboScaleTimer;
	Timer itsHighNoonTimer;
	Timer itsHighNoonTimer2;

	EnemyManager *enemyManager;
};

