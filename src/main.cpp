#include <iostream>
#include "attributes.h"
#include "hs_board.h"

using namespace std;

void init_list(list<HSMinion> &l, unsigned long attack, int health, unsigned num)
{
	HSMinion m(attack, health);
	for (unsigned i = 0; i < num; i++)
		l.push_back(m);
}

int main()
{
	std::list<HSMinion> my;
	init_list(my, 4, 4, 2);
	std::list<HSMinion> enemy = my;

	my.begin()->skill |= attributes::Skill::Cleave;
	HSBoard b(my, enemy);

	cout << b.calc_odds() << endl;;

	return 0;
}
