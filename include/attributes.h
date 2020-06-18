#pragma once

constexpr unsigned BombDamage = 4;

namespace attributes {

enum class Deathrattle {
	No = 0,
	Rat,
	Bomb,
	Coiler,
	Nzoth,
	Selfless,
	Goldrinn,
};

enum Tribe {
	Murloc = 1,
	Demon = 1 << 2,
	Beast = 1 << 3,
	Mech = 1 << 4,
	All = -1u,
};

enum Skill {
	Shield = 1,
	Taunt = 1 << 1,
	Cleave = 1 << 2,
	Windfury = 1 << 3,
};

} // namespace attributes
