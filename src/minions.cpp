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
	case Nzoth:
		health = 2;
		attack = 2;
		deathrattles = attributes::Deathrattle::Nzoth;
		break;
	case Selfless:
		health = 1;
		attack = 2;
		deathrattles = attributes::Deathrattle::Selfless;
		break;
	case Goldrinn:
		health = 4;
		attack = 4;
		tribe = attributes::Tribe::Beast;
		deathrattles = attributes::Deathrattle::Goldrinn;
		break;
	}
}

std::ostream &operator<<(std::ostream &ostr, const HSMinion &m)
{
	if (m.skill & attributes::Skill::Shield)
		ostr << '(';
	if (m.skill & attributes::Skill::Taunt)
		ostr << '[';
	if (m.skill & attributes::Skill::Cleave)
		ostr << "<-";
	ostr << m.attack << ';' << m.health;
	if (m.skill & attributes::Skill::Cleave)
		ostr << "->";
	if (m.skill & attributes::Skill::Taunt)
		ostr << ']';
	if (m.skill & attributes::Skill::Shield)
		ostr << ')';
	return ostr;
}
