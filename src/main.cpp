#include <iostream>
#include <ctime>
#include <chrono>
#include "hs_board.h"

using namespace std;
using Sec = chrono::duration<double>;

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
	auto wall_start = chrono::system_clock::now();
	auto cpu_start = double(clock());
	cout << b.calc_odds() << endl;
	auto cpu_end = double(clock());
	auto wall_end = chrono::system_clock::now();

	auto cpu_time = (cpu_end - cpu_start) / CLOCKS_PER_SEC;
	auto wall_time = chrono::duration_cast<Sec>(wall_end - wall_start).count();
	cout << "CPU time used: " << cpu_time << " s" << endl;
	cout << "Wall time passed: " << wall_time << " s" << endl;
	return 0;
}
