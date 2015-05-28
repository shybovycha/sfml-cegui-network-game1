#include "Player.h"

Player::Player()
{
	Race = 'a';
	HP = 100;
	MaxHP = 100;
	MP = 100;
	MaxMP = 100;
	Attack = 1;
	ARadius = 1;
	WRadius = 2;
	Level = 0;
	XP = 0;
	ID = -1;
	SkillPoints = 0;
}

Player::Player(const Player& p)
{
	//Race = 'a';
	HP = p.HP;
	MaxHP = p.MaxHP;
	MP = p.MP;
	MaxMP = p.MaxMP;
	Attack = p.Attack;
	ARadius = p.ARadius;
	WRadius = p.WRadius;
	Level = p.Level;
	XP = p.XP;
	ID = p.ID;
	SkillPoints = p.SkillPoints;

	SetRace(p.Race);
}

Player::Player(int race, int hp, int maxHP, int mp, int maxMP, int attack, int aRadius, int wRadius, int level, int xp, int skillPoints)
{
	HP = hp;
	MaxHP = maxHP;
	MP = mp;
	MaxMP = maxMP;
	Attack = attack;
	ARadius = aRadius;
	WRadius = wRadius;
	Level = level;
	XP = xp;
	ID = -1;
	SkillPoints = skillPoints;

	SetRace(race);
}

Player Player::operator=(const Player& p)
{
	HP = p.HP;
	MaxHP = p.MaxHP;
	MP = p.MP;
	MaxMP = p.MaxMP;
	Attack = p.Attack;
	ARadius = p.ARadius;
	WRadius = p.WRadius;
	Level = p.Level;
	XP = p.XP;
	SkillPoints = p.SkillPoints;

	SetRace(p.Race);

	return *this;
}

Player::~Player()
{
}

void Player::SetHP(int val)
{
	HP = val;
}

void Player::SetMP(int val)
{
	MP = val;
}

void Player::SetXP(int val)
{
	XP = val;
}

void Player::SetMaxHP(int val)
{
	MaxHP = val;
}

void Player::SetMaxMP(int val)
{
	MaxMP = val;
}

void Player::SetLevel(int val)
{
	Level = val;
}

void Player::SetID(int val)
{
	ID = val;
}

void Player::SetSkillPoints(int val)
{
	SkillPoints = val;
}

void Player::SetAttack(int val)
{
	Attack = val;
}

void Player::SetARadius(int val)
{
	ARadius = val;
}

void Player::SetWRadius(int val)
{
	WRadius = val;
}

void Player::SetRace(int val)
{
	Race = val;

	if (Race == 'a')
	{
		// load models, set params for humans
		Model.Load("pknight.md2");
		Model.SetTexture("pknight.jpg");

		Weapon.Load("pknight_weapon.md2");
		Weapon.SetTexture("pknight_weapon.jpg");

		Model.SetAnimation(MD2ANIM_STAND);
		Weapon.SetAnimation(MD2ANIM_STAND);

		ARadius = 2;
		WRadius = 4;
		Attack = 20;

		XP = 0;
		Level = 0;
		HP = 100;
		MP = 0;
		MaxHP = 100;
		MaxMP = 0;
	} else
	if (Race == 'b')
	{
		// load models, set params for not-fully-humans
		Model.Load("forgottenone2.md2");
		Model.SetTexture("forgottenone.jpg");

		Weapon.Load("forgottenone_weapon.md2");
		Weapon.SetTexture("forgottenone_weapon.jpg");

		Model.SetAnimation(MD2ANIM_STAND);
		Weapon.SetAnimation(MD2ANIM_STAND);

		ARadius = 2;
		WRadius = 4;
		Attack = 20;

		XP = 0;
		Level = 0;
		HP = 100;
		MP = 0;
		MaxHP = 100;
		MaxMP = 0;
	} else
	if (Race == 'c')
	{
		// load models, set params for un-humans
		Model.Load("wolftris.md2");
		Model.SetTexture("wolfskin.jpg");

		Model.SetAnimation(MD2ANIM_STAND);
		Weapon.SetAnimation(MD2ANIM_STAND);

		ARadius = 1;
		WRadius = 2;
		Attack = 30;

		XP = 0;
		Level = 0;
		HP = 100;
		MP = 0;
		MaxHP = 100;
		MaxMP = 0;
	}
}

void Player::SetCellPosition(int x, int y)
{
	X = x;
	Y = y;
}

void Player::SetXCell(int val)
{
	X = val;
}

void Player::SetYCell(int val)
{
	Y = val;
}

void Player::SetSpacePosition(float x, float y, float z)
{
	Model.SetPosition(x, y, z);
	Weapon.SetPosition(Model.GetPosition());
	Weapon.SetRotation(Model.GetRotation());
}

int Player::GetHP() const
{
	return HP;
}

int Player::GetMP() const
{
	return MP;
}

int Player::GetXP() const
{
	return XP;
}

int Player::GetMaxHP() const
{
	return MaxHP;
}

int Player::GetMaxMP() const
{
	return MaxHP;
}

int Player::GetLevel() const
{
	return Level;
}

int Player::GetID() const
{
	return ID;
}

int Player::GetSkillPoints() const
{
	return SkillPoints;
}

int Player::GetAttack() const
{
	return Attack;
}

int Player::GetARadius() const
{
	return ARadius;
}

int Player::GetWRadius() const
{
	return WRadius;
}

int Player::GetRace() const
{
	return Race;
}

void Player::GetCellPosition(int &x, int &y)
{
	x = X;
	y = Y;
}

int Player::GetXCell() const
{
	return X;
}

int Player::GetYCell() const
{
	return Y;
}

void Player::Draw(float time)
{
	if (Weapon.IsLoaded())
	{
		Weapon.SetPosition(Model.GetPosition());
		Weapon.SetRotation(Model.GetRotation());

		if (Weapon.GetAnimation() != Model.GetAnimation() || Weapon.GetAnimationLooped() != Model.GetAnimationLooped())
			Weapon.SetAnimation(Model.GetAnimation(), Model.GetAnimationLooped());

		Weapon.Draw(time);
	}

	Model.Draw(time);
}

void Player::GetSpacePosition(float &x, float &y, float &z)
{
	x = Model.GetPosition().GetX();
	y = Model.GetPosition().GetY();
	z = Model.GetPosition().GetZ();
}
