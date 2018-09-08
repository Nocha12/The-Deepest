#pragma once
#include "Sprite.h"
class Animation :
	public Sprite
{
public:
	Animation(string p, int fileNum, float fps, bool loop);
	~Animation();

	bool stop;
	float fps;
	bool loop;
	float currentFrame;

	void update(float dt);

	vector<Texture*> textures;
};

