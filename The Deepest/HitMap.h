#pragma once
#include "Entity.h"
#include "Sprite.h"

class HitMap : public Entity
{
public:
	HitMap(string p);
	~HitMap();

	void initMap(Sprite *img);

	Sprite *img;
	int mapWidth;
	bool **map;
};

