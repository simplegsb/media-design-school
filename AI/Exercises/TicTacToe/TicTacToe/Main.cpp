#include "AIPlayer.h"
#include "Game.h"
#include "HumanPlayer.h"

using namespace tictactoe;

int main(int argc, char** argv)
{
	// Comment out the appropriate lines to enable the different combinations of players.
	//Player* player1 = new AIPlayer("AI1", "X");
	Player* player1 = new HumanPlayer("Human1", "X");
	Player* player2 = new AIPlayer("AI2", "O");
	//Player* player2 = new HumanPlayer("Human2", "O");

	Game game(player1, player2);
	game.start();

	return 0;
}
