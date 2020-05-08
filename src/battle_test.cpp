#include <gtest/gtest.h>
#include "hs_board.h"

constexpr double eps = 0.00001;

void init_vec(BoardSide &v, unsigned long attack, int health, unsigned num)
{
	HSMinion m(attack, health);
	for (unsigned i = 0; i < num; i++)
		v.push_back(m);
}

// 1;2 1;2  1;2  1;2
//        vs
// 1;2 1;2  1;2 (1;2)
TEST(test, divine_shield)
{
	BoardSide my;
	init_vec(my, 1, 2, 4);
	BoardSide enemy = my;

	my[3].skill |= attributes::Shield;

	HSBoard b(my, enemy);
	ASSERT_TRUE(1 - b.calc_odds() < eps) << "odds are " << b.calc_odds();
}

// 1;2  1;3
//    vs
// 3;2  1;2
TEST(test, basic_fight)
{
	BoardSide my;
	init_vec(my, 1, 2, 2);
	BoardSide enemy = my;

	my[0].attack = 3;
	enemy[1].health = 3;
	HSBoard b(my, enemy);
	ASSERT_EQ(b.calc_odds(), 0.8125);
}

// 2;2 2;2 2;2
//     vs
// 2;2 2;2
// rat bomb
TEST(test, rat_and_bomb)
{
	BoardSide my;
	init_vec(my, 2, 2, 2);
	BoardSide enemy = my;

	enemy.push_back(HSMinion(2, 2));
	my[0].deathrattles |= attributes::Deathrattle::Rat;
	my[1].deathrattles |= attributes::Deathrattle::Bomb;

	HSBoard b(my, enemy);
	ASSERT_EQ(b.calc_odds(), 0.75);
}

TEST(test, cleave)
{
	BoardSide my;
	init_vec(my, 4, 4, 4);
	BoardSide enemy = my;

	my[0].skill |= attributes::Skill::Cleave;
	HSBoard b(my, enemy);
	ASSERT_TRUE(0.875 - b.calc_odds() < eps);
}

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
