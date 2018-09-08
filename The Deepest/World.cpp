#include "DXUT.h"
#include "World.h"
#include "Asset.h"
#include "GameScene.h"
#include "LodingScene.h"
#include "RanknigScene.h"

World::World() : currentScene(nullptr), timeScale(1)
{
}

World::~World()
{
}

void World::init()
{
	asset.init();
	world.changeScene(new LodingScene());
	D3DXCreateSprite(DXUTGetD3D9Device(), &rootSprite);
	ZeroMemory(currentKeys, sizeof(currentKeys));
	ZeroMemory(lastKeys, sizeof(lastKeys));
}

void World::dispose()
{
	asset.dispose();
	SAFE_DELETE(currentScene);
	SAFE_RELEASE(rootSprite);
}

void World::update(float dt)
{
	dt *= timeScale;
	if (currentScene) currentScene->update(dt);
	for (int i = 0; i < 256; ++i)
	{
		lastKeys[i] = currentKeys[i];
		currentKeys[i] = GetAsyncKeyState(i) & 0x8000; 
	}
	GetCursorPos(&mousePoint);
	ScreenToClient(DXUTGetHWND(), &mousePoint);
	for_iter(iter, timers)
		(*iter)->update(dt);
}

void World::render()
{
	if (currentScene) currentScene->render();
}

int World::getKeyState(int vk)
{
	if (currentKeys[vk] && lastKeys[vk]) return 2;
	if (!currentKeys[vk] && lastKeys[vk]) return 1;
	if (currentKeys[vk] && !lastKeys[vk]) return -1;
	return -2;
}

void World::changeScene(Scene *scene)
{
	if (currentScene) SAFE_DELETE(currentScene);
	currentScene = scene;
}

Vec2 World::getMousePos()
{
	return Vec2(mousePoint.x, mousePoint.y);
}

World world;