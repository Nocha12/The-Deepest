#include "DXUT.h"
#include "UI.h"
#include "GameManager.h"

UI::UI()
{
	addChild(highNoon = new Sprite("image/UI/ingame/hit.png"));
	highNoon->color.a = 0;
	addChild(hit = new Sprite("image/UI/ingame/hit.png"));
	hit->color.a = 0;
	addChild(new Sprite("image/UI/ingame/ingame.png"));
	addChild(hpBar = new Sprite("image/UI/ingame/hpbar.png"));
	hpBar->pos = Vec2(102, 45); 

	addChild(bossHpBack = new Sprite("image/UI/ingame/bossHp.png"));
	addChild(bossHpBar = new Sprite("image/UI/ingame/bossHpbar.png"));
	bossHpBar->color.a = 0;
	bossHpBack->color.a = 0;
	bossHpBar->pos = Vec2(350, 660);
	bossHpBack->pos = Vec2(333, 618);

	if (gameManager.stage == 1)
	{
		addChild(stageStart = new Sprite("image/UI/ingame/text/1stage.png"));
		addChild(stageClear = new Sprite("image/UI/ingame/text/1stageClear.png"));
	}
	else
	{
		addChild(stageStart = new Sprite("image/UI/ingame/text/2stage.png"));
		addChild(stageClear = new Sprite("image/UI/ingame/text/gameClear.png"));
	}

	stageStart->setCenter(Vec2(-500, 720 / 2));
	stageClear->setCenter(Vec2(-500, 720 / 2));

	addChild(combo = new Sprite("image/UI/ingame/combo.png"));
	combo->pos = Vec2(1100, 398);
	combo->color.a = 0;

	addChild(warning = new Sprite("image/UI/ingame/text/warning.png"));
	addChild(warningBar[0] = new Sprite("image/UI/ingame/warningBar.png"));
	addChild(warningBar[1] = new Sprite("image/UI/ingame/warningBar.png"));
	warning->setCenter(Vec2(1280 / 2, 720 / 2));
	warningBar[1]->pos = Vec2(-1120, 682);
	warning->color.a = 0.3f;
	warningBar[0]->color.a = 0.3f;
	warningBar[1]->color.a = 0.3f;
	warning->visible = false;
	warningBar[0]->visible = false;
	warningBar[1]->visible = false;

	for (int i = 0; i < 6; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			addChild(scoreText[i][j] = new Sprite("image/UI/ingame/scoreNumber/" + to_string(j) + ".png"));
			scoreText[i][j]->pos = Vec2(1103 + 24 * i, 70);
			scoreText[i][j]->visible = false;
		}
		scoreText[i][0]->visible = true;
	}

	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			addChild(bombText[i][j] = new Sprite("image/UI/ingame/scoreNumber/" + to_string(j) + ".png"));
			bombText[i][j]->pos = Vec2(162 + 24 * i, 134);
			bombText[i][j]->visible = false;
		}
		bombText[i][0]->visible = true;
	}

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			addChild(comboText[i][j] = new Sprite("image/UI/ingame/combotext/" + to_string(j) + ".png"));
			comboText[i][j]->pos = Vec2(987 + 36 * i, 397);
			comboText[i][j]->color.a = 0;
			comboText[i][j]->visible = false;
			comboText[i][j]->scaleCenter = comboText[i][j]->rect.center();
		}
	}

	addChild(continueScene = new Sprite("image/UI/ingame/continue/continue.png"));
	addChild(yes = new Sprite("image/UI/ingame/continue/YES.png"));
	addChild(no = new Sprite("image/UI/ingame/continue/NO.png"));
	yes->pos = Vec2(200, 486);
	no->pos = Vec2(926, 482);
	continueScene->color.a = 0;
	yes->color.a = 0;
	yes->scaleCenter = yes->rect.center();
	no->color.a = 0;
	no->scaleCenter = no->rect.center();

	addChild(back = new Sprite("image/UI/background.png"));
	back->visible = false;
	addChild(nextScene = new Animation("image/UI/nextstage", 20, 12, false));
	nextScene->visible = false;
	nextScene->stop = true;
	
	addChild(ed = new Animation("image/UI/ed", 25, 12, false));
	ed->visible = false;
	ed->stop = true;

	addChild(black = new Sprite("image/black.png"));
	addChild(gameOver = new Sprite("image/UI/ingame/text/gameOver.png"));
	gameOver->color.a = 0;
	gameOver->setCenter(Vec2(1280 / 2, 720 / 2));
}

UI::~UI()
{
}
