#ifndef GAME_H_
#define GAME_H_

#include "AlphaBetaPruningMinMaxTree.h"
#include "Board.h"
#include "Player.h"
#include "SimpleMinMaxTree.h"

namespace tictactoe
{
	class Game
	{
		public:
			Game(Player* player1, Player* player2);

			~Game();

			void start();

		private:
			Player* activePlayer;

			Board board;

			// Comment out one of these to select the MinMaxTree implementation.
			SimpleMinMaxTree minMaxTree;
			//AlphaBetaPruningMinMaxTree minMaxTree;

			Player* player1;

			Player* player2;

			void initAIPlayers();

			void mainLoop();

			void playNextMove();

			void printState() const;

			void switchToNextPlayer();
	};
}

#endif /* GAME_H_ */
