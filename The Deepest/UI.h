#pragma once
#include "Entity.h"
#include "Animation.h"

class UI : public Entity
{
public:
	UI();
	~UI();

	Sprite *black;
	Sprite *hpBar;
	Sprite *bossHpBar;
	Sprite *bossHpBack;	
	Sprite *hit;
	Sprite *highNoon;
	Sprite *stageClear;
	Sprite *stageStart;
	Sprite *scoreText[6][10];
	Sprite *bombText[2][10];
	Sprite *comboText[3][10];
	Sprite *continueScene;
	Sprite *yes;
	Sprite *no;
	Sprite *gameOver;
	Animation *nextScene;
	Sprite *back;
	Sprite *warning;
	Sprite *warningBar[2];
	Sprite *combo;
	Animation *ed;
};

