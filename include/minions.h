#pragma once
#include "attributes.h"

//TODO fill this enum with minions that can spawn from other minions
enum Minions {
	Shredder,
	Rat
};

//Also we need to group minions by their parents
//for example deathrattle group for gascoiler would be something like
constexpr Minions deathrattle_minions[] = { Shredder, Rat };//TODO complete

struct HSMinion {
	unsigned long attack;
	int health;
	unsigned deathrattles;
	unsigned tribe;
	unsigned skill;
	HSMinion(unsigned long a, int h, unsigned d = 0, unsigned t = 0, unsigned s = 0)
		: attack(a), health(h), deathrattles(d), tribe(t), skill(s)
	{
	}
	HSMinion(int id);
};
