#ifndef PLAYER_H_
#define PLAYER_H_

#include <string>

#include "Board.h"

namespace tictactoe
{
	class Player
	{
		public:
			Player(const std::string& name, const std::string& symbol);

			virtual ~Player();

			const std::string& getName() const;

			const std::string& getSymbol() const;

			virtual void play(Board& board) = 0;

		private:
			std::string name;

			std::string symbol;
	};

	const std::string NULL_PLAYER_SYMBOL = " ";

	bool operator!=(const Player& lhs, const Player& rhs);

	bool operator==(const Player& lhs, const Player& rhs);
}

#endif /* PLAYER_H_ */
