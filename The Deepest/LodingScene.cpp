#include "DXUT.h"
#include "LodingScene.h"
#include "Asset.h"
#include "World.h"
#include "MainScene.h"

LodingScene::LodingScene()
{
	addChild(new Sprite("image/UI/background.png"));
	addChild(new Animation("image/UI/load/ani", 20, 12, true));
	addChild(new Sprite("image/UI/load/load.png"));
	Animation *ani;
	addChild(ani = new Animation("image/UI/load/load", 2, 4, true));
	ani->pos = Vec2(986, 53);
	addChild(bar = new Sprite("image/UI/load/bar.png"));
	bar->pos = Vec2(61, 618);
	asset.ready("image");

	addChild(ed = new Animation("image/UI/op", 25, 12, false));
	ed->visible = false;
	ed->stop = true;
	asset.getSound(L"sound/main.wav");
	asset.getSound(L"sound/ingame.wav");
}


LodingScene::~LodingScene()
{
}

void LodingScene::update(float dt)
{
	Scene::update(dt);

	if (ed->visible)
	{
		if(world.getKeyState(VK_RETURN) == 1)
		{
			ed->stop = false;	
		}

		if (ed->currentFrame >= ed->textures.size() - 1)
		{
			world.changeScene(new MainScene());
			return;
		}

		return;
	}

	for (int i = 0; i < 1; ++i)
	{
		asset.loadNext();
		bar->visibleRect.right = bar->rect.right * asset.filesLoaded / asset.filesToLoad;

		if (asset.filesLoaded == asset.filesToLoad)
		{
			ed->visible = true;
			return;
		}
	}
}