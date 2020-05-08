#pragma once

constexpr unsigned BombDamage = 4;

namespace attributes {

enum Deathrattle {
	ThreeMechs = 1,
	Bgurgle = 1 << 1,
	Shredder = 1 << 2,
	Sneeds = 1 << 3,
	Rat = 1 << 4,
	Bomb = 1 << 5,
	Wolf = 1 << 6,
	Divine = 1 << 7,
};

enum Tribe {
	Murloc = 1,
	Demon = 1 << 2,
	Beast = 1 << 3,
	Mech = 1 << 4,
};

enum Skill {
	Shield = 1,
	Taunt = 1 << 1,
	Cleave = 1 << 2,
};

} // namespace attributes
