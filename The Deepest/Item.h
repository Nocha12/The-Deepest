#pragma once
#include "Entity.h"
#include "Animation.h"
class Item : public Entity
{
public:
	Item(ItemType type);
	~Item();

	ItemType type;

	Animation *ani;
	Sprite *bubble;
	void update(float dt);
};

