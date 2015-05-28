#include "BasicSpell.h"

BasicSpell::BasicSpell()
{
	Active = false;
	StepsLeft = 0;
}

BasicSpell::BasicSpell(const BasicSpell &val)
{
	Name = val.Name;
	Description = val.Description;
	Active = val.Active;
	StepsLeft = val.StepsLeft;
	StepsInfluenced = val.StepsInfluenced;
}

BasicSpell::BasicSpell(std::string name, int stepsInfl)
{
	Name = name;
	StepsInfluenced = stepsInfl;
}

void BasicSpell::Activate()
{
	Active = true;
	StepsLeft = StepsInfluenced;
}

void BasicSpell::Deactivate()
{
	Active = false;
}

std::string BasicSpell::GetName() const
{
	return Name;
}

std::string BasicSpell::GetDescription() const
{
	return Description;
}

int BasicSpell::GetLevel() const
{
	return Level;
}

int BasicSpell::GetStepsInfluenced() const
{
	return StepsInfluenced;
}

void BasicSpell::SetName(std::string val)
{
	Name = val;
}

void BasicSpell::SetDescription(std::string val)
{
	Description = val;
}

void BasicSpell::SetLevel(int val)
{
	Level = val;
}

void BasicSpell::SetStepsInfluenced(int val)
{
	StepsInfluenced = val;
}
