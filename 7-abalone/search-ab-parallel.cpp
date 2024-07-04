/**
 * A real world, sequential strategy:
 * Alpha/Beta with Iterative Deepening (ABID)
 *
 * (c) 2005, Josef Weidendorfer
 */

#include <stdio.h>

#include "search.h"
#include "board.h"
#include "eval.h"
// #include <omp.h>

class ABStrategy_par : public SearchStrategy
{
public:
	ABStrategy_par() : SearchStrategy("AB par") {}
	SearchStrategy *clone() { return new ABStrategy_par(); }

	// Move &nextMove() { return _pv[1]; }

private:
	void searchBestMove();
	/* recursive alpha/beta search */
	int alphabeta(int depth, int alpha, int beta, Board& board, Evaluator& ev);

	// We need to evaluate on copies of the board not _board.
    int evaluateParallel(Board& board, Evaluator& ev);

	Move _currentBestMove;
	int _currentMaxDepth;
};


int ABStrategy_par::evaluateParallel(Board& board, Evaluator& ev)
{
    int v = ev.calcEvaluation(&board);

    // Remove this for competition no need to measure.
    if (_sc) _stopSearch = _sc->afterEval();

    return v;
}

/**
 * Entry point for search
 *
 * Does iterative deepening and alpha/beta width handling, and
 * calls alpha/beta search
 */
void ABStrategy_par::searchBestMove()
{

	int alpha = -15000, beta = 15000;
	int currentValue = 0;
	_currentBestMove.type = Move::none;
	// _currentMaxDepth = 1;

	if (_sc && _sc->verbose())
	{
		char tmp[100];
		sprintf(tmp, "Alpha/Beta [%d;%d] with max depth %d", alpha, beta, _currentMaxDepth);
		_sc->substart(tmp);
	}

	currentValue = alphabeta(_currentMaxDepth, -16000, 16000, *_board, *_ev);
	_bestMove = _currentBestMove;
}

/*
 * Alpha/Beta search
 *
 */
int ABStrategy_par::alphabeta(int curr_depth, int alpha, int beta, Board& board, Evaluator& ev)
{

	if (curr_depth >= _maxDepth)
	{
		// printf("Max depth reached\n");
		return -evaluateParallel(board, ev);
	}

	int currentValue = -15000;
	Move m;
	MoveList list;
	// generateMoves(list);
	board.generateMoves(list);

	while (list.getNext(m))
	{
		// printf("Move: %d\n", m.type);
		int value;
		Board boardCopy = board;
        boardCopy.playMove(m);
        value = -alphabeta(curr_depth + 1, -beta, -alpha, boardCopy, ev);
        boardCopy.takeBack();

		if (value > currentValue)
		{
			currentValue = value;
			foundBestMove(curr_depth, m, value);

			if (curr_depth == 0)
				_currentBestMove = m;
		}

		// alpha beta pruning
		if (value > alpha)
		{
			alpha = value;
		}

		if (beta <= alpha)
		{
			break;
		}
	}

	finishedNode(curr_depth, 0);

	if (_sc->verbose())
	{
		// printf("Alpha: %d, beta: %d, value: %d\n", alpha, beta, currentValue);
	}

	return currentValue;
}

// register ourselves as a search strategy
ABStrategy_par abStrategy_par;
