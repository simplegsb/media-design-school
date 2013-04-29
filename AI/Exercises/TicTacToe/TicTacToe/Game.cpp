#include "AIPlayer.h"
#include "Game.h"

#include <iostream>

using namespace std;

namespace tictactoe
{
	Game::Game(Player* player1, Player* player2) :
			activePlayer(player2), board(), minMaxTree(*player1, *player2), player1(player1), player2(player2)
	{
		initAIPlayers();
	}

	Game::~Game()
	{
		delete player1;
		delete player2;
	}

	void Game::initAIPlayers()
	{
		AIPlayer* aiPlayer1 = dynamic_cast<AIPlayer*>(player1);
		if (aiPlayer1 != NULL)
		{
			aiPlayer1->setMinMaxTree(minMaxTree);
			aiPlayer1->setOpponent(*player2);
		}
		
		AIPlayer* aiPlayer2 = dynamic_cast<AIPlayer*>(player2);
		if (aiPlayer2 != NULL)
		{
			aiPlayer2->setMinMaxTree(minMaxTree);
			aiPlayer2->setOpponent(*player1);
		}
	}

	void Game::mainLoop()
	{
		while (!board.lineOfThreeExists(activePlayer->getSymbol()) && !board.isFull())
		{
			switchToNextPlayer();
			printState();
			playNextMove();
		}
	}

	void Game::playNextMove()
	{
		activePlayer->play(board);
		minMaxTree.moveTo(board, *activePlayer);
	}

	void Game::printState() const
	{
		cout << endl;
		board.print();

		cout << endl;
		cout << activePlayer->getName() << ", it is your turn..." << endl;
	}

	void Game::start()
	{
		cout << "#####################" << endl;
		cout << "Tic-Tac-Toe-Thousand!" << endl;
		cout << "Author: Gary Buyn" << endl;
		cout << "#####################" << endl;

		mainLoop();

		cout << endl;
		board.print();
		
		if (board.isFull())
		{
			cout << endl;
			cout << "#####################" << endl;
			cout << "Draw!" << endl;
			cout << "#####################" << endl;
		}
		else
		{
			cout << endl;
			cout << "#####################" << endl;
			cout << activePlayer->getName() << " is the winner!" << endl;
			cout << "#####################" << endl;
		}
		
		string endGame;
		cin >> endGame;
	}


	void Game::switchToNextPlayer()
	{
		if (*activePlayer == *player1)
		{
			activePlayer = player2;
		}
		else
		{
			activePlayer = player1;
		}
	}
}
