#include "DXUT.h"
#include "Player.h"
#include "World.h"
#include "GameManager.h"
#include "Bullet.h"
#include "GameScene.h"

Player::Player() : currentSpeed(300), moveAni(5.5f), shootWaterZet(0), bubbleTimer(0), clearSpeed(-300), effTimer(0)
{
	hp = maxHp = 50;
	addChild(img = new Sprite("image/player/body/5.png"));
	addChild(ani = new Animation("image/player/wing", 9, 10, true));
	ani->pos -= Vec2(3, 0);
	rect = img->rect;
	speed = 300;

	shootTimer = pair<float, float>(0, 0.6f);
	waterZetTimer = pair<float, float>(0, 0.7f);
	waterZet3Timer = pair<float, float>(0, 3);
	homingWaterZetTimer = pair<float, float>(0, 2);

	for (int i = 0; i < 4; ++i)
	{
		addChild(addOn[i] = new AddOn());
		addOn[i]->angle = D3DXToRadian(360) / 4 * i;
		if (i >= gameManager.addOnCount)
			addOn[i]->visible = false;
		else 
			addOn[i]->canShoot = true;
	}

	left = Vec2(38, 80);
	right = Vec2(114, 80);
	top = Vec2(76, 30);
	bottom = Vec2(76, 134);

	rect.top += 32;
	rect.bottom -= 27;
	rect.left += 38;
	rect.right -= 47;
}

Player::~Player()
{
}

void Player::update(float dt)
{
	GameScene *scene = (GameScene*)world.currentScene;
	
	scene->ui->hpBar->visibleRect.right = scene->ui->hpBar->rect.right * hp / maxHp;

	if (hp <= 0 && visible)
	{
		scene->faidInList.push_back(scene->ui->continueScene);
		scene->faidInList.push_back(scene->ui->yes);
		scene->faidInList.push_back(scene->ui->no);
	}

	if (hp <= 0)
	{
		visible = false;
		hp = 0;
		return;
	}

	Unit::update(dt);

	bubbleTimer += dt;

	if (bubbleTimer >= 0.01f)
	{
		bubbleTimer = 0;
		Sprite *b = new Sprite("image/player/bubble.png");
		b->setCenter(Vec2(random(41, 122), 133) + ani->pos + pos);
		scene->addChild(b);
		scene->bubbleList.push_back(b);
	}

	if (scene->gameClearTimer >= 5)
	{
		pos.y -= dt * (clearSpeed += 15);
		return;
	}

	speed = currentSpeed;
	if (pos.x <= -130)
		pos.x = -130;
	if (pos.x >= 1280)
		pos.x = 1280;
	if (pos.y <= 0)
		pos.y = 0;
	if (pos.y > 640)
		pos.y = 640;

	homingWaterZetTimer.first += dt;
	waterZet3Timer.first += dt;
	waterZetTimer.first += dt;
	shootTimer.first += dt;

	int vx = world.getKeyState(VK_LEFT) == 2 ? -1 : (world.getKeyState(VK_RIGHT) == 2 ? 1 : 0);
	int vy = world.getKeyState(VK_UP) == 2 ? -1 : (world.getKeyState(VK_DOWN) == 2 ? 1 : 0);

	if (vx != 0 && vy != 0)
		speed /= sqrt(2);

	if (vx == 1)
	{
		moveAni += dt * 20;
		if (moveAni > 10)
			moveAni = 10;
	}
	else if (vx == -1)
	{
		moveAni -= dt * 20;
		if (moveAni < 0)
			moveAni = 0;
	}
	else
	{
		if (moveAni > 5.5f)
			moveAni -= dt * 20;
		else
			moveAni += dt * 20;
	}
	img->setPath("image/player/body/" + to_string((int)moveAni) + ".png");

	pos += Vec2(vx * speed * dt, vy * speed * dt);
	effTimer += dt;
	//치트으으으---------------------
	if (world.getKeyState(VK_F1) == 1)
	{
		if (gameManager.isGod)
			gameManager.isGod = false;
		else
			gameManager.isGod = true;
	}

	if (gameManager.isGod)
		hp = maxHp;

	if (world.getKeyState('Q') == 1)
	{
		gameManager.upgradeCount++;
	}
	if (world.getKeyState('W') == 1)
	{
		if (gameManager.addOnCount < 4)
		{
			addOn[gameManager.addOnCount]->visible = true;
			addOn[gameManager.addOnCount]->canShoot = true;
			gameManager.addOnCount++;
		}
	}
	if (world.getKeyState('1') == 1)
	{
		gameManager.waterZetCount++;
	}
	if (world.getKeyState('2') == 1)
	{
		gameManager.bombCount = 50;
	}
	if (world.getKeyState(VK_F2) == 1)
	{
		gameManager.waterZet3Count++;
	}
	if (world.getKeyState(VK_F3) == 1)
	{
		gameManager.homingWaterZetCount++;
	}

	// 충돌
	if (scene->map[0]->pos.y < pos.y && hp > 0)
	{
		if (scene->hitMap[0]->map[(int)(pos.x + 147 + left.x)][(int)(pos.y - scene->map[0]->pos.y + left.y)] ||
			scene->hitMap[0]->map[(int)(pos.x + 147 + right.x)][(int)(pos.y - scene->map[0]->pos.y + right.y)] ||
			scene->hitMap[0]->map[(int)(pos.x + 147 + top.x)][(int)(pos.y - scene->map[0]->pos.y + top.y)] ||
			scene->hitMap[0]->map[(int)(pos.x + 147 + bottom.x)][(int)(pos.y - scene->map[0]->pos.y + bottom.y)])
		{
			GetRed();
			scene->shake(0.01, 5, 8, false);
			hp -= 0.3f;
			gameManager.combo = 0;
			scene->ui->hit->color.a = 1 - hp / maxHp;
		}
	}
	else if (scene->hitMap[1]->map[(int)(pos.x + 147 + left.x)][(int)(pos.y - scene->map[1]->pos.y + left.y)] ||
		scene->hitMap[1]->map[(int)(pos.x + 147 + right.x)][(int)(pos.y - scene->map[1]->pos.y + right.y)] ||
		scene->hitMap[1]->map[(int)(pos.x + 147 + top.x)][(int)(pos.y - scene->map[1]->pos.y + top.y)] ||
		scene->hitMap[1]->map[(int)(pos.x + 147 + bottom.x)][(int)(pos.y - scene->map[1]->pos.y + bottom.y)])
	{
		if (hp > 0)
		{
			GetRed();
			scene->shake(0.01, 5, 8, false);
			hp -= 0.3f;
			gameManager.combo = 0;
			scene->ui->hit->color.a = 1 - hp / maxHp;
		}
	}
	// 끗

	//아이템
	for_iter(item, scene->itemList)
	{
		Rect t;
		if ((*item)->rectWithPos().intersects(rectWithPos(), &t))
		{
			if ((*item)->type == WaterZet3Item)
			{
				gameManager.waterZet3Count++;
				Sprite *b = new Sprite("image/UI/ingame/itemText/torpedo.png");
				b->setCenter(center());
				scene->addChild(b);
				scene->itemTextList.push_back(b);
			}
			else if ((*item)->type == WaterZetItem)
			{
				gameManager.waterZetCount++;
				Sprite *b = new Sprite("image/UI/ingame/itemText/torpedo.png");
				b->setCenter(center());
				scene->addChild(b);
				scene->itemTextList.push_back(b);
			}
			else if ((*item)->type == HomingWaterZetItem)
			{
				gameManager.homingWaterZetCount++;
				Sprite *b = new Sprite("image/UI/ingame/itemText/torpedo.png");
				b->setCenter(center());
				scene->addChild(b);
				scene->itemTextList.push_back(b);
			}
			else if ((*item)->type == AddOnItem)
			{
				if (gameManager.addOnCount < 4)
				{
					Sprite *b = new Sprite("image/UI/ingame/itemText/addon.png");
					b->setCenter(center());
					scene->addChild(b);
					scene->itemTextList.push_back(b);
					addOn[gameManager.addOnCount]->visible = true;
					addOn[gameManager.addOnCount]->canShoot = true;
					gameManager.addOnCount++;
				}
			}
			else if ((*item)->type == HpItem)
			{
				hp = maxHp;
				Sprite *b = new Sprite("image/UI/ingame/itemText/hp.png");
				b->setCenter(center());
				scene->addChild(b);
				scene->itemTextList.push_back(b);
			}
			else if ((*item)->type == NuclearItem)
			{
				gameManager.bombCount++;
			}
			else if ((*item)->type == PowerItem)
			{
				gameManager.upgradeCount++;
				Sprite *b = new Sprite("image/UI/ingame/itemText/power.png");
				b->setCenter(center());
				scene->addChild(b);
				scene->itemTextList.push_back(b);
			}
			else if ((*item)->type == SpeedItem)
			{
				Sprite *b = new Sprite("image/UI/ingame/itemText/speed.png");
				b->setCenter(center());
				scene->addChild(b);
				scene->itemTextList.push_back(b);
				currentSpeed = 700;
				speedUpTimer.reset(5, 1);
				speedUpTimer.onFinished = [=](){ currentSpeed = 300; };
				afterImageTimer.reset(0.1, 50);
				afterImageTimer.onTick = [=](){
					AfterImage *aImg = new AfterImage(new Sprite("image/player/body/" + to_string((int)moveAni) + ".png"));
					aImg->rotation = rotation;
					world.currentScene->addChild(aImg);
					aImg->setCenter(center());
					((GameScene*)world.currentScene)->afterImageList.push_back(aImg);
				};
			}

			removeChild(*item);
			item = scene->itemList.erase(item);
			if (item == scene->itemList.end())
				break;
		}
	}

	if (world.getKeyState('X') == 1)
	{
		if (gameManager.bombCount > 0)
		{
			gameManager.bombCount--;
			Bullet *b = new Bullet(Nuclear, 200, D3DXToRadian(-90), 3, 0);
			scene->addChild(b);
			b->setCenter(Vec2(76, 30) + pos);
			scene->bulletList.push_back(b);
			b->isPlayerBullet = true;
		}
	}

	if (world.getKeyState('Z') == 2)
	{
		if (shootTimer.first > shootTimer.second)
		{
			shootTimer.first = 0;
			if (gameManager.upgradeCount == 0)
			{
				Bullet *b = new Bullet(PlayerBullet, 50, D3DXToRadian(-90), 10, 0);
				scene->addChild(b);
				b->setCenter(Vec2(76, 30) + pos);
				scene->bulletList.push_back(b);
				b->isPlayerBullet = true;
			}
			else if (gameManager.upgradeCount == 1)
			{
				shootTimer.second = 0.3f;
				Bullet *b = new Bullet(PlayerBullet, 50, D3DXToRadian(-90), 10, 0);
				scene->addChild(b);
				b->setCenter(Vec2(76, 30) + pos);
				scene->bulletList.push_back(b);
				b->isPlayerBullet = true;
			}
			else if (gameManager.upgradeCount >= 2)
			{
				shootTimer.second = 0.3f;
				for (int i = 0; i < 3; ++i)
				{
					Bullet *b = new Bullet(PlayerBullet, 50, D3DXToRadian(-120 + 30 * i), 10, 0.01f - 0.01f * i);
					scene->addChild(b);
					b->setCenter(Vec2(76, 30) + pos);
					scene->bulletList.push_back(b);
					b->isPlayerBullet = true;
				}
			}
		}
		if (waterZetTimer.first > waterZetTimer.second)
		{
			waterZetTimer.first = 0;
			if (gameManager.waterZetCount == 1)
			{
				Bullet *b = new Bullet(PlayerWaterZet, -100, D3DXToRadian(-90), 15, 0);
				scene->addChild(b);
				b->setCenter(Vec2(39 + 75 * (shootWaterZet++ % 2), 41) + pos);
				scene->bulletList.push_back(b);
				b->isPlayerBullet = true;
			}
			else if (gameManager.waterZetCount == 2)
			{
				Bullet *b = new Bullet(PlayerWaterZet, -100, D3DXToRadian(-90), 15, 0);
				scene->addChild(b);
				b->setCenter(Vec2(39 + 75 * (shootWaterZet++ % 2), 41) + pos);
				scene->bulletList.push_back(b);
				b->isPlayerBullet = true;
			}
			else if (gameManager.waterZetCount >= 3)
			{
				waterZetTimer.second = 0.5f;
				Bullet *b = new Bullet(PlayerWaterZet, -100, D3DXToRadian(-90), 15, 0);
				scene->addChild(b);
				b->setCenter(Vec2(39 + 75 * (shootWaterZet++ % 2), 41) + pos);
				scene->bulletList.push_back(b);
				b->isPlayerBullet = true;
			}
		}
		if (waterZet3Timer.first > waterZet3Timer.second)
		{
			waterZet3Timer.first = 0;
			if (gameManager.waterZet3Count == 1)
			{
				for (int i = 0; i < 3; ++i)
				{
					Bullet *b = new Bullet(PlayerWaterZet, -200, D3DXToRadian(-120 + 30 * i), 15, 0);
					scene->addChild(b);
					b->setCenter(Vec2(76, 30) + pos);
					scene->bulletList.push_back(b);
					b->isPlayerBullet = true;
				}
			}
			else if (gameManager.waterZet3Count == 2)
			{
				for (int i = 0; i < 3; ++i)
				{
					Bullet *b = new Bullet(PlayerWaterZet, -200, D3DXToRadian(-120 + 30 * i), 15, 0);
					scene->addChild(b);
					b->setCenter(Vec2(76, 30) + pos);
					scene->bulletList.push_back(b);
					b->isPlayerBullet = true;
				}
			}
			else if (gameManager.waterZet3Count >= 3)
			{
				waterZet3Timer.second = 2.5f;

				for (int i = 0; i < 3; ++i)
				{
					Bullet *b = new Bullet(PlayerWaterZet, -200, D3DXToRadian(-120 + 30 * i), 15, 0);
					scene->addChild(b);
					b->setCenter(Vec2(76, 30) + pos);
					scene->bulletList.push_back(b);
					b->isPlayerBullet = true;
				}
			}
		}
		if (homingWaterZetTimer.first > homingWaterZetTimer.second && !scene->enemyList.empty())
		{
			homingWaterZetTimer.first = 0;
			if (gameManager.homingWaterZetCount == 1)
			{
				Bullet *b = new Bullet(PlayerHoming, -100, D3DXToRadian(-90), 15, 0, 10, (*scene->enemyList.begin()));
				scene->addChild(b);
				b->setCenter(Vec2(76, 30) + pos);
				scene->bulletList.push_back(b);
				b->isPlayerBullet = true;
			}
			else if (gameManager.homingWaterZetCount == 2)
			{
				Bullet *b = new Bullet(PlayerHoming, -100, D3DXToRadian(-90), 15, 0, 10, (*scene->enemyList.begin()));
				scene->addChild(b);
				b->setCenter(Vec2(76, 30) + pos);
				scene->bulletList.push_back(b);
				b->isPlayerBullet = true;
			}
			else if (gameManager.homingWaterZetCount >= 3)
			{
				homingWaterZetTimer.second = 1.5f;

				Bullet *b = new Bullet(PlayerHoming, -100, D3DXToRadian(-90), 15, 0, 10, (*scene->enemyList.begin()));
				scene->addChild(b);
				b->setCenter(Vec2(76, 30) + pos);
				scene->bulletList.push_back(b);
				b->isPlayerBullet = true;
			}
		}
	}
}

void Player::GetRed()
{
	img->color.b = 0.3f;
	img->color.g = 0.3f;

	redTimer.reset(0.01f, 25);
	redTimer.onTick = [=](){
		img->color.b += 0.028f;
		img->color.g += 0.028f;
	};
}

void Player::GetBlack()
{
	img->color.r = 0.3f;
	img->color.b = 0.3f;
	img->color.g = 0.3f;

	blackTimer.reset(0.01f, 25);
	blackTimer.onTick = [=](){
		img->color.r += 0.028f;
		img->color.b += 0.028f;
		img->color.g += 0.028f;
	};
}