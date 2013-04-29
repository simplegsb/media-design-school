#ifndef HUMANPLAYER_H_
#define HUMANPLAYER_H_

#include "Player.h"

namespace tictactoe
{
	class HumanPlayer : public Player
	{
		public:
			HumanPlayer(const std::string& name, const std::string& symbol);
			
			virtual ~HumanPlayer();

			void play(Board& board);

		private:
			std::string getMove() const;

			std::string getValidMove() const;

			unsigned int getX(const std::string& move) const;

			unsigned int getY(const std::string& move) const;

			bool validateMove(const std::string& move) const;
	};
}

#endif /* HUMANPLAYER_H_ */
