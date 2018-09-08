#pragma once
#include "Entity.h"
class Text : public Entity
{
public:
	Text(string text, int size);
	~Text();

	void render();

	string text;

	LPD3DXFONT font;
	D3DXCOLOR color;
};

