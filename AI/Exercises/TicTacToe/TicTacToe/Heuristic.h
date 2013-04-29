#ifndef HEURISTIC_H_
#define HEURISTIC_H_

#include "Board.h"
#include "Player.h"

namespace tictactoe
{
	class Heuristic
	{
		public:
			int apply(const Board& board, const Player& activePlayer, const Player& otherPlayer) const;
	};
}

#endif /* HEURISTIC_H_ */
