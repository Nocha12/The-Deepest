#include "DXUT.h"
#include "Item.h"

Item::Item(ItemType type) : type(type)
{
	if (type == WaterZetItem)
		addChild(ani = new Animation("image/item/torpedo", 10, 10, true));
	else if (type == WaterZet3Item)
		addChild(ani = new Animation("image/item/3direct", 10, 10, true));
	else if (type == HomingWaterZetItem)
		addChild(ani = new Animation("image/item/homing", 10, 10, true));
	else if (type == AddOnItem)
		addChild(ani = new Animation("image/item/addon", 10, 10, true));
	else if (type == HpItem)
		addChild(ani = new Animation("image/item/hp", 10, 10, true));
	else if (type == NuclearItem)
		addChild(ani = new Animation("image/item/nuclear", 10, 10, true));
	else if (type == PowerItem)
		addChild(ani = new Animation("image/item/power", 10, 10, true));
	else if (type == SpeedItem)
		addChild(ani = new Animation("image/item/speed", 10, 10, true));
	
	addChild(bubble = new Sprite("image/item/bubble.png"));
	rect = ani->rect;
	bubble->setCenter(rect.center()); 
}

Item::~Item()
{
}

void Item::update(float dt)
{
	Entity::update(dt);

	pos.y += 80 * dt;
}
