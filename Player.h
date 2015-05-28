#include "BasicSpell.h"
#include "MD2Model.h"
#include "Utils.h"

#include <stdlib.h>

class Player
{
	protected:
		int Race, HP, MaxHP, MP, MaxMP;
		int XP, Attack, ARadius, WRadius;
		int X, Y, ID, Level, SkillPoints;

	public:
		MD2Model Model, Weapon;

	public:
		Player();
		Player(int race, int hp = 100, int maxHP = 100, int mp = 100, int maxMP = 100, int attack = 1, int aRadius = 1, int wRadius = 2, int level = 0, int xp = 0, int skillPoints = 0);

		Player(const Player& p);

		Player operator=(const Player& p);

		~Player();

		void SetHP(int val);
		void SetMP(int val);
		void SetXP(int val);
		void SetMaxHP(int val);
		void SetMaxMP(int val);
		void SetLevel(int val);
		void SetID(int val);
		void SetSkillPoints(int val);
		void SetAttack(int val);
		void SetARadius(int val);
		void SetWRadius(int val);
		void SetRace(int val);
		void SetCellPosition(int x, int y);
		void SetXCell(int val);
		void SetYCell(int val);
		void SetSpacePosition(float x, float y, float z);

		int GetHP() const;
		int GetMP() const;
		int GetXP() const;
		int GetMaxHP() const;
		int GetMaxMP() const;
		int GetLevel() const;
		int GetID() const;
		int GetSkillPoints() const;
		int GetAttack() const;
		int GetARadius() const;
		int GetWRadius() const;
		int GetRace() const;
		void GetCellPosition(int &x, int &y);
		void GetSpacePosition(float &x, float &y, float &z);

		int GetXCell() const;
		int GetYCell() const;

		void Draw(float time);
};