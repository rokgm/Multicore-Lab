/**
 * Negamax search strategy.
 */

#include <stdlib.h>
#include <iostream>
#include "search.h"
#include "board.h"
#include "eval.h"

class NegamaxStrategy : public SearchStrategy
{
public:
    // Defines the name of the strategy
    NegamaxStrategy() : SearchStrategy("Negamax") {}

    // Factory method: just return a new instance of this class
    SearchStrategy *clone() override { return new NegamaxStrategy(); }

private:
    /**
     * Implementation of the strategy.
     */
    void searchBestMove() override;
    int negamax(int depth);
    int negamaxEvaluate();
};

void NegamaxStrategy::searchBestMove()
{
    int evaluation = negamax(0);
    std::cout << "Evaluation: " << evaluation << std::endl;
}

int NegamaxStrategy::negamax(int depth)
{
    // Add depth to find shortest win
    int bestEvaluation = minEvaluation() + depth;

    Move m;
    MoveList list;
    _board->generateMoves(list);

    while (list.getNext(m)) {
        int evaluation;
	    playMove(m);
        // check for a win position first
        if (!_board->isValid())
            evaluation = maxEvaluation() - depth;
        else{
            if (depth + 1 < _maxDepth)
                evaluation = -negamax(depth + 1);
            else
                evaluation = evaluate();
        }
        takeBack();

        if (evaluation > bestEvaluation) {
            bestEvaluation = evaluation;
            foundBestMove(depth, m, evaluation);
        }

        if (_stopSearch)
            break;
    }
    
    finishedNode(depth, 0);

    return bestEvaluation;
}

// register ourselves as a search strategy
NegamaxStrategy negamaxtrategy;