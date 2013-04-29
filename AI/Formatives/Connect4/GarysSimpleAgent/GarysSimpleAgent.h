#ifndef GARYSSIMPLEAGENT_H_
#define GARYSSIMPLEAGENT_H_

#include "../GarysBaseAgent/GarysBaseAgent.h"

/**
 *  An Agent that employs a simple min-max algorithm (does not perform any pruning).
 */
class GarysSimpleAgent : public GarysBaseAgent
{
	public:
		virtual ~GarysSimpleAgent();

		void Evaluate(CBoard& board, CBoard::EBoardPos player);

		void operator=(GarysSimpleAgent& original);

	private:
		/**
		 * Evaluates the given board state, finding the highest possible end score (or score once the maximum depth has
		 * been reached) for the given active player.
		 * 
		 * @param board The board state to evaluate.
		 * @param activePlayer The active player (the player whose turn it is).
		 * @param currentPlayer The current player (the player whose turn it will be at the given depth of the decision
		 * tree).
		 * @param depth The current depth in the decision tree.
		 */
		int Evaluate(const CBoard& board, CBoard::EBoardPos activePlayer, CBoard::EBoardPos currentPlayer, int depth);

		/**
		 * Evaluates all the moves possible from the given board state, finding the highest possible end score (or
		 * score once the maximum depth has been reached) for the given active player.
		 * 
		 * @param board The board state to evaluate.
		 * @param activePlayer The active player (the player whose turn it is).
		 * @param currentPlayer The current player (the player whose turn it will be at the given depth of the decision
		 * tree).
		 * @param depth The current depth in the decision tree.
		 */
		int GetMaximum(const CBoard& board, CBoard::EBoardPos activePlayer, CBoard::EBoardPos currentPlayer, int depth);

		/**
		 * Evaluates all the moves possible from the given board state, finding the lowest possible end score (or score
		 * once the maximum depth has been reached) for the given active player.
		 * 
		 * @param board The board state to evaluate.
		 * @param activePlayer The active player (the player whose turn it is).
		 * @param currentPlayer The current player (the player whose turn it will be at the given depth of the decision
		 * tree).
		 * @param depth The current depth in the decision tree.
		 */
		int GetMinimum(const CBoard& board, CBoard::EBoardPos activePlayer, CBoard::EBoardPos currentPlayer, int depth);
};

#endif // GARYSSIMPLEAGENT_H_
