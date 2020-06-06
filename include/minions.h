#pragma once
#include "attributes.h"

// TODO fill this enum with minions that can spawn from other minions
enum Minion { Bomb, Rat, Coiler, Ratling };

struct HSMinion {
	unsigned attack;
	int health;
	unsigned deathrattles{0};
	unsigned tribe{0};
	unsigned skill{0};
	HSMinion(unsigned long a, int h, unsigned d = 0, unsigned t = 0,
	         unsigned s = 0)
	  : attack(a), health(h), deathrattles(d), tribe(t), skill(s)
	{
	}
	HSMinion(Minion id);
	HSMinion() {}
};
