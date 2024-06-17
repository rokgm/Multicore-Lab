/**
 * Minimax
 *
 *
 */

#include <stdlib.h>
#include <iostream>
#include "search.h"
#include "board.h"
#include "eval.h"

/**
 * To create your own search strategy:
 * - copy this file into another one,
 * - change the class name one the name given in constructor,
 * - adjust clone() to return an instance of your class
 * - adjust last line of this file to create a global instance
 *   of your class
 * - adjust the Makefile to include your class in SEARCH_OBJS
 * - implement searchBestMove()
 *
 * Advises for implementation of searchBestMove():
 * - call foundBestMove() when finding a best move since search start
 * - call finishedNode() when finishing evaluation of a tree node
 * - Use _maxDepth for strength level (maximal level searched in tree)
 */

class MinimaxStrategy : public SearchStrategy
{
public:
    // Defines the name of the strategy
    MinimaxStrategy() : SearchStrategy("Minimax") {}

    // Factory method: just return a new instance of this class
    SearchStrategy *clone() { return new MinimaxStrategy(); }

private:
    /**
     * Implementation of the strategy.
     */
    // Move _currentBestMove;
    int _currDepth;
    void searchBestMove();
    int minimax(int depth);
};

void MinimaxStrategy::searchBestMove()
{
    int bestEvaluation = minEvaluation();
    int evaluation;
    _currDepth = 0;
    setMaxDepth(3); // TODO: Where to do this?
    evaluation = minimax(_currDepth);
    std::cout << "Evaluation: " << evaluation << std::endl;
}

int MinimaxStrategy::minimax(int depth)
{
    // Check is finished searching
    // TODO: add check to see if game over?
    if (depth >= _maxDepth)
    {
        return evaluate();
    }

    int max = -15000;
    int min = 15000;
    Move m;
    MoveList list;
    // Generate all possible moves from current position
    generateMoves(list);

    // Evaluate all moves in move list and recursively traverse until max depth
    while (list.getNext(m))
    {
        int evaluation;
        playMove(m);

        if (depth + 1 < _maxDepth)
        {
            evaluation = -minimax(depth + 1);
        }
        else
        {
            evaluation = evaluate();
        }

        // Reset the board
        takeBack();

        if (evaluation > max)
        {
            // Found best move so far
            max = evaluation;
            foundBestMove(depth, m, evaluation);
        }
    }

    finishedNode(depth, 0);
    return max;
}

// register ourselves as a search strategy
MinimaxStrategy minimaxStrategy;