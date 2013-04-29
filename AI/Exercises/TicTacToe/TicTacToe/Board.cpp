#include "Board.h"
#include "Player.h"

#include <iostream>

using namespace std;

namespace tictactoe
{
	Board::Board() :
			cells(3, vector<string>(3, NULL_PLAYER_SYMBOL))
	{
	}

	void Board::clear(unsigned int x, unsigned int y)
	{
		cells.at(x).at(y) = NULL_PLAYER_SYMBOL;
	}

	bool Board::diagonalLineOfThreeExists(const string symbol) const
	{
		if (get(0, 0) == symbol && get(1, 1) == symbol && get(2, 2) == symbol)
		{
			return true;
		}

		if (get(0, 2) == symbol && get(1, 1) == symbol && get(2, 0) == symbol)
		{
			return true;
		}

		return false;
	}

	const string& Board::get(unsigned int x, unsigned int y) const
	{
		return cells.at(x).at(y);
	}

	bool Board::isFull() const
	{
		for (unsigned int columnIndex = 0; columnIndex < 3; columnIndex++)
		{
			for (unsigned int rowIndex = 0; rowIndex < 3; rowIndex++)
			{
				if (get(columnIndex, rowIndex) == NULL_PLAYER_SYMBOL)
				{
					return false;
				}
			}
		}

		return true;
	}

	bool Board::horizontalLineOfThreeExists(const string symbol) const
	{
		if (get(0, 0) == symbol && get(1, 0) == symbol && get(2, 0) == symbol)
		{
			return true;
		}

		if (get(0, 1) == symbol && get(1, 1) == symbol && get(2, 1) == symbol)
		{
			return true;
		}

		if (get(0, 2) == symbol && get(1, 2) == symbol && get(2, 2) == symbol)
		{
			return true;
		}

		return false;
	}

	bool Board::lineOfThreeExists(const std::string symbol) const
	{
		if (diagonalLineOfThreeExists(symbol) || horizontalLineOfThreeExists(symbol) ||
			verticalLineOfThreeExists(symbol))
		{
			return true;
		}

		return false;
	}

	void Board::print() const
	{
		cout << "     | 1 | 2 | 3 |" << endl;
		cout << endl;
		cout << "---  -------------" << endl;
		cout << " A   | " << get(0, 0) << " | " << get(1, 0) << " | " << get(2, 0) << " |" << endl;
		cout << "---  -------------" << endl;
		cout << " B   | " << get(0, 1) << " | " << get(1, 1) << " | " << get(2, 1) << " |" << endl;
		cout << "---  -------------" << endl;
		cout << " C   | " << get(0, 2) << " | " + get(1, 2) << " | " << get(2, 2) << " |" << endl;
		cout << "---  -------------" << endl;
	}

	bool Board::set(unsigned int x, unsigned int y, const string& symbol)
	{
		if (cells.at(x).at(y) != NULL_PLAYER_SYMBOL)
		{
			return false;
		}

		cells.at(x).at(y) = symbol;

		return true;
	}

	bool Board::verticalLineOfThreeExists(const string symbol) const
	{
		if (get(0, 0) == symbol && get(0, 1) == symbol && get(0, 2) == symbol)
		{
			return true;
		}

		if (get(1, 0) == symbol && get(1, 1) == symbol && get(1, 2) == symbol)
		{
			return true;
		}

		if (get(2, 0) == symbol && get(2, 1) == symbol && get(2, 2) == symbol)
		{
			return true;
		}

		return false;
	}

	bool operator!=(const Board& lhs, const Board& rhs)
	{
		return !(lhs == rhs);
	}

	bool operator==(const Board& lhs, const Board& rhs)
	{
		return lhs.cells == rhs.cells;
	}
}
