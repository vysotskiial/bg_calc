#include <iostream>
#include "hs_board.h"

using namespace std;

double HSBoard::calc_odds() {
	stack<HSBoard*> states;
	HSBoard *state;
	double result = 0;

	my_turn = 1;
	current_attacker = 0;
	next_attacker = 0;
	odds = 1;
	states.push(new HSBoard(*this));
	while (!states.empty()) {
		state = states.top();
		states.pop();
		if (!state->add_next_states(states)) {
			if (state->won()) result += state->odds;
		}
		delete state;
	}

	return result;
}

void HSBoard::print()
{
	cout << "-----" << endl;
	for (unsigned i = 0; i < enemy_board.size(); i++)
		cout << enemy_board[i].attack << ";" << enemy_board[i].health << " ";
	cout << endl;
	for (unsigned i = 0; i < my_board.size(); i++)
		cout << my_board[i].attack << ";" << my_board[i].health << " ";
	cout << endl;
}

int HSBoard::add_next_states(stack<HSBoard*> &states)
{
	//print();
	if (my_board.empty() || enemy_board.empty()) return 0;
	vector<HSMinion> &target_side = my_turn ? enemy_board : my_board;

	int taunt_count = 0;
	//calculate number of taunts
	for (unsigned i = 0; i < target_side.size(); i++)
		if (target_side[i].skill & attributes::Taunt)
			taunt_count++;

	int target_num = (taunt_count) ? taunt_count : target_side.size();
	odds /= target_num;

	for (unsigned i = 0; i < target_side.size(); i++)
		if (target_side[i].skill & attributes::Taunt)
			states.push(process_attack(i));

	// if has no taunts everything is a potential target
	if (!taunt_count)
		for (unsigned i = 0; i < target_side.size(); i++)
			states.push(process_attack(i));

	return 1;
}

HSBoard* HSBoard::process_attack(int target_idx) {
	HSBoard *res = new HSBoard(*this);

	vector<HSMinion> &attacking_side = my_turn ? res->my_board : res->enemy_board;
	vector<HSMinion> &target_side = my_turn ? res->enemy_board : res->my_board;

	HSMinion &attacker = attacking_side[current_attacker];
	HSMinion &target = target_side[target_idx];

	attacker.health -= target.attack;
	target.health -= attacker.attack;
	if (attacker.health <= 0) attacking_side.erase(attacking_side.begin() + current_attacker);
	else res->current_attacker++;

	if (res->current_attacker >= attacking_side.size()) res->current_attacker = 0;

	if (target.health <= 0) {
		target_side.erase(target_side.begin() + target_idx);
		if (next_attacker > target_idx)
			res->next_attacker--;
		else if (next_attacker == target_idx) {
			if (next_attacker >= target_side.size()) res->next_attacker = 0;
		}
	}

	int buf = res->next_attacker;
	res->next_attacker = res->current_attacker;
	res->current_attacker = buf;

	res->my_turn = !res->my_turn;

	return res;
}
