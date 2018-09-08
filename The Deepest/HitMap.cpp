#include "DXUT.h"
#include "HitMap.h"


HitMap::HitMap(string p)
{
	addChild(img = new Sprite(p));
	initMap(img);
}

HitMap::~HitMap()
{
	for (int i = 0; i < mapWidth; ++i)
		SAFE_DELETE(map[i]);
	SAFE_DELETE(map);
}

void HitMap::initMap(Sprite *img)
{
	DWORD *data;
	D3DLOCKED_RECT lockRect;
	LPDIRECT3DSURFACE9 surface;

	const int x = 1580;
	const int y = 2083;

	map = new bool*[x];
	for (int i = 0; i < x; ++i)
		map[i] = new bool[y];

	for (int i = 0; i < x; ++i)
		for (int j = 0; j < y; ++j)
			map[i][j] = false;

	img->texture->d3dTexture->GetSurfaceLevel(0, &surface);
	surface->LockRect(&lockRect, 0, 0);
	data = (DWORD*)lockRect.pBits;

	for (int i = 0; i < y; ++i)
		for (int j = 0; j < x; ++j)
			if (data[i * lockRect.Pitch / 4 + j] < D3DCOLOR_XRGB(10, 10, 10))
				map[j][i] = true;

	surface->UnlockRect();
	SAFE_RELEASE(surface);
}