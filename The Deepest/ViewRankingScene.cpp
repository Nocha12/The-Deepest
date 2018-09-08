#include "DXUT.h"
#include "ViewRankingScene.h"
#include "Sprite.h"
#include "World.h"
#include "MainScene.h"

ViewRankingScene::ViewRankingScene(string name, int score) : input("data.txt"), output("data.txt", ios::app | ios::end)
{
	addChild(new Sprite("image/UI/title/rank/1.png"));

	if (name != "")
		output << name << ' ' << score << endl;

	for (int i = 0; i < 5; ++i)
	{
		addChild(nameT[i] = new Text("", 32));
		addChild(scoreT[i] = new Text("", 32));
		nameT[i]->pos = Vec2(247, 217 + 86 * i);
		scoreT[i]->pos = Vec2(897, 217 + 86 * i);
	}

	Rank r;

	while (input >> r.name >> r.score)
		ranks.push_back(r);

	ranks.sort([](Rank r1, Rank r2){
		return r1.score > r2.score;
	});

	int i = 0; // 87
	for_iter(iter, ranks)
	{
		nameT[i]->text = iter->name;
		scoreT[i]->text = to_string(iter->score);
		if (iter->score == score && iter->name == name)
		{
			Sprite *t;
			addChild(t = new Sprite("image/UI/title/rank/triangle.png"));
			t->pos = Vec2(107, 214 + 86 * i);
		}
		if (++i == 5)
			break;
	}
}

ViewRankingScene::~ViewRankingScene()
{
}

void ViewRankingScene::update(float dt)
{
	Scene::update(dt);
	if (world.getKeyState(VK_RETURN) == 1)
	{
		world.changeScene(new MainScene());
		return;
	}
}