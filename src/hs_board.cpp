#include <iostream>
#include <algorithm>
#include "attributes.h"
#include "hs_board.h"
//TODO fuck magic constants
using namespace std;

double HSBoard::calc_odds() {
	HSBoard *state;
	states = new stack<HSBoard *>;
	double result = 0;
	my_turn = true;

	if (sides[attack]->size() == sides[target]->size()) odds = 0.5;
	else odds = 1;

	if (sides[attack]->size() >= sides[target]->size()) {
		states->push(new HSBoard(*this));
	}

	if (sides[target]->size() >= sides[attack]->size()) {
		swap_sides();
		states->push(new HSBoard(*this));
	}

	while (!states->empty()) {
		state = states->top();
		//state->print();
		states->pop();
		if (!state->add_next_states()) {
			if (state->won()) {
				result += state->odds;
			}
		}
		delete state;
	}
	delete states;

	return result;
}

void HSBoard::swap_sides()
{
	my_turn = !my_turn;
	void *buf = sides[attack];
	sides[attack] = sides[target];
	sides[target] = (BoardSide *)buf;
}

void HSBoard::print()
{
	//print enemy at the top
	std::list<HSMinion> &my = (my_turn) ? sides[attack]->board : sides[target]->board;
	std::list<HSMinion> &enemy = (my_turn) ? sides[target]->board : sides[attack]->board;
	cout << "-----" << endl;
	for (auto i : enemy)
		cout << i.attack << ";" << i.health << " ";
	cout << endl;
	for (auto i : my)
		cout << i.attack << ";" << i.health << " ";
	cout << endl;
	cout << "-----" << endl;
}

int HSBoard::add_next_states()
{
	if (sides[attack]->empty() || sides[target]->empty()) return 0;

	int taunt_count = 0;
	//calculate number of taunts
	for (auto i : sides[target]->board)
		if (i.skill & attributes::Taunt)
			taunt_count++;

	int target_num = (taunt_count) ? taunt_count : sides[target]->size();
	odds /= target_num;

	unsigned i = 0;
	for (auto it = sides[target]->board.begin(); it != sides[target]->board.end(); i++, it++)
		if ((it->skill & attributes::Taunt) || !taunt_count) {
			HSBoard *new_state = new HSBoard(*this);
			minion t = new_state->sides[target]->board.begin();
			advance(t, i);
			new_state->process_attack(t);
		}
	return 1;
}

void HSBoard::process_attack(minion t) {
	int damage = sides[attack]->attacker->attack;
	bool cleave = sides[attack]->attacker->skill & attributes::Cleave;

	sides[attack]->take_damage(t->attack, sides[attack]->attacker);
	sides[target]->take_damage(damage, t);

	if (cleave) {
		t++;
		if (t != sides[target]->board.end()) {
			sides[target]->take_damage(damage, t);
		}
		t--;
		if (t != sides[target]->board.begin())
			sides[target]->take_damage(damage, --t);
	}

	process_deathrattles();
	return;
}

void HSBoard::process_deathrattles()
{
	while (trigger_deathrattle(attack) || trigger_deathrattle(target));

	if (!sides[attack]->attacker_died) sides[attack]->next_attacker();
	swap_sides();
	states->push(this);
}

minion BoardSide::pop_dead()
{
	minion result = board.begin();
	for (; result != board.end(); result++)
		if (result->health <= 0)
			return result;

	return result;
}

bool HSBoard::trigger_deathrattle(Side side)
{
	minion dead = sides[side]->pop_dead();
	if (sides[side]->pop_dead() == sides[side]->board.end()) return false;

	Side other_side = (side == attack) ? target : attack;
	switch(dead->deathrattles) {
		case attributes::Deathrattle::Bomb:
			// Divide the odds TODO
			if (sides[side]->attacker == dead) {
				sides[side]->attacker_died = true;
				sides[side]->next_attacker();
			}
			sides[side]->board.erase(dead);
			trigger_bomb(other_side);
			return true;
		case attributes::Deathrattle::Rat:
			sides[side]->trigger_rat(dead);
			break;
		default:
			break;
	}

	if (sides[side]->attacker == dead) {
		sides[side]->next_attacker();
		sides[side]->attacker_died = true;
	}
	sides[side]->board.erase(dead);
	return true;
}

void HSBoard::trigger_bomb(Side side) {
	bool first = true;
	unsigned variant_num = 0;
	for (auto m : sides[side]->board) if (m.health > 0) variant_num++;
	odds /= variant_num;
	HSBoard copy(*this);

	unsigned distance = 0;
	for (auto it = sides[side]->board.begin(); it != sides[side]->board.end(); it++, distance++) {
		//Dead minion cannot be target
		if (it->health <= 0) continue;
		//First target is handled by current instance of HSBoard
		if (first) {
			sides[side]->take_damage(4, it);
			first = false;
			continue;
		}
		//For every other target create new instance of HSBoard
		HSBoard *b = new HSBoard(copy);
		minion t = b->sides[side]->board.begin();
		advance(t, distance);
		b->sides[side]->take_damage(4, t);
		b->process_deathrattles();
	}
}

void BoardSide::trigger_rat(minion dead)
{
	HSMinion ratling(1, 1);
	minion next = ++dead;
	dead--;
	board.insert(next, min(8 - board.size(), dead->attack), ratling);
}

void BoardSide::take_damage(unsigned damage, minion target)
{
	if (target->skill & attributes::Shield) {
		target->skill &= ~attributes::Shield;
		return;
	}
	target->health -= damage;

	return;
}

void BoardSide::next_attacker()
{
	if (++attacker == board.end())
		attacker = board.begin();
}
