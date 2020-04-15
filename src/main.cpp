#include <iostream>
#include "hs_board.h"

using namespace std;

int main()
{
	HSBoard b;
	HSMinion m;

	m.attack = 1;
	m.health = 1;
	m.skill = 0;

	for (int i = 0; i < 2; i++) {
		b.my_board.push_back(m);
		b.enemy_board.push_back(m);
	}

	b.my_board[0].attack = 3;
	b.my_board[1].health = 2;
	b.enemy_board[1].health = 3;

/*
	b.my_board[6].skill |= attributes::Taunt;
	b.my_board[5].skill |= attributes::Taunt;
	b.my_board[1].skill |= attributes::Taunt;
	b.my_board[3].skill |= attributes::Taunt;
*/

/*
	b.enemy_board[6].skill |= attributes::Taunt;
	b.enemy_board[5].skill |= attributes::Taunt;
	b.enemy_board[3].skill |= attributes::Taunt;
	b.enemy_board[1].skill |= attributes::Taunt;
*/
	cout << b.calc_odds() << endl;;

	return 0;
}
