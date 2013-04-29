#ifndef AIPLAYER_H_
#define AIPLAYER_H_

#include "MinMaxTree.h"
#include "Player.h"

namespace tictactoe
{
	class AIPlayer : public Player
	{
		public:
			AIPlayer(const std::string& name, const std::string& symbol);

			virtual ~AIPlayer();

			void play(Board& board);

			void setMinMaxTree(MinMaxTree& opponent);

			void setOpponent(const Player& opponent);

		private:
			MinMaxTree* minMaxTree;

			const Player* opponent;
	};
}

#endif /* AIPLAYER_H_ */
