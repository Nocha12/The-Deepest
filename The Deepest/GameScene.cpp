#include "DXUT.h"
#include "GameScene.h"
#include "World.h"
#include "RanknigScene.h"
#include "GameManager.h"
#include "MainScene.h"
#include "Asset.h"

GameScene::GameScene() : mapScrollSpeed(300), backScrollSpeed(100), isBoss(false), isClear(false), gameClearTimer(0), gameOver(false), warning(false), itsHighNoon(false)
{
	ui = new UI();
	blue = new Sprite("image/map/2stage/0.png");
	addChild(player = new Player());
	player->setCenter(Vec2(1280 / 2, 720 / 2 + 720 / 3));
	follow(player);
	addChild(enemyManager = new EnemyManager());
	if (gameManager.bossCount == 1)
	{
		enemyManager->patternTimer = 50;
		enemyManager->pattern = 9;
	}
	addChild(back[0] = new Sprite("image/map/" + to_string(gameManager.stage) + "stage/back.png"));
	addChild(back[1] = new Sprite("image/map/" + to_string(gameManager.stage) + "stage/back.png"));
	back[0]->pos = Vec2(-147, -900);
	back[1]->pos = Vec2(0, -2080) + Vec2(-147, -900);
	addChild(map[0] = new Sprite("image/map/" + to_string(gameManager.stage) + "stage/map/1.png"));
	addChild(map[1] = new Sprite("image/map/" + to_string(gameManager.stage) + "stage/map/3.png"));
	map[0]->pos = Vec2(-147, -900);
	map[1]->pos = Vec2(0, -1800) + Vec2(-147, -900);
	hitMap[0] = new HitMap("image/map/" + to_string(gameManager.stage) + "stage/hit/1.png");
	hitMap[1] = new HitMap("image/map/" + to_string(gameManager.stage) + "stage/hit/3.png");
	asset.sounds[L"sound/ingame.wav"]->Play();
	moveUIList.push_back(ui->stageStart);
}

GameScene::~GameScene()
{
}

void GameScene::update(float dt)
{
	if (!asset.sounds[L"sound/ingame.wav"]->IsSoundPlaying())
		asset.sounds[L"sound/ingame.wav"]->Play();
	if (ui->nextScene->currentFrame >= ui->nextScene->textures.size() - 1)
	{
		gameManager.bossCount = 0;
		gameManager.stage = 2;
		asset.sounds[L"sound/ingame.wav"]->Stop();
		world.changeScene(new GameScene());
		return;
	}

	if (world.getKeyState('C') == 1 && !enemyList.empty())
	{
		enemyListIter = enemyList.begin();
		
		if (!itsHighNoon)
		{
			faidInList.push_back(ui->highNoon);
			world.timeScale = 0.2f;
			itsHighNoon = true;
			itsHighNoonTimer.reset(0.02f, enemyList.size());
			itsHighNoonTimer.onTick = [=](){
				if (enemyListIter != enemyList.end())
				{
					Targetting *t = new Targetting(*enemyListIter);
					addChild(t);
					targettingList.push_back(t);
					enemyListIter++;
				}
			};
		}
		else if (ui->highNoon->color.a >= 1)
		{
			faidOutList.push_back(ui->highNoon);
			world.timeScale = 1;
			itsHighNoon = false;
			itsHighNoonTimer2.reset(0.1f, enemyList.size());
			itsHighNoonTimer2.onTick = [=](){
				if (enemyListIter != enemyList.end())
				{
					for_iter(iter, targettingList)
						if ((*iter)->target == *enemyListIter)
							(*iter)->removeAni = true;
					(*enemyListIter)->hp -= 500;
					Animation *t = new Animation("image/player/effect/base", 5, 15, false);
					addChild(t);
					t->setCenter((*enemyListIter)->center());
					effectList.push_back(t);
					enemyListIter++;
				}
			};
			itsHighNoonTimer2.onFinished = [=](){
				for_iter(iter, targettingList)
				(*iter)->removeAni = true;
			};
		}
	}

	if (ui->ed->currentFrame >= ui->ed->textures.size() - 1)
	{
		asset.sounds[L"sound/ingame.wav"]->Stop();
		world.changeScene(new RanknigScene(gameManager.score));
		gameManager.init();
		return;
	}

	if (ui->ed->visible)
	{
		ui->black->color.a -= 0.01f;
		if (world.getKeyState(VK_RETURN) == 1)
			ui->ed->stop = false;
		ui->ed->update(dt);
		return;
	}

	if (ui->nextScene->visible)
	{
		ui->black->color.a -= 0.01f;
		if (world.getKeyState(VK_RETURN) == 1)
			ui->nextScene->stop = false;
		ui->nextScene->update(dt);
		return;
	}

	for_iter(iter, faidInList)
	{
		(*iter)->color.a += 0.01f;

		if ((*iter)->color.a >= 1)
		{
			iter = faidInList.erase(iter);
			if (iter == faidInList.end())
				break;
		}
	}

	for_iter(iter, faidOutList)
	{
		(*iter)->color.a -= 0.01f;

		if ((*iter)->color.a <= 0)
		{
			iter = faidOutList.erase(iter);
			if (iter == faidOutList.end())
				break;
		}
	}

	if (gameOver && world.getKeyState(VK_RETURN) == 1)
	{
		gameManager.init();
		asset.sounds[L"sound/ingame.wav"]->Stop();
		world.changeScene(new MainScene());
		return;
	}

	if (ui->continueScene->color.a >= 1)
	{
		if (ui->yes->rectWithPos().contains(world.getMousePos()))
		{
			ui->yes->scale = Vec2(1.2f, 1.2f);
			if (world.getKeyState(VK_LBUTTON) == 1)
			{
				gameManager.score = 0;
				asset.sounds[L"sound/ingame.wav"]->Stop();
				world.changeScene(new GameScene());
				return;
			}
		}
		else
			ui->yes->scale = Vec2(1, 1);

		if (ui->no->rectWithPos().contains(world.getMousePos()))
		{
			ui->no->scale = Vec2(1.2f, 1.2f);
			if (world.getKeyState(VK_LBUTTON) == 1)
			{
				faidOutList.push_back(ui->continueScene);
				faidOutList.push_back(ui->yes);
				faidOutList.push_back(ui->no);
				faidInList.push_back(ui->gameOver);
				gameOver = true;
			}
		}
		else
			ui->no->scale = Vec2(1, 1);

		return;
	}

	Scene::update(dt);

	if (warning)
	{
		ui->warning->visible = true;
		ui->warningBar[0]->visible = true;
		ui->warningBar[1]->visible = true;

		ui->warningBar[0]->pos.x -= 100 * dt;
		ui->warningBar[1]->pos.x += 100 * dt;

		if (ui->warning->color.a <= 0.3f && ui->warningBar[0]->color.a <= 0.3f && ui->warningBar[1]->color.a <= 0.3f)
		{
			faidInList.push_back(ui->warning);
			faidInList.push_back(ui->warningBar[0]);
			faidInList.push_back(ui->warningBar[1]);
		}
		else if (ui->warning->color.a > 1 && ui->warningBar[0]->color.a > 1 && ui->warningBar[1]->color.a > 1)
		{
			faidOutList.push_back(ui->warning);
			faidOutList.push_back(ui->warningBar[0]);
			faidOutList.push_back(ui->warningBar[1]);
		}
	}
	else if (ui->warning->color.a <= 0)
	{
		ui->warning->color.a = 0.3f;
		ui->warningBar[0]->color.a = 0.3f;
		ui->warningBar[1]->color.a = 0.3f;
		ui->warning->visible = false;
		ui->warningBar[0]->visible = false;
		ui->warningBar[1]->visible = false;
	}


	if (pos.x > 130)
		pos.x = 130;
	if (pos.x < -130)
		pos.x = -130;

	Rect t;
	if (ui->hit->color.a > 0)
		ui->hit->color.a -= 0.01f;

	if (!isClear && ui->black->color.a > 0 && !gameOver)
		ui->black->color.a -= 0.05f;
	if (gameClearTimer > 5 || gameOver)
		ui->black->color.a += 0.01f;

	if (isClear)
		gameClearTimer += dt;

	if (isClear && ui->black->color.a >= 1 && !gameOver)
	{
		if (gameManager.stage == 1)
		{
			ui->back->visible = true;
			ui->nextScene->visible = true;
			return;
		}
		else
		{
			ui->back->visible = true;
			ui->ed->visible = true;
			return;
		}
	}
	if (world.getKeyState(VK_F4) == 1)
	{
		gameManager.init();
		asset.sounds[L"sound/ingame.wav"]->Stop();
		world.changeScene(new MainScene());
		return;
	}
	if (world.getKeyState(VK_F5) == 1)
	{
		gameManager.bossCount = 0;
		gameManager.stage = 1;
		asset.sounds[L"sound/ingame.wav"]->Stop();
		world.changeScene(new GameScene());
		return;
	}
	if (world.getKeyState(VK_F6) == 1)
	{
		gameManager.bossCount = 0;
		gameManager.stage = 2;
		asset.sounds[L"sound/ingame.wav"]->Stop();
		world.changeScene(new GameScene());
		return;
	}

	//스코어 및 폭탄 텍스트
	int scoreT = gameManager.score;
	for (int i = 0; i < 6; ++i)
	{
		for (int j = 0; j < 10; ++j)
			ui->scoreText[5 - i][j]->visible = false;

		ui->scoreText[5 - i][scoreT % 10]->visible = true;
		scoreT /= 10;
	}
	int bombT = gameManager.bombCount;
	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 10; ++j)
			ui->bombText[1 - i][j]->visible = false;

		ui->bombText[1 - i][bombT % 10]->visible = true;
		bombT /= 10;
	}

	map[0]->pos.y += mapScrollSpeed * dt;
	map[1]->pos.y += mapScrollSpeed * dt;
	back[0]->pos.y += backScrollSpeed * dt;
	back[1]->pos.y += backScrollSpeed * dt;

	if (back[0]->pos.y > 1000)
	{
		removeChild(back[0]);
		back[0] = back[1];
		addChild(back[1] = new Sprite("image/map/" + to_string(gameManager.stage) + "stage/back.png"));
		back[1]->pos = back[0]->pos + Vec2(0, -2080);
	}

	if (map[0]->pos.y > 1000)
	{
		int index = random(1, 3);

		removeChild(map[0]);
		map[0] = map[1];
		if (isBoss)
			addChild(map[1] = new Sprite("image/map/" + to_string(gameManager.stage) + "stage/map/" + to_string(0) + ".png"));
		else
			addChild(map[1] = new Sprite("image/map/" + to_string(gameManager.stage) + "stage/map/" + to_string(index) + ".png"));
		map[1]->pos = map[0]->pos + Vec2(0, -1800);

		SAFE_DELETE(hitMap[0]);
		hitMap[0] = hitMap[1];
		if (isBoss)
			hitMap[1] = new HitMap("image/map/" + to_string(gameManager.stage) + "stage/hit/" + to_string(0) + ".png");
		else
			hitMap[1] = new HitMap("image/map/" + to_string(gameManager.stage) + "stage/hit/" + to_string(index) + ".png");
	}

	for_iter(enemy, enemyList)
	{
		if ((*enemy)->rectWithPos().intersects(player->rectWithPos(), &t) && player->hp > 0)
		{
			player->GetRed();
			shake(0.01, 5, 10, false);
			player->hp -= 0.5f;
			gameManager.combo = 0;
			ui->hit->color.a = 1 - player->hp / player->maxHp;
		}
	}

	for_iter(bullet, bulletList) // 총알 충돌처리
	{
		bool dump = false;

		if ((*bullet)->isPlayerBullet) // 플레이어 총알 -> 적
		{
			for_iter(enemy, enemyList)
			{
				if ((*enemy)->rectWithPos().intersects((*bullet)->rectWithPos(), &t))
				{
					ChangeCombo();
					Animation *eff;
					if ((*bullet)->type == PlayerBullet)
						eff = new Animation("image/player/effect/base", 5, 15, false);
					else if ((*bullet)->type == PlayerHoming)
						eff = new Animation("image/player/effect/homing", 5, 15, false);
					else if ((*bullet)->type == PlayerWaterZet)
						eff = new Animation("image/player/effect/torpedo", 5, 15, false);
					else if ((*bullet)->type == Nuclear)
						eff = new Animation("image/player/effect/nuclear/peobeong", 24, 15, false);

					addChild(eff);
					effectList.push_back(eff);
					eff->setCenter((*bullet)->center());

					gameManager.score += 10;

					if ((*bullet)->type == Nuclear)
					{
						for_iter(iter, enemyList)
							(*iter)->hp -= 500;
						int i = 0;
						for_iter(iter, bulletList)
						{
							if ((*iter)->isPlayerBullet) continue;
							cout << ++i << " : " << (*iter)->isPlayerBullet << " : " << bulletList.size() << endl;

							Sprite *dieEff = new Sprite("image/mob/effect/die/0.png");
							dieEff->scale = Vec2(0, 0);
							dieEff->scaleCenter = dieEff->rect.center();
							dieEff->rotationCenter = dieEff->rect.center();
							addChild(dieEff);
							dieEffectList.push_back(dieEff);
							dieEff->setCenter((*iter)->center());
							
							for (int i = 0; i < 5; ++i)
							{
								Particle *p = new Particle(D3DXToRadian(random(0, 360)), random(100, 200), player);
								addChild(p);
								particleList.push_back(p);
								p->setCenter((*iter)->center());
							}
							cout << "delete" << endl;
							removeChild(*iter);
							iter = bulletList.erase(iter);
							if (iter == bulletList.end())
								break;
							else
								advance(iter, -1);
						}
						//bool b = true;
						//while (b){
						//	b = false;
						//	for_iter(iter, bulletList){
						//		if (!(*iter)->removing) continue;
						//		iter = bulletList.erase(iter);
						//		if (iter == bulletList.end()){
						//			break;
						//		}
						//	}
						//	for (auto &iter : bulletList){
						//		if (iter->isPlayerBullet) continue;
						//		if (iter->removing)
						//			b = true;
						//	}
						//}
						shake(0.01, 30, 30, true);
					}
					shake(0.01, 5, 3, false);
					(*enemy)->hp -= (*bullet)->damage;
					(*enemy)->GetRed();
					dump = true;
				}
			}
		}
		else // 적 총알 -> 플레이어
		{
			if ((*bullet)->rectWithPos().intersects(player->rectWithPos(), &t) && player->hp > 0)
			{
				gameManager.combo = 0;
				player->GetRed();
				shake(0.01, 5, 10, false);
				player->hp -= (*bullet)->damage;
				ui->hit->color.a = 1 - player->hp / player->maxHp;
				dump = true;
				Animation *eff;
				if ((*bullet)->type == S1_OctoBullet || (*bullet)->type == S1_FishBullet)
					eff = new Animation("image/mob/effect/1", 4, 15, false);
				else if ((*bullet)->type == S1_MidBullet || (*bullet)->type == S1_FinalDropBullet || (*bullet)->type == S1_FinalBullet)
					eff = new Animation("image/boss/effect/1", 4, 15, false);
				else if ((*bullet)->type == S2_OctoBullet || (*bullet)->type == S2_FishBullet)
					eff = new Animation("image/mob/effect/2", 4, 15, false);
				else if ((*bullet)->type == S2_MidBullet)
					eff = new Animation("image/boss/effect/2", 4, 15, false);

				if ((*bullet)->type != S2_FinalStorm && (*bullet)->type != S2_FinalSword)
				{
					addChild(eff);
					effectList.push_back(eff);
					eff->setCenter((*bullet)->center());
				}
				else if (player->effTimer >= 0.3f)
				{
					player->effTimer = 0;
					eff = new Animation("image/boss/effect/2", 4, 15, false);
					addChild(eff);
					effectList.push_back(eff);
					eff->setCenter(player->center());
				}
			}
		}
		if (dump && (*bullet)->type != S2_FinalStorm && (*bullet)->type != S2_FinalSword)
		{
			removeChild(*bullet);
			bullet = bulletList.erase(bullet);
			if (bullet == bulletList.end())
				return;
		}
	}

	for_iter(iter, seaWeedList)
	{
		(*iter)->pos.y += backScrollSpeed * dt;
		if ((*iter)->rectWithPos().intersects(player->rectWithPos(), &t))
		{
			player->GetBlack();
			player->currentSpeed = 100;
		}
		else if (player->currentSpeed != 700)
			player->currentSpeed = 300;

		if ((*iter)->pos.y > 720 + 200)
		{
			removeChild(*iter);
			iter = seaWeedList.erase(iter);
			if (iter == seaWeedList.end())
				break;
		}
	}

	for_iter(iter, bubbleList)
	{
		(*iter)->color.a -= dt;
		(*iter)->pos.y += 150 * dt;
		if ((*iter)->color.a <= 0)
		{
			removeChild(*iter);
			iter = bubbleList.erase(iter);
			if (iter == bubbleList.end())
				break;
		}
	}

	for_iter(iter, effectList)
	{
		if ((*iter)->currentFrame >= (*iter)->textures.size() - 1)
		{
			removeChild(*iter);
			iter = effectList.erase(iter);
			if (iter == effectList.end())
				break;
		}
	}

	for_iter(iter, itemTextList)
	{
		(*iter)->color.a -= 0.01f;
		(*iter)->pos.y -= 50 * dt;
		if ((*iter)->color.a <= 0)
		{
			removeChild(*iter);
			iter = itemTextList.erase(iter);
			if (iter == itemTextList.end())
				break;
		}
	}

	for_iter(iter, dieEffectList)
	{
		(*iter)->color.a -= dt * 1.5f;
		(*iter)->scale += Vec2(dt * 1.5f, dt * 1.5f);
		(*iter)->rotation += 10 * dt;
		if ((*iter)->color.a <= 0)
		{
			removeChild(*iter);
			iter = dieEffectList.erase(iter);
			if (iter == dieEffectList.end())
				break;
		}
	}

	for_iter(iter, moveUIList)
	{
		int moveSpeed = abs(1280 / 2 - (*iter)->center().x);
		if (moveSpeed <= 150)
			moveSpeed = 150;

		(*iter)->pos.x += moveSpeed * dt;

		if ((*iter)->pos.x > 1280 + 147)
		{
			removeChild(*iter);
			iter = moveUIList.erase(iter);
			if (iter == moveUIList.end())
				break;
		}
	}
}

void GameScene::render()
{
	Scene::render();

	back[0]->render();
	back[1]->render();

	for_iter(iter, seaWeedList)
		(*iter)->render();

	for_iter(iter, itemList)
		(*iter)->render();

	for_iter(iter, enemyList)
		(*iter)->render();

	for_iter(iter, afterImageList)
		(*iter)->render();

	for_iter(iter, bulletList)
		(*iter)->render();

	for_iter(iter, particleList)
		(*iter)->render();

	for_iter(iter, dieEffectList)
		(*iter)->render();

	for_iter(iter, targettingList)
		(*iter)->render();

	map[0]->render();
	map[1]->render();

	player->render();

	for_iter(iter, bubbleList)
		(*iter)->render();

	for_iter(iter, itemTextList)
		(*iter)->render();

	for_iter(iter, bossArmList)
		(*iter)->render();

	for_iter(iter, effectList)
		(*iter)->render();

	if (gameManager.stage == 2)
		blue->render();

	ui->render();
}

void GameScene::ChangeCombo()
{
	if (++gameManager.combo >= 999)
		gameManager.combo = 999;

	int comboT = gameManager.combo;

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			ui->comboText[2 - i][j]->visible = false;
			ui->comboText[2 - i][j]->scale = Vec2(2, 2);
			ui->comboText[2 - i][j]->color.a = 1;
		}
		ui->comboText[2 - i][comboT % 10]->visible = true;
		comboT /= 10;
	}
	ui->combo->color.a = 1;
	comboScaleTimer.reset(0.01f, 25);
	comboScaleTimer.onTick = [=](){
		for (int i = 0; i < 3; ++i)
			for (int j = 0; j < 10; ++j)
				if (ui->comboText[i][j]->visible)
					ui->comboText[i][j]->scale -= Vec2(0.04f, 0.04f);
	};
	comboScaleTimer.onFinished = [=](){
		comboVisibleTimer.reset(0.01f, 25);
		comboVisibleTimer.onTick = [=](){
			for (int i = 0; i < 3; ++i)
				for (int j = 0; j < 10; ++j)
					if (ui->comboText[i][j]->visible)
					{
						ui->comboText[i][j]->color.a -= 0.04f;
						ui->combo->color.a -= 0.04f;
					}
		};
	};
}