#pragma once
#include "Define.h"
#include "Texture.h"

class Asset
{
public:
	Asset();
	~Asset();

	void init();
	void dispose();
	void ready(path p);
	void loadNext();
	Texture* getTexture(path p);
	CSound* getSound(wpath p);
	map<path, Texture*> textures;
	map<wpath, CSound*> sounds;
	CSoundManager _s;

	list<path> paths;
	list<path>::iterator pathsIter;

	int filesToLoad;
	int filesLoaded;
};

extern Asset asset;