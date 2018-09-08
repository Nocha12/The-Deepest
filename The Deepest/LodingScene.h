#pragma once
#include "Scene.h"
#include "Animation.h"

class LodingScene :
	public Scene
{
public:
	LodingScene();
	~LodingScene();

	void update(float dt);
	Animation *ed;
	Sprite *bar;
};

