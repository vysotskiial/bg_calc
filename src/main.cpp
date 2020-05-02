#include <iostream>
#include "attributes.h"
#include "hs_board.h"

using namespace std;

int main()
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

	HSBoard b(other_side, side);

	cout << b.calc_odds() << endl;;

	return 0;
}
