#include "DXUT.h"
#include "Text.h"
#include "World.h"

Text::Text(string text, int size) : text(text), color(1, 1, 1, 1)
{
	D3DXCreateFontA(DXUTGetD3D9Device(), size, size / 2 - size / 10, FW_HEAVY, D3DX_DEFAULT, NULL, DEFAULT_CHARSET, NULL, ANTIALIASED_QUALITY, NULL, "¸¼Àº°íµñ", &font);
}

Text::~Text()
{
	SAFE_RELEASE(font);
}

void Text::render()
{
	Entity::render();
	world.rootSprite->Begin(D3DXSPRITE_ALPHABLEND);
	world.rootSprite->SetTransform(&matrix);
	font->DrawTextA(world.rootSprite, text.data(), -1, NULL, DT_TOP | DT_LEFT, color);
	world.rootSprite->End();
}