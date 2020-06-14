#pragma once
#include <ostream>
#include "attributes.h"

// TODO fill this enum with minions that can spawn from other minions
enum Minion { Bomb, Rat, Coiler, Ratling, Nzoth, Selfless, Goldrinn };

struct HSMinion {
	unsigned attack;
	int health;
	attributes::Deathrattle deathrattles{attributes::Deathrattle::No};
	unsigned tribe{0};
	unsigned skill{0};
	HSMinion(unsigned long a, int h,
	         attributes::Deathrattle d = attributes::Deathrattle::No,
	         unsigned t = 0, unsigned s = 0)
	  : attack(a), health(h), deathrattles(d), tribe(t), skill(s)
	{
	}
	HSMinion(Minion id);
	HSMinion() = default;
};

std::ostream &operator<<(std::ostream &ostr, const HSMinion &m);
