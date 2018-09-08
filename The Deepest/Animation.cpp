#include "DXUT.h"
#include "Animation.h"
#include "Asset.h"

Animation::Animation(string p, int fileNum, float fps, bool loop) : fps(fps), currentFrame(0), loop(loop), stop(false)
{
	for (int i = 0; i <= fileNum; ++i)
	{
		Texture* t = asset.getTexture(p + "/" + to_string(i) + ".png");
		textures.push_back(t);
	}
	texture = textures[0];
	rect = visibleRect = Rect(0, texture->info.Width, 0, texture->info.Height);
}

Animation::~Animation()
{
}

void Animation::update(float dt)
{
	if (stop) return;
	Sprite::update(dt);
	
	currentFrame += fps * dt;

	if (currentFrame > textures.size() - 1)
	{
		if (loop)
			currentFrame = 0;
		else
			currentFrame = textures.size() - 1;
	}

	texture = textures[(int)currentFrame];
}
