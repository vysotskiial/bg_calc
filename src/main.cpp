#include <iostream>
#include "attributes.h"
#include "hs_board.h"

using namespace std;

void init_vec(BoardSide &v, unsigned long attack, int health, unsigned num)
{
	HSMinion m(attack, health);
	for (unsigned i = 0; i < num; i++)
		v.push_back(m);
}

int main()
{
	BoardSide my;
	init_vec(my, 1, 2, 7);

	BoardSide enemy = my;

	my[4].skill |= attributes::Skill::Taunt;
	HSBoard b(my, enemy);

	cout << b.calc_odds() << endl;;

	return 0;
}
