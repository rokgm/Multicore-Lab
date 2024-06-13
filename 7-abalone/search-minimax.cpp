/**
 * Minimax
 *
 *
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
    MinimaxStrategy() : SearchStrategy("Minimax", 3) {}

    // Factory method: just return a new instance of this class
    SearchStrategy *clone() { return new MinimaxStrategy(); }

private:
    /**
     * Implementation of the strategy.
     */
    Move _currentBestMove;
    int _maxDepth;
    void searchBestMove();
    int minimax(Move m, int depth, bool maximizingPlayer);
};

void MinimaxStrategy::searchBestMove()
{
    Move m;

    int value = minimax(m, _maxDepth, true);
}

int MinimaxStrategy::minimax(Move m, int depth, bool maximizingPlayer)
{
    int currentValue = -14999 + depth, value;
    int value;
    int eval;
    MoveList list;

    if (depth == 0) // TODO fix condition: add || game over
    {
        _board->playMove(m);
        value = evaluate();
        if (value > currentValue)
        {
            currentValue = value;
            _sc->foundBestMove(0, m, value); // wtf
            _currentBestMove = m;
        }

        return value;
    }

    if (maximizingPlayer)
    {
        value = -15000;
        generateMoves(list);

        while (list.getNext(m))
        {
            value = std::max(value, minimax(m, depth - 1, false));
            return value;
        }
    }
    else
    {
        value = 15000;
        generateMoves(list);

        while (list.getNext(m))
        {
            value = std::min(value, minimax(m, depth - 1, true));
            return value;
        }
    }
}

// register ourselves as a search strategy
MinimaxStrategy minimaxStrategy;
