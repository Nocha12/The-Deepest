#include "DXUT.h"
#include "RanknigScene.h"
#include "World.h"
#include "ViewRankingScene.h"
#include "Sprite.h"

RanknigScene::RanknigScene(int score) : score(score)
{
	addChild(new Sprite("image/UI/title/rank/0.png"));
	addChild(t = new Text("", 55));
	t->pos = Vec2(361, 423);
}

RanknigScene::~RanknigScene()
{
}

void RanknigScene::update(float dt)
{
	Scene::update(dt);

	for (int i = 0; i < 256; ++i)
	{
		if (world.getKeyState(i) == 1)
		{
			if (i == VK_BACK && t->text.length() > 0)
				t->text.pop_back();
			if (i >= 'A' && i <= 'Z' && t->text.length() < 5)
			{
				t->text.push_back(i);
			}if (i == VK_RETURN && t->text.length() > 0)
			{
				world.changeScene(new ViewRankingScene(t->text, score));
				return;
			}
		}
	}
}