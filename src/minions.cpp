#include "minions.h"

// TODO id from Minions enum in minions.h
HSMinion::HSMinion(Minion id)
{
	switch (id) {
	case Ratling:
		health = 1;
		attack = 1;
		tribe = attributes::Tribe::Beast;
		break;
	case Coiler:
		health = 7;
		attack = 7;
		tribe = attributes::Tribe::Beast;
		deathrattles = attributes::Deathrattle::Coiler;
		break;
	case Rat:
		health = 2;
		attack = 2;
		tribe = attributes::Tribe::Beast;
		deathrattles = attributes::Deathrattle::Rat;
		break;
	case Bomb:
		health = 2;
		attack = 2;
		tribe = attributes::Tribe::Mech;
		deathrattles = attributes::Deathrattle::Bomb;
		break;
	default:
		break;
	}
}
