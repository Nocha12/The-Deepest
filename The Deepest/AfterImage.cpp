#include "DXUT.h"
#include "AfterImage.h"
#include "World.h"
#include "GameScene.h"

AfterImage::AfterImage(Sprite *img) : img(img)
{
	img->color.a = 0.6f;
	addChild(img);
	rect = img->rect;
	rotationCenter = img->rect.center();
}

AfterImage::~AfterImage()
{
}

void AfterImage::update(float dt)
{
	Entity::update(dt);

	img->color.a -= dt;
	
	if (img->color.a <= 0)
	{
		removeChild(this);
		((GameScene*)world.currentScene)->afterImageList.remove(this);
	}
}