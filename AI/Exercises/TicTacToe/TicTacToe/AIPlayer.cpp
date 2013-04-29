#include "AIPlayer.h"

using namespace std;

namespace tictactoe
{
	AIPlayer::AIPlayer(const string& name, const string& symbol) :
			Player(name, symbol), minMaxTree(NULL), opponent(NULL)
	{
	}

	AIPlayer::~AIPlayer()
	{
	}

	void AIPlayer::play(Board& board)
	{
		minMaxTree->evaluate(*this, *opponent);
		board.set(minMaxTree->getOptimalMoveX(), minMaxTree->getOptimalMoveY(), getSymbol());
	}

	void AIPlayer::setMinMaxTree(MinMaxTree& minMaxTree)
	{
		this->minMaxTree = &minMaxTree;
	}

	void AIPlayer::setOpponent(const Player& opponent)
	{
		this->opponent = &opponent;
	}
}
