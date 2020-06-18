#include <gtest/gtest.h>
#include "hs_board.h"

void init_vec(BoardSide &v, unsigned long attack, int health, unsigned num)
{
	for (unsigned i = 0; i < num; i++)
		v.push_back({attack, health});
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
	ASSERT_FLOAT_EQ(b.calc_odds(), 1.);
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
	ASSERT_DOUBLE_EQ(b.calc_odds(), 0.8125);
}

// 2;2 2;2 2;2
//     vs
// 2;2 2;2
// rat bomb
TEST(test, rat_and_bomb)
{
	BoardSide enemy;
	init_vec(enemy, 2, 2, 3);
	BoardSide my;
	my.push_back(Rat);
	my.push_back(Bomb);

	HSBoard b(my, enemy);
	ASSERT_DOUBLE_EQ(b.calc_odds(), 0.75);
}

// 4;4   4;4  4;4 4;4
//          vs
// 4;4   4;4  4;4 4;4
// cleave
TEST(test, cleave)
{
	BoardSide my;
	init_vec(my, 4, 4, 4);
	BoardSide enemy = my;

	my[0].skill |= attributes::Skill::Cleave;
	HSBoard b(my, enemy);
	ASSERT_DOUBLE_EQ(0.875, b.calc_odds());
}

// Test is written when Coiler only has two options -- rat and bomb
// Coiler
//  7;7
//   vs
//  7;16
TEST(test, coiler)
{
	BoardSide my;
	my.push_back(Coiler);
	BoardSide enemy;
	enemy.push_back({7, 16});
	HSBoard b(enemy, my);
	ASSERT_DOUBLE_EQ(0.25, b.calc_odds());
}

// Goldrinn Rat
//   4;4    2;2
//       vs
//  2;2    2;1    5;199
// Nzoth Selfless
TEST(test, buff_deathrattles)
{
	BoardSide enemy;
	enemy.push_back(Goldrinn);
	enemy.push_back(Rat);
	BoardSide my;
	my.push_back(Nzoth);
	my.push_back(Selfless);
	my.push_back({5, 5});
	HSBoard b(my, enemy);
	ASSERT_DOUBLE_EQ(1. / 6, b.calc_odds());
}

TEST(test, windfury)
{
	BoardSide my;
	my.push_back(HSMinion(6, 7, attributes::Deathrattle::No, 0,
	                      attributes::Skill::Windfury));
	my.push_back({1, 1});
	my.push_back({1, 1});
	BoardSide enemy;
	enemy.push_back({100, 4});
	enemy.push_back({6, 6});
	HSBoard b(my, enemy);
	ASSERT_DOUBLE_EQ(b.calc_odds(), .5);
}

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
