#include <gtest/gtest.h>
#include "hs_board.h"

constexpr double eps = 0.00001;

TEST(test, test_always_win)
{
	HSBoard b;
	HSMinion m;

	m.attack = 1;
	m.health = 2;
	m.skill = 0;

	for (int i = 0; i < 3; i++) {
		b.my_board.push_back(m);
		b.enemy_board.push_back(m);
	}

	b.my_board[2].health = 3;

	ASSERT_TRUE(1 - b.calc_odds() < eps);
}

TEST(test, test_75_percent)
{
	HSBoard b;
	HSMinion m;

	m.attack = 1;
	m.health = 2;
	m.skill = 0;

	for (int i = 0; i < 2; i++) {
		b.my_board.push_back(m);
		b.enemy_board.push_back(m);
	}

	b.my_board[0].attack = 3;
	b.enemy_board[1].health = 3;
	ASSERT_EQ(b.calc_odds(), 0.75);
}

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
