#pragma once
#include "Entity.h"
#include "Sprite.h"

class AfterImage :
	public Entity
{
public:
	AfterImage(Sprite *img);
	~AfterImage();

	void update(float dt);

	Sprite *img;
};

