#pragma once
#include "Scene.h"
#include "Animation.h"

class MainScene : public Scene
{
public:
	MainScene();
	~MainScene();

	void update(float dt);

	Sprite *title;
	Sprite *gameStartB;
	Sprite *howToB;
	Sprite *introB;
	Sprite *rankB;

	Sprite *creditB;
	Sprite *exitB;

	Sprite *creditEx;
	Sprite *introEx;
	Sprite *howToEx[3];
	int howToCount;
};

