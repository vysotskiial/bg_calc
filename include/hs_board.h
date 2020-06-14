#pragma once
#include <cstring>
#include <vector>
#include <array>
#include "minions.h"

constexpr unsigned MAX_MINIONS = 7;

class BoardSide {
	friend class HSBoard;

private:
	HSMinion buf[MAX_MINIONS];
	unsigned real_size{0};
	unsigned attacker{0};
	bool increase_attacker{false};

public:
	unsigned size() { return real_size; }
	bool empty() { return real_size == 0; }
	void push_back(const HSMinion &m)
	{
		buf[real_size] = m;
		real_size++;
	}
	HSMinion &operator[](unsigned idx) { return buf[idx]; }
	void erase(unsigned idx)
	{
		if (idx == attacker)
			increase_attacker = false;
		if (idx < attacker)
			attacker--;
		real_size--;
		if (attacker >= real_size)
			attacker = 0;
		if (idx == real_size)
			return;
		memmove(buf + idx, buf + idx + 1, (real_size - idx) * sizeof(HSMinion));
	}
	void insert(unsigned pos, unsigned count, const HSMinion &m)
	{
		memmove(buf + pos + count, buf + pos, (real_size - pos) * sizeof(HSMinion));
		for (unsigned i = 0; i < count; i++)
			buf[pos + i] = m;
		real_size += count;
	}
	void take_damage(unsigned damage, unsigned target);
	void trigger_summon(unsigned dead, Minion id, unsigned num);
	void trigger_buff_all(const HSMinion &buff);
};

class HSBoard {
private:
	BoardSide my_side;
	BoardSide enemy_side;
	BoardSide *attack_side;
	BoardSide *target_side;

	std::vector<HSBoard> *states;
	double odds; // odds of getting to current state
	bool trigger_deathrattle(bool my);
	void trigger_bomb(bool my);
	void trigger_buff_random(bool my, const HSMinion &buff);
	bool my_turn;
	void print();
	int add_next_states();
	void process_attack(unsigned t);
	void process_deathrattles();
	void swap_sides();
	bool won() { return !my_side.empty(); }
	double calc_half();

	template<size_t size>
	void trigger_summon_optional(bool my, const std::array<Minion, size> &options,
	                             unsigned count, unsigned pos,
	                             bool initial = true)
	{
		BoardSide &side = my ? my_side : enemy_side;
		auto real_count = std::min(MAX_MINIONS - side.real_size, count);
		if (!real_count) {
			if (!initial)
				process_deathrattles();
			return;
		}
		bool first = true;
		odds /= size;
		HSBoard copy(*this);

		for (auto minion : options) {
			if (first) {
				side.insert(pos, 1, HSMinion(minion));
				trigger_summon_optional(my, options, count - 1, pos, initial);
				first = false;
				continue;
			}
			HSBoard b(copy);
			BoardSide &s = my ? b.my_side : b.enemy_side;
			s.insert(pos, 1, HSMinion(minion));
			b.trigger_summon_optional(my, options, count - 1, pos, false);
		}
	}

public:
	// My board has to be passed first real attacker is decided in calc_odds
	HSBoard(const BoardSide &my, const BoardSide &enemy)
	  : my_side(my), enemy_side(enemy), attack_side(&my_side),
	    target_side(&enemy_side)
	{
	}
	HSBoard(const HSBoard &other)
	  : my_side(other.my_side), enemy_side(other.enemy_side),
	    states(other.states), odds(other.odds), my_turn(other.my_turn)
	{
		attack_side = my_turn ? &my_side : &enemy_side;
		target_side = my_turn ? &enemy_side : &my_side;
	}
	double calc_odds(); // Returns the odds of you winning the round
};
