#pragma once
#include "Scene.h"
#include "Text.h"

class RanknigScene : public Scene
{
public:
	RanknigScene(int score);
	~RanknigScene();

	void update(float dt);

	int score;
	Text *t;
};

