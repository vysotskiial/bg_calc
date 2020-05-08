#include <iostream>
#include <ctime>
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

	// my[4].skill |= attributes::Skill::Taunt;

	HSBoard b(my, enemy);

	clock_t c_start = clock();
	cout << b.calc_odds() << endl;
	;
	clock_t c_end = std::clock();

	long double time_elapsed_ms = 1000.0 * (c_end - c_start) / CLOCKS_PER_SEC;
	std::cout << "CPU time used: " << time_elapsed_ms << " ms\n";
	return 0;
}
