/**
 * NegamaxParallel search strategy.
 */

#include <stdlib.h>
#include <iostream>
#include <omp.h>

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
    int negamax(int depth, Board& board, Evaluator& ev);

    // We need to evaluate on copies of the board not _board.
    int evaluateParallel(Board& board, Evaluator& ev);
};

int NegamaxParallelStrategy::evaluateParallel(Board& board, Evaluator& ev)
{
    int v = ev.calcEvaluation(&board);

    // Remove this for competition no need to measure.
    if (_sc) _stopSearch = _sc->afterEval();

    return v;
}

void NegamaxParallelStrategy::searchBestMove()
{
    #pragma omp parallel
    {
        #pragma omp single
        {
            negamax(0, *_board, *_ev);
        }
    }
}

int NegamaxParallelStrategy::negamax(int depth, Board& board, Evaluator& evaluator)
{
    // Check for a losing position, return negative minimum because
    // opponents move was last. Add depth to find shortest win.
    if (!board.isValid())
        return minEvaluation() + depth;

    // Evaluation is done from opponents perspective, so negate it.
    if (depth >= _maxDepth)
        return -evaluateParallel(board, evaluator);

    int bestEvaluation = minEvaluation();
    Move m;
    MoveList list;
    board.generateMoves(list);

    // Create tasks until desired depth away from leaves.
    if (depth + 2 <= _maxDepth) {
    // if (true) {
        while (list.getNext(m)) {
            // TODO firstprivate evaluate?
            #pragma omp task firstprivate(m) shared(bestEvaluation)
            {
                // We pass by reference to negamax and copy here, for sequential reference is good.
                Board boardCopy = board;
                boardCopy.playMove(m);
                int evaluation = -negamax(depth + 1, boardCopy, evaluator);
                boardCopy.takeBack();
                #pragma omp critical (negamax)
                {
                    if (evaluation > bestEvaluation) {
                        bestEvaluation = evaluation;
                        foundBestMove(depth, m, evaluation);
                    }
                }
            }
        }
    }
    else {
        while (list.getNext(m)) {
            board.playMove(m);
            int evaluation = -negamax(depth + 1, board, evaluator);
            board.takeBack();
            if (evaluation > bestEvaluation) {
                bestEvaluation = evaluation;
                foundBestMove(depth, m, evaluation);
            }
        }
    }

    #pragma omp taskwait

    finishedNode(depth, nullptr);

    return bestEvaluation;
}

// register ourselves as a search strategy
NegamaxParallelStrategy negamaxParallelStrategy;