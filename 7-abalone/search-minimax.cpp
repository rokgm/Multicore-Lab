/**
 * Very simple example strategy:
 * Search all possible positions reachable via one move,
 * and return the move leading to best position
 *
 * (c) 2006, Josef Weidendorfer
 */

#include <stdlib.h>
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
    MinimaxStrategy() : SearchStrategy("OneLevel") {}

    // Factory method: just return a new instance of this class
    SearchStrategy *clone() { return new MinimaxStrategy(); }

private:
    /**
     * Implementation of the strategy.
     */
    void searchBestMove();
};

void MinimaxStrategy::searchBestMove()
{
    // TODO: call minimax
    int maxDepth = 5;
    Move m;
}

int minimax(Move &m, int depth, bool maximizingPlayer, int maxDepth)
{
    int value;
    int eval;
    MoveList list;

    if (depth == 0 || depth == maxDepth) // TODO fix condition
    {
        playMove(m);
        value = evaluate();

        return value;
    }

    if (maximizingPlayer)
    {
        value = -15000;
        generateMoves(list);

        while (list.getNext(m))
        {
            value = std::max(value, minimax(m, depth - 1, false, maxDepth));
            return value;
        }
    }
    else
    {
        value = 15000;
        while (list.getNext(m))
        {
            value = std::min(value, minimax(m, depth - 1, true, maxDepth));
            return value;
        }
    }
}

// register ourselve as a search strategy
MinimaxStrategy minimaxStrategy;
