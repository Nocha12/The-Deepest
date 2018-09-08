#pragma once
#include "Define.h"
#include "Scene.h"
#include "Timer.h"

class World
{
public:
	World();
	~World();

	void init();
	void dispose();
	void update(float dt);
	void render();
	int getKeyState(int vk);
	Vec2 getMousePos();
	void changeScene(Scene *scene);
	float timeScale;
	bool currentKeys[256];
	bool lastKeys[256];
	Scene *currentScene;
	list<Timer*> timers; 
	LPD3DXSPRITE rootSprite;
	POINT mousePoint;
};

extern World world;