#include "DXUT.h"
#include "MainScene.h"
#include "GameScene.h"
#include "World.h"
#include "ViewRankingScene.h"
#include "Asset.h"

MainScene::MainScene() : howToCount(-1)
{
	addChild(new Sprite("image/UI/background.png"));
	addChild(new Animation("image/UI/title/ani", 10, 10, true));
	addChild(new Sprite("image/UI/title/titleFront.png"));
	addChild(title = new Sprite("image/UI/title/title.png"));
	title->pos = Vec2(66, 68); 
	addChild(gameStartB = new Sprite("image/UI/title/gameStart.png"));
	gameStartB->pos = Vec2(140, 325);
	addChild(howToB = new Sprite("image/UI/title/howto.png"));
	howToB->pos = Vec2(126, 385);
	addChild(introB = new Sprite("image/UI/title/introB.png"));
	introB->pos = Vec2(154, 443);
	addChild(rankB = new Sprite("image/UI/title/rank.png"));
	rankB->pos = Vec2(177, 501);
	addChild(creditB = new Sprite("image/UI/title/creditB.png"));
	creditB->pos = Vec2(201, 561);
	addChild(exitB = new Sprite("image/UI/title/exit.png"));
	exitB->pos = Vec2(158, 619);

	addChild(howToEx[0] = new Sprite("image/UI/title/howto/0.png"));
	howToEx[0]->visible = false;
	addChild(howToEx[1] = new Sprite("image/UI/title/howto/1.png"));
	howToEx[1]->visible = false;
	addChild(howToEx[2] = new Sprite("image/UI/title/howto/2.png"));
	howToEx[2]->visible = false;

	addChild(introEx = new Sprite("image/UI/title/intro.png"));
	introEx->visible = false;
	addChild(creditEx = new Sprite("image/UI/title/credit.png"));
	creditEx->visible = false;

	title->scaleCenter = title->rect.center();
	gameStartB->scaleCenter = gameStartB->rect.center();
	howToB->scaleCenter = howToB->rect.center();
	introB->scaleCenter = introB->rect.center();
	rankB->scaleCenter = rankB->rect.center();
	creditB->scaleCenter = creditB->rect.center();
	exitB->scaleCenter = exitB->rect.center();
	asset.sounds[L"sound/main.wav"]->Play();
}

MainScene::~MainScene()
{
}

void MainScene::update(float dt)
{
	Scene::update(dt);

	if (!asset.sounds[L"sound/main.wav"]->IsSoundPlaying())
		asset.sounds[L"sound/main.wav"]->Play();

	if (howToCount >= 0)
	{
		if (world.getKeyState(VK_LBUTTON) == 1)
		{
			if (howToCount < 2)
			{
				howToEx[howToCount]->visible = false;
				howToEx[++howToCount]->visible = true;
			}
			else
			{
				howToEx[2]->visible = false;
				howToCount = -1;
			}
		}
	}
	else if (creditEx->visible)
	{
		if (world.getKeyState(VK_LBUTTON) == 1)
			creditEx->visible = false;
	}
	else if (introEx->visible)
	{
		if (world.getKeyState(VK_LBUTTON) == 1)
			introEx->visible = false;
	}
	else
	{
		if (gameStartB->rectWithPos().contains(world.getMousePos()))
		{
			gameStartB->scale = Vec2(1.2f, 1.2f);
			if (world.getKeyState(VK_LBUTTON) == 1)
			{
				asset.sounds[L"sound/main.wav"]->Stop();
				world.changeScene(new GameScene());
				return;
			}
		}
		else
			gameStartB->scale = Vec2(1, 1);

		if (howToB->rectWithPos().contains(world.getMousePos()))
		{
			howToB->scale = Vec2(1.2f, 1.2f);
			if (world.getKeyState(VK_LBUTTON) == 1)
			{
				howToCount = 0;
				howToEx[howToCount]->visible = true;
			}
		}
		else
			howToB->scale = Vec2(1, 1);

		if (introB->rectWithPos().contains(world.getMousePos()))
		{
			introB->scale = Vec2(1.2f, 1.2f);
			if (world.getKeyState(VK_LBUTTON) == 1)
			{
				introEx->visible = true;
			}
		}
		else
			introB->scale = Vec2(1, 1);

		if (rankB->rectWithPos().contains(world.getMousePos()))
		{
			rankB->scale = Vec2(1.2f, 1.2f);
			if (world.getKeyState(VK_LBUTTON) == 1)
			{
				asset.sounds[L"sound/main.wav"]->Stop();
				world.changeScene(new ViewRankingScene("", 0));
				return;
			}
		}
		else
			rankB->scale = Vec2(1, 1);

		if (creditB->rectWithPos().contains(world.getMousePos()))
		{
			creditB->scale = Vec2(1.2f, 1.2f);
			if (world.getKeyState(VK_LBUTTON) == 1)
			{
				creditEx->visible = true;
			}
		}
		else
			creditB->scale = Vec2(1, 1);

		if (exitB->rectWithPos().contains(world.getMousePos()))
		{
			exitB->scale = Vec2(1.2f, 1.2f);
			if (world.getKeyState(VK_LBUTTON) == 1)
			{
				exit(1);
				return;
			}
		}
		else
			exitB->scale = Vec2(1, 1);
	}
}