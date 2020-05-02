#pragma once
#include <iterator>//distance
#include <map>
#include <list>
#include <stack>
#include "minions.h"

class HSBoard;

typedef std::list<HSMinion>::iterator minion;
class BoardSide {
friend class HSBoard;
private:
	std::list<HSMinion> board;
	minion attacker;

	bool attacker_died;

	void take_damage(unsigned damage, minion target);
	void trigger_rat(minion dead);
	minion pop_dead();
	void next_attacker();
	bool empty() { return board.empty(); }
	unsigned size() { return board.size(); }
	BoardSide(const BoardSide &other): board(other.board), attacker_died(false)
	{
		if (!board.size()) return;
		attacker = board.begin();
		std::advance(attacker, std::distance<std::list<HSMinion>::const_iterator>(other.board.begin(), other.attacker));
	}
	void cleanup();
	BoardSide(const std::list<HSMinion> &b): board(b), attacker(board.begin()), attacker_died(false) {}
};

class HSBoard {
private:
	enum Side {
		attack,
		target
	};
	std::map<Side, BoardSide *> sides;

	std::stack<HSBoard *> *states;
	double odds;//odds of getting to current state
	bool trigger_deathrattle(Side s);
	void trigger_bomb(Side s);
	bool my_turn;
	void print();
	int add_next_states();
	void process_attack(minion t);
	void process_deathrattles();
	void swap_sides();
	bool won()
	{
		if (my_turn) return !sides[Side::attack]->empty();
		else return !sides[Side::target]->empty();
	}
public:
	//My board has to be passed first real attacker is decided in calc_odds
	HSBoard(const std::list<HSMinion> &my, const std::list<HSMinion> &enemy)
	{
		sides[Side::attack] = new BoardSide(my);
		sides[Side::target] = new BoardSide(enemy);
	}
	HSBoard(const HSBoard &other): odds(other.odds), my_turn(other.my_turn)
	{
		states = other.states;
		sides[Side::attack] = new BoardSide(*other.sides.at(Side::attack));
		sides[Side::target] = new BoardSide(*other.sides.at(Side::target));
	}
	~HSBoard()
	{
		delete sides[Side::attack];
		delete sides[Side::target];
	}
	double calc_odds();//Returns the odds of you winning the round
};
