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
	int health;
	unsigned long attack;
	unsigned deathrattles;
	unsigned tribe;
	unsigned skill;
	HSMinion(): deathrattles(0), tribe(0), skill(0) {}
	HSMinion(int id);
};
