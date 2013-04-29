#include "Heuristic.h"

namespace tictactoe
{
	int Heuristic::apply(const Board& board, const Player& activePlayer, const Player& otherPlayer) const
	{
		if (board.lineOfThreeExists(activePlayer.getSymbol()))
		{
			return 1;
		}

		if (board.lineOfThreeExists(otherPlayer.getSymbol()))
		{
			return -1;
		}

		return 0;
	}
}
