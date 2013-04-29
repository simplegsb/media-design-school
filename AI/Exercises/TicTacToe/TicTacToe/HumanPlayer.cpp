#include "HumanPlayer.h"

#include <iostream>

using namespace std;

namespace tictactoe
{
	HumanPlayer::HumanPlayer(const string& name, const string& symbol) :
			Player(name, symbol)
	{
	}

	HumanPlayer::~HumanPlayer()
	{
	}

	string HumanPlayer::getMove() const
	{
		cout << "Please enter your move e.g. A1 > ";

		string move;
		cin >> move;

		return move;
	}

	string HumanPlayer::getValidMove() const
	{
		string move = getMove();

		while (!validateMove(move))
		{
			cout << "I thought it was quite self explanatory but you managed to mess it up..."
					<< " Try again! A1 = first column, first row (top left)." << endl;
			move = getMove();
		}

		return move;
	}

	unsigned int HumanPlayer::getX(const string& move) const
	{
		return move.at(1) - '1';
	}

	unsigned int HumanPlayer::getY(const string& move) const
	{
		return move.at(0) - 'A';
	}

	void HumanPlayer::play(Board& board)
	{
		string move = getValidMove();

		while (!board.set(getX(move), getY(move), getSymbol()))
		{
			cout << "Looks like that spot is already taken." << endl;
			move = getValidMove();
		}
	}

	bool HumanPlayer::validateMove(const string& move) const
	{
		if (move.length() != 2)
		{
			return false;
		}

		if (move.at(0) < 'A' || move.at(0) > 'C')
		{
			return false;
		}

		if (move.at(1) < '1' || move.at(1) > '3')
		{
			return false;
		}

		return true;
	}
}
