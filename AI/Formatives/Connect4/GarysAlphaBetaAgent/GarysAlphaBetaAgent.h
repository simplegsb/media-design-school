#ifndef GARYSALPHABETAAGENT_H_
#define GARYSALPHABETAAGENT_H_

#include "../GarysBaseAgent/GarysBaseAgent.h"

/**
 *  An Agent that employs a min-max algorithm with alpha-beta pruning.
 */
class GarysAlphaBetaAgent : public GarysBaseAgent
{
	public:
		virtual ~GarysAlphaBetaAgent();

		void Evaluate(CBoard& board, CBoard::EBoardPos player);

		void operator=(GarysAlphaBetaAgent& original);

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
		 * @param alpha The current alpha value.
		 * @param beta The current beta value.
		 */
		int Evaluate(const CBoard& board, CBoard::EBoardPos activePlayer, CBoard::EBoardPos currentPlayer, int depth, int alpha, int beta);

		/**
		 * Evaluates all the moves possible from the given board state, finding the highest possible end score (or
		 * score once the maximum depth has been reached) for the given active player.
		 * 
		 * @param board The board state to evaluate.
		 * @param activePlayer The active player (the player whose turn it is).
		 * @param currentPlayer The current player (the player whose turn it will be at the given depth of the decision
		 * tree).
		 * @param depth The current depth in the decision tree.
		 * @param alpha The current alpha value.
		 * @param beta The current beta value.
		 */
		int GetMaximum(const CBoard& board, CBoard::EBoardPos activePlayer, CBoard::EBoardPos currentPlayer, int depth, int alpha, int beta);

		/**
		 * Evaluates all the moves possible from the given board state, finding the lowest possible end score (or score
		 * once the maximum depth has been reached) for the given active player.
		 * 
		 * @param board The board state to evaluate.
		 * @param activePlayer The active player (the player whose turn it is).
		 * @param currentPlayer The current player (the player whose turn it will be at the given depth of the decision
		 * tree).
		 * @param depth The current depth in the decision tree.
		 * @param alpha The current alpha value.
		 * @param beta The current beta value.
		 */
		int GetMinimum(const CBoard& board, CBoard::EBoardPos activePlayer, CBoard::EBoardPos currentPlayer, int depth, int alpha, int beta);
};

#endif // GARYSALPHABETAAGENT_H_
