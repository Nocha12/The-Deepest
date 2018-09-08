#pragma once
#include "Scene.h"
#include "Text.h"
#include <fstream>

struct Rank {
	string name;
	int score;
};

class ViewRankingScene :
	public Scene
{
public:
	ViewRankingScene(string name, int score);
	~ViewRankingScene();

	Text *scoreT[5];
	Text *nameT[5];

	ifstream input;
	ofstream output;

	list<Rank> ranks;

	void update(float dt);
};

