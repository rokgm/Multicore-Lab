/**
 * NegamaxParallel search strategy.
 */

#include <stdlib.h>
#include <iostream>
#include "search.h"
#include "board.h"
#include "eval.h"

class NegamaxParallelStrategy : public SearchStrategy
{
public:
    // Defines the name of the strategy
    NegamaxParallelStrategy() : SearchStrategy("NegamaxParallel") {}

    // Factory method: just return a new instance of this class
    SearchStrategy *clone() override { return new NegamaxParallelStrategy(); }

private:
    /**
     * Implementation of the strategy.
     */
    void searchBestMove() override;
    int negamax(int depth, Board& board);
};

void NegamaxParallelStrategy::searchBestMove()
{
    int bestEvaluation = minEvaluation();
    Move bestMove;
    MoveList list;
    _board->generateMoves(list);

    #pragma omp parallel
    {
    #pragma omp single
    {

    Move m;
    while (list.getNext(m))
    {
        Board boardCopy = *_board;
        #pragma omp task shared(bestEvaluation, bestMove) firstprivate(m, boardCopy)
        {
            boardCopy.playMove(m);
            int evaluation = -negamax(0, boardCopy);
            boardCopy.takeBack();

            #pragma omp critical
            {
                if (evaluation > bestEvaluation) {
                    bestEvaluation = evaluation;
                    bestMove = m;
                }
            }
        }
    }

    }
    }
}

int NegamaxParallelStrategy::negamax(int depth, Board& board)
{
    // Check for a losing position, return negative minimum because
    // opponents move was last. Add depth to find shortest win.
    if (!board.isValid())
        return minEvaluation() + depth;

    // Evaluation is done from opponents perspective, so negate it.
    if (depth >= _maxDepth)
        return -_ev->calcEvaluation(&board);

    int bestEvaluation = minEvaluation();
    Move m;
    MoveList list;
    board.generateMoves(list);

    while (list.getNext(m)) {
        board.playMove(m);
        int evaluation = -negamax(depth + 1, board);
        board.takeBack();

        if (evaluation > bestEvaluation) {
            bestEvaluation = evaluation;

            // No need to update best move, searchBestMove will do that
            // #pragma omp critical
            // {
            //     _bestMove = m;
            // }
        }

        if (_stopSearch)
            break;
    }

    return bestEvaluation;
}

// register ourselves as a search strategy
NegamaxParallelStrategy negamaxParallelStrategy;