#ifndef GARYSBASEAGENT_H_
#define GARYSBASEAGENT_H_

#include "Common.h"
#include "Board.h"
#include "../PlayerAgentImp.h"

/**
 *  A base class for Agents.
 */
class GarysBaseAgent : public IPlayerAgentImp
{
	public:
		GarysBaseAgent();

		virtual ~GarysBaseAgent();

		/**
		 * Evaluates the given board state, finding the best move for the given active player.
		 * 
		 * @param board The board state to evaluate.
		 * @param player The active player (the player whose turn it is).
		 */
		virtual void Evaluate(CBoard& board, CBoard::EBoardPos player) = 0;

		/**
		 * Checks if the given board is complete i.e. if all the positions of the board have been taken. This method is
		 * used instead of CGameBoard::IsComplete() because it results in undefined behaviour when used on clones (the
		 * number of free positions, which is used by that method, is not copied to the clone).
		 * 
		 * @param board The board to check.
		 *
		 * @return True if the board is complete, false otherwise.
		 */
		bool IsComplete(const CBoard& board) const;

		/**
		 * Determines whether the next move has been decided yet.
		 *
		 * @param[out] column The chosen column.
		 *
		 * @return True if the next move has been decided, false otherwise.
		 */
		bool IsNextColumnChosen(int& column);

		/**
		 * Sets the maximum depth to which the Agent will search in the decision tree (i.e. how many moves it will look
		 * ahead).
		 *
		 * @param maximumDepth The maximum depth to which the Agent will search in the decision tree.
		 */
		void SetMaximumDepth(int maximumDepth);

		/**
		 * Triggers the Agent to begin evaluating the given board for the given active player.
		 *
		 * @param board The board to evaluate.
		 * @param player The active player (the player whose turn it is).
		 *
		 * @return True. I'm not sure what this signifies, it does not appear to be checked anywhere.
		 */
		bool StartChooseNextColumn(CBoard* board, int playerId);

	protected:
		int GetMaximumDepth();

		CBoard::EBoardPos GetOtherPlayer(CBoard::EBoardPos player);

		CBoard::EBoardPos GetPlayerEnum(int playerId);

		int GetRandomInt(int min, int max);

		void SetChosenColumn(int chosenColumn);

	private:
		int chosenColumn;

		int maximumDepth;
};

#endif // GARYSBASEAGENT_H_
