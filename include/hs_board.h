#pragma once
#include <vector>
#include <stack>
#include "minions.h"


//TODO something about same boardstates (most variants come from token spawn)
//and maybe always win/lose calculation for optimisation
class HSBoard {
private:
	double odds;//odds of getting to current state
	bool my_turn;
	int my_attacker;//which minion in vector attacks
	int enemy_attacker;
	std::vector<HSMinion> my_board;
	std::vector<HSMinion> enemy_board;
	int add_next_states(std::stack<HSBoard*> &states);
	HSBoard *process_attack(int target, int targ_num);
	bool won() { return !my_board.empty() && enemy_board.empty(); }
public:
	double calc_odds();//Returns the odds of you winning the round
};
