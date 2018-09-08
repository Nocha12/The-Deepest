#pragma once
class GameManager
{
public:
	GameManager();
	~GameManager();

	int upgradeCount;
	int waterZetCount;
	int waterZet3Count;
	int homingWaterZetCount;
	int bombCount;
	int score;
	int combo;
	int bossCount;
	int stage;
	int addOnCount;
	bool isGod;

	void init();
};

extern GameManager gameManager;

