#include <gtest/gtest.h>
#include "hs_board.h"

constexpr double eps = 0.00001;

// 1;2 1;2  1;2  1;2
//        vs
// 1;2 1;2  1;2 (1;2)
TEST(test, divine_shield)
{
	HSMinion m;
	std::list<HSMinion> my;
	std::list<HSMinion> enemy;

	m.attack = 1;
	m.health = 2;
	m.skill = 0;

	for (int i = 0; i < 4; i++) {
		my.push_back(m);
		enemy.push_back(m);
	}

	(--my.end())->skill |= attributes::Shield;

	HSBoard b(my, enemy);

	ASSERT_TRUE(1 - b.calc_odds() < eps);
}

// 1;2  1;3
//    vs
// 3;2  1;2
TEST(test, basic_fight)
{
	HSMinion m;

	std::list<HSMinion> my, enemy;

	m.attack = 1;
	m.health = 2;
	m.skill = 0;

	for (int i = 0; i < 2; i++) {
		my.push_back(m);
		enemy.push_back(m);
	}

	my.begin()->attack = 3;
	(++enemy.begin())->health = 3;
	HSBoard b(my, enemy);
	ASSERT_EQ(b.calc_odds(), 0.8125);
}


// 2;2 2;2 2;2
//     vs
// 2;2 2;2
// rat bomb
TEST(test, rat_and_bomb)
{
	HSMinion m;

	std::list<HSMinion> side;

	m.attack = 2;
	m.health = 2;
	m.skill = 0;

	for (int i = 0; i < 2; i++) {
		side.push_back(m);
	}

	std::list<HSMinion> other_side = side;

	other_side.push_back(m);
	minion min = side.begin();
	min->deathrattles |= attributes::Deathrattle::Rat;
	min++;
	min->deathrattles |= attributes::Deathrattle::Bomb;

	HSBoard b(side, other_side);
	ASSERT_EQ(b.calc_odds(), 0.75);
}

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
