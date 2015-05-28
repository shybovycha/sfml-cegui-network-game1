#include "MD2Model.h"
#include "Utils.h"

#include <stdlib.h>

class Player
{
	protected:
		int Race, HP, MaxHP, MP, MaxMP;
		int XP, Attack, ARadius, WRadius;
		int X, Y, ID, Level;
		
		MD2Model Model, Weapon;

	public:
		Player();
		Player(int race, int hp = 100, int maxHP = 100, int mp = 100, int maxMP = 100, int attack = 1, int aRadius = 1, int wRadius = 2, int level = 0, int xp = 0);
		~Player();

		void SetHP(int val);
		void SetMP(int val);
		void SetXP(int val);
		void SetMaxHP(int val);
		void SetMaxMP(int val);
		void SetLevel(int val);
		void SetID(int val);
		void SetAttack(int val);
		void SetARadius(int val);
		void SetWRadius(int val);
		void SetRace(int val);
		void SetCellPosition(int x, int y);
		void SetXCell(int val);
		void SetYCell(int val);
		void SetSpacePosition(float x, float y, float z);

		int GetHP();
		int GetMP();
		int GetXP();
		int GetMaxHP();
		int GetMaxMP();
		int GetLevel();
		int GetID();
		int GetAttack();
		int GetARadius();
		int GetWRadius();
		int GetRace();
		void GetCellPosition(int &x, int &y);
		void GetSpacePosition(float &x, float &y, float &z);

		int GetXCell();
		int GetYCell();

		MD2Model& GetModel();
		MD2Model& GetWeapon();

		void Draw(float time);
};