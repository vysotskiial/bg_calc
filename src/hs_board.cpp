#include <iostream>
#include <algorithm>
#include <future>
#include "attributes.h"
#include "hs_board.h"

using namespace std;

// Also we need to group minions by their parents
// for example deathrattle group for gascoiler would be something like
constexpr std::array from_coiler = {Bomb, Rat}; // TODO complete

double HSBoard::calc_odds()
{
	my_turn = true;
	future<double> first_half;

	if (attack_side->size() == target_side->size()) {
		odds = 0.5;
		HSBoard copy(*this);
		first_half = async(launch::async, &HSBoard::calc_half, &copy);
	}
	else {
		odds = 1;
	}

	if (attack_side->size() <= target_side->size())
		swap_sides();

	double second_half = calc_half();
	return (odds == 1) ? second_half : second_half + first_half.get();
}

double HSBoard::calc_half()
{
	states = new std::vector<HSBoard>;
	states->reserve(40);
	double result = 0;

	states->push_back(*this);

	while (!states->empty()) {
		HSBoard state(states->back());
		states->pop_back();
		// state.print();
		if (!state.add_next_states()) {
			if (state.won()) {
				result += state.odds;
			}
		}
	}
	delete states;

	return result;
}

void HSBoard::swap_sides()
{
	my_turn = !my_turn;
	swap(attack_side, target_side);
}

void HSBoard::print()
{
	// print enemy at the top
	cout << "-----" << endl;
	for (unsigned i = 0; i < enemy_side.size(); i++)
		cout << enemy_side[i].attack << ";" << enemy_side[i].health << " ";
	cout << endl;
	for (unsigned i = 0; i < my_side.size(); i++)
		cout << my_side[i].attack << ";" << my_side[i].health << " ";
	cout << endl;
	cout << "-----" << endl;
}

int HSBoard::add_next_states()
{
	if (target_side->empty() || attack_side->empty())
		return 0;

	if (attack_side->increase_attacker) {
		attack_side->attacker++;
		if (attack_side->attacker >= attack_side->size()) {
			attack_side->attacker = 0;
		}
	}

	attack_side->increase_attacker = true;

	int taunt_count = 0;
	// calculate number of taunts
	for (unsigned i = 0; i < target_side->size(); i++)
		if ((*target_side)[i].skill & attributes::Taunt)
			taunt_count++;

	int target_num = (taunt_count) ? taunt_count : target_side->size();
	odds /= target_num;

	for (unsigned i = 0; i < target_side->size(); i++) {
		if (((*target_side)[i].skill & attributes::Taunt) || !taunt_count) {
			HSBoard new_state(*this);
			new_state.process_attack(i);
		}
	}
	return 1;
}

void HSBoard::process_attack(unsigned t)
{
	int damage = (*attack_side)[attack_side->attacker].attack;
	bool cleave =
	  (*attack_side)[attack_side->attacker].skill & attributes::Cleave;

	attack_side->take_damage((*target_side)[t].attack, attack_side->attacker);
	target_side->take_damage(damage, t);

	if (cleave) {
		if (t != target_side->size() - 1) {
			target_side->take_damage(damage, t + 1);
		}
		if (t != 0)
			target_side->take_damage(damage, t - 1);
	}

	process_deathrattles();
	return;
}

void HSBoard::process_deathrattles()
{
	while (trigger_deathrattle(my_turn) || trigger_deathrattle(!my_turn)) {}
	swap_sides();
	states->push_back(*this);
}

bool HSBoard::trigger_deathrattle(bool my)
{
	BoardSide &side = my ? my_side : enemy_side;
	unsigned dead = 0;
	for (; dead < side.size(); dead++)
		if (side[dead].health <= 0)
			break;

	if (dead == side.size())
		return false;
	HSMinion m_dead = side[dead];
	side.erase(dead);

	switch (m_dead.deathrattles) {
	case attributes::Deathrattle::Bomb:
		trigger_bomb(!my);
		break;
	case attributes::Deathrattle::Rat:
		side.trigger_summon(dead, Ratling, m_dead.attack);
		break;
	case attributes::Deathrattle::Coiler:
		trigger_summon_optional(my, from_coiler, 2, dead);
		break;
	default:
		break;
	}

	return true;
}

void HSBoard::trigger_bomb(bool my)
{
	BoardSide &side = my ? my_side : enemy_side;
	bool first = true;
	unsigned variant_num = 0;
	for (unsigned i = 0; i < side.size(); i++)
		if (side[i].health > 0)
			variant_num++;
	if (!variant_num)
		return;
	odds /= variant_num;
	HSBoard copy(*this);

	for (unsigned i = 0; i < side.size(); i++) {
		// Dead minion cannot be target
		if (side[i].health <= 0)
			continue;
		// First target is handled by current instance of HSBoard
		if (first) {
			side.take_damage(BombDamage, i);
			first = false;
			continue;
		}
		// For every other target create new instance of HSBoard
		HSBoard b(copy);
		BoardSide *s = my ? &b.my_side : &b.enemy_side;
		s->take_damage(BombDamage, i);
		b.process_deathrattles();
	}
}

void BoardSide::trigger_summon(unsigned dead, Minion id, unsigned num)
{
	insert(dead, min(MAX_MINIONS - real_size, num), HSMinion(id));
}

void BoardSide::take_damage(unsigned damage, unsigned t)
{
	HSMinion &targ = buf[t];
	if (targ.skill & attributes::Shield) {
		targ.skill &= ~attributes::Shield;
		return;
	}
	targ.health -= damage;

	return;
}
