#pragma once
#include "Define.h"
#include "Enum.h"

class Entity
{
public:
	Entity();
	virtual ~Entity();

	virtual void update(float dt);
	virtual void render();

	void addChild(Entity *child);
	void removeChild(Entity *child);

	void setCenter(Vec2 p);
	Vec2 center();
	Rect rectWithPos();

	list<Entity*> children;
	Entity *parent;
	float rotation;
	Vec2 rotationCenter;
	Vec2 scale;
	Vec2 scaleCenter;
	D3DXMATRIX matrix;
	Rect rect;
	Rect visibleRect;
	bool visible;
	bool removing;
	Vec2 pos;
};

