#pragma once
#include "Entity.h"
#include "Texture.h"

class Sprite : public Entity
{
public:
	Sprite();
	Sprite(path p);
	~Sprite();

	void setPath(path p);
	void setTexture(Texture *t);
	void render();
	
	Texture *texture;
	D3DXCOLOR color;
};

