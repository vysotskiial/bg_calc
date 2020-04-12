#include "hs_board.h"

using namespace std;

double HSBoard::calc_odds() {
	stack<HSBoard*> states;
	HSBoard *state;
	double result = 0;

	states.push(this);
	while (!states.empty()) {
		state = states.top();
		states.pop();
		if (!state->add_next_states(states))
			if (state->won()) result += state->odds;
		delete state;
	}

	return result;
}

int HSBoard::add_next_states(stack<HSBoard*> &states)
{
	if (my_board.empty() || enemy_board.empty()) return 0;
	vector<HSMinion> &side_to_attack = my_turn ? enemy_board : my_board;

	int taunt_count = 0;
	//calculate number of taunts
	for (unsigned i = 0; i < side_to_attack.size(); i++)
		if (side_to_attack[i].skill & attributes::Taunt)
			taunt_count++;

	for (unsigned i = 0; i < side_to_attack.size(); i++)
		if (side_to_attack[i].skill & attributes::Taunt)
			states.push(process_attack(i, taunt_count));

	// if has no taunts everything is a potential target
	if (!taunt_count)
		for (unsigned i = 0; i < side_to_attack.size(); i++)
			states.push(process_attack(i, side_to_attack.size()));

	return 1;
}

HSBoard* HSBoard::process_attack(int target, int targ_num) {
	HSBoard *res = new HSBoard(*this);
	res->odds = odds / targ_num;
	res->my_turn = !my_turn;
	return res;
}
