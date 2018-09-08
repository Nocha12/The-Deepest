#include "DXUT.h"
#include "GameManager.h"


GameManager::GameManager() : upgradeCount(0), waterZetCount(0), waterZet3Count(0), homingWaterZetCount(0), bombCount(3), score(0), combo(0), bossCount(0), stage(1), addOnCount(0), isGod(false)
{
}

GameManager::~GameManager()
{
}

void GameManager::init()
{
	upgradeCount = 0;
	waterZetCount = 0;
	waterZet3Count = 0;
	homingWaterZetCount = 0;
	bombCount = 3;
	score = 0;
	combo = 0;
	bossCount = 0;
	stage = 1;
	addOnCount = 0;
	isGod = false;
}

GameManager gameManager;