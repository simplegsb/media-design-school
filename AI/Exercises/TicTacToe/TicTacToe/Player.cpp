#include "Player.h"

using namespace std;

namespace tictactoe
{
	Player::Player(const string& name, const string& symbol) :
			name(name), symbol(symbol)
	{
	}

	Player::~Player()
	{
	}

	const string& Player::getName() const
	{
		return name;
	}

	const string& Player::getSymbol() const
	{
		return symbol;
	}

	bool operator!=(const Player& lhs, const Player& rhs)
	{
		return !(lhs == rhs);
	}

	bool operator==(const Player& lhs, const Player& rhs)
	{
		return lhs.getSymbol() == rhs.getSymbol();
	}
}
