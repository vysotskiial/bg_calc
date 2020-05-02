#include <gtest/gtest.h>
#include "attributes.h"
#include "hs_board.h"

constexpr double eps = 0.00001;

void init_list(std::list<HSMinion> &l, unsigned long attack, int health, unsigned num)
{
	HSMinion m(attack, health);
	for (unsigned i = 0; i < num; i++)
		l.push_back(m);
}

// 1;2 1;2  1;2  1;2
//        vs
// 1;2 1;2  1;2 (1;2)
TEST(test, divine_shield)
{
	std::list<HSMinion> my;
	init_list(my, 1, 2, 4);
	std::list<HSMinion> enemy = my;

	(--my.end())->skill |= attributes::Shield;

	HSBoard b(my, enemy);

	ASSERT_TRUE(1 - b.calc_odds() < eps);
}

// 1;2  1;3
//    vs
// 3;2  1;2
TEST(test, basic_fight)
{
	std::list<HSMinion> my;
	init_list(my, 1, 2, 2);
	std::list<HSMinion> enemy = my;

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
	std::list<HSMinion> my;
	init_list(my, 2, 2, 2);
	std::list<HSMinion> enemy = my;

	enemy.push_back(HSMinion(2, 2));
	minion min = my.begin();
	min->deathrattles |= attributes::Deathrattle::Rat;
	min++;
	min->deathrattles |= attributes::Deathrattle::Bomb;

	HSBoard b(my, enemy);
	ASSERT_EQ(b.calc_odds(), 0.75);
}

TEST(test, cleave)
{
	std::list<HSMinion> my;
	init_list(my, 4, 4, 4);
	std::list<HSMinion> enemy = my;

	my.begin()->skill |= attributes::Skill::Cleave;
	HSBoard b(my, enemy);
	ASSERT_TRUE(0.875 - b.calc_odds() < eps);
}

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
