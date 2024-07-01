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
#include <omp.h>

class ABParStrategy : public SearchStrategy
{
public:
	ABParStrategy() : SearchStrategy("AB parallel") {}
	SearchStrategy *clone() { return new ABParStrategy(); }

	// Move &nextMove() { return _pv[1]; }

private:
	void searchBestMove();
	/* recursive alpha/beta search */
	int alphabeta(int depth, int alpha, int beta);

	Move _currentBestMove;
	int _currentMaxDepth;
	float _perc_sequential;
};

/**
 * Entry point for search
 *
 */
void ABParStrategy::searchBestMove()
{

	int alpha = -15000, beta = 15000;
	int currentValue = 0;
	_currentBestMove.type = Move::none;
	_perc_sequential = 0.5;
	// _currentMaxDepth = 1;

	if (_sc && _sc->verbose())
	{
		char tmp[100];
		sprintf(tmp, "Alpha/Beta [%d;%d] with max depth %d", alpha, beta, _currentMaxDepth);
		_sc->substart(tmp);
	}

#pragma omp parallel
	{
#pragma omp single
		{
			Move m;
			MoveList list;
			generateMoves(list);
			int length = list.getLength();
			int i = 0;
			int curr_depth = 0;

			list.getNext(m);

			int value;
			playMove(m);
			if (curr_depth + 1 < _maxDepth)
			{
				value = -alphabeta(curr_depth + 1, -beta, -alpha);
			}
			else
			{
				value = evaluate();
			}

			takeBack();

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

			// if (beta <= alpha)
			// {
			// 	return alpha;
			// }

			finishedNode(curr_depth, 0);

			// printf("Thread: %d\n", omp_get_thread_num());
			currentValue = alphabeta(0, -beta, -alpha);
			_bestMove = _currentBestMove;
		}
	}
}

/*
 * Alpha/Beta search
 *
 */
int ABParStrategy::alphabeta(int curr_depth, int alpha, int beta)
{

	if (curr_depth >= _maxDepth)
	{
		// printf("Max depth reached\n");
		return evaluate();
	}

	int currentValue = -15000;
	Move m;
	MoveList list;
	generateMoves(list);
	// int length = list.getLength();
	int i = 0;

	while (list.getNext(m))
	{
#pragma omp task firstprivate(m, curr_depth, currentValue) // TODO missing board
		{
			// printf("Thread: %d, depth: %d\n", omp_get_thread_num(), curr_depth);
			// printf("Move: %d\n", m.type);
			int value;
			playMove(m);
			if (curr_depth + 1 < _maxDepth)
			{
				value = -alphabeta(curr_depth + 1, -beta, -alpha);
			}
			else
			{
				value = evaluate();
			}

			takeBack();

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
				// return alpha;
			}

#pragma omp critical
			{
				finishedNode(curr_depth, 0);
			}
		}
#pragma omp taskwait
	}

	// if (_sc->verbose())
	// {
	// 	printf("Alpha: %d, beta: %d, value: %d\n", alpha, beta, currentValue);
	// }

	return alpha;
}

// register ourselves as a search strategy
ABParStrategy abparStrategy;
