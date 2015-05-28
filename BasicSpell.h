#ifndef __BASIC_SPELL__
#define __BASIC_SPELL__

#include <string.h>
#include <string>

class BasicSpell
{
	protected:
		std::string Name, Description;
		int Level, StepsInfluenced, StepsLeft;
		bool Active;

	public:
		BasicSpell();
		BasicSpell(const BasicSpell &val);
		BasicSpell(std::string name, int stepsInfl);

		void Activate();
		void Deactivate();

		std::string GetName() const;
		std::string GetDescription() const;
		int GetLevel() const;
		int GetStepsInfluenced() const;

		void SetName(std::string val);
		void SetDescription(std::string val);
		void SetLevel(int val);
		void SetStepsInfluenced(int val);

		virtual void Run() = 0;
};

/*class HealSpell : public BasicSpell
{
	private:
		Player* mPlayer;

	public:
		HealSpell()
		{
			Level = 0;
			TurnsInfluenced = 0;
			TurnsDone = 0;
			Active = false;
		}

		HealSpell(Player *player)
		{
			Name = "Heal";
			Description = "Возобновляє хопе";
			TurnsInfluenced = 0;
			TurnsDone = 0;
			Active = false;

			mPlayer = player;

			if (player->GetRace() == 'b')
				Level = 1; else
					Level = 0;
		}

		void Run()
		{
			if (mPlayer && mPlayer->GetHP() < mPlayer->GetMaxHP())
				mPlayer->SetHP(mPlayer->GetHP() + int(((Level + 1) * 0.2f) / mPlayer->GetMaxHP()));
		}
};

class TeleportSpell : public BasicSpell
{
	private:
		Player* mPlayer;

	public:
		TeleportSpell()
		{
			Level = 0;
			TurnsInfluenced = 1;
			TurnsDone = 0;
			Active = false;
		}

		TeleportSpell(Player *player)
		{
			Name = "Teleport";
			Description = "Тэлыпортуэ в хзкуды";
			TurnsInfluenced = 1;
			TurnsDone = 0;
			Active = false;

			mPlayer = player;

			if (player->GetRace() == 'b')
				Level = 1; else
					Level = 0;
		}

		void Run()
		{
			if (TurnsDone >= TurnsInfluenced)
			{
				Active = false;
			}

			if (!Active)
			{
				Active = true;
				TurnsDone = 0;

				if (mPlayer)
					mPlayer->SetWRadius(mPlayer->GetWRadius() * 1024);
			}
		}

		void Update()
		{

		}
};*/

#endif