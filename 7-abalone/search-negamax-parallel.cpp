/**
 * NegamaxParallel search strategy.
 */

#include <stdlib.h>
#include <iostream> 
#include <vector>   // TODO remove after debug

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

    // We need to evaluate on copies of the board not _board.
    int evaluate_parallel(Board& board);
};

int NegamaxParallelStrategy::evaluate_parallel(Board& board)
{
    int v = _ev->calcEvaluation(&board); 
    if (_sc) _stopSearch = _sc->afterEval();

    return v;
}

void NegamaxParallelStrategy::searchBestMove()
{
    int bestEvaluation = minEvaluation();
    MoveList list;
    _board->generateMoves(list);

    #pragma omp parallel
    {
    #pragma omp single nowait
    {
    
    Move m;
    while (list.getNext(m))
    {
        #pragma omp task firstprivate(m)
        {
            // Must copy as firstprivate would just copy the pointer.
            Board boardCopy = *_board;

            boardCopy.playMove(m);
            int evaluation = -negamax(1, boardCopy);
            boardCopy.takeBack();

            #pragma omp critical (searchBestMove)
            {
                if (evaluation > bestEvaluation) {
                    bestEvaluation = evaluation;
                    _bestMove = m;
                }
            }
        }
    }

    } // omp single
    } // omp parallel
}

int NegamaxParallelStrategy::negamax(int depth, Board& board)
{
    // Check for a losing position, return negative minimum because
    // opponents move was last. Add depth to find shortest win.
    if (!board.isValid())
        return minEvaluation() + depth;

    // Evaluation is done from opponents perspective, so negate it.
    if (depth >= _maxDepth)
        return -evaluate_parallel(board);

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
        }

        // WARNING FOR LATER!
        // Evaluate can stop search if search callbacks are true.
        // Also for PV, call foundBestMove and finishedNode to store whole principal variation.
        // if (_stopSearch)
        //     break;
    }

    return bestEvaluation;
}

// register ourselves as a search strategy
NegamaxParallelStrategy negamaxParallelStrategy;