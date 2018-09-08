#pragma once

enum BulletType
{
	PlayerBullet,
	PlayerHoming,
	PlayerWaterZet,
	Nuclear,
	S1_OctoBullet,
	S2_OctoBullet,
	S1_FishBullet,
	S2_FishBullet,
	S1_MidBullet,
	S1_FinalBullet,
	S1_FinalDropBullet,
	S2_MidBullet,
	S2_FinalSword,
	S2_FinalStorm
};

enum AnimationType
{
	Attack,
	AttackReady,
	Move,
	SideAttack,
	BulletDropAttack,
	LeftAttack,
	RightAttack,
	DropArmAttack,
	StormAttack,
	SwardAttack
};

enum ItemType
{
	WaterZet3Item,
	WaterZetItem,
	HomingWaterZetItem,
	AddOnItem,
	HpItem,
	NuclearItem,
	PowerItem,
	SpeedItem
};

enum EnemyType
{
	NORMAL,
	BOSS
};