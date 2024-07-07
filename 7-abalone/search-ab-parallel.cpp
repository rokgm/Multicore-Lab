/**
 * Parallel negamax with alpha-beta pruning search strategy.
 * 
 * Two performance measurements strategies:
 * 1. Fixed depth search: measure time.
 * 2. Fixed time search: measure depth.
 */

#include "search.h"
#include "board.h"
#include "eval.h"
#include "timer.h"

#include <iostream>
#include <chrono>
#include <atomic>
#include <thread>
#include <cmath>
#include <omp.h>

/**
 * When using with time limit set max depth to a high number (e.g. 1000) via command line.
 * Otherwise might be limit by depth and not time.
 * 
 */
class ABParallelStrategy : public SearchStrategy
{
public:
    // Defines the name of the strategy
    ABParallelStrategy() : SearchStrategy("ABParallel") {}

    // Factory method: just return a new instance of this class
    SearchStrategy *clone() override { return new ABParallelStrategy(); }

private:
    /**
     * Implementation of the strategy.
	 * 
	 * Limited by time or max depth.
     */
    void searchBestMove() override;
    int alphabeta(int depth, int alpha, int beta, Board& board, Evaluator& evaluator);
	// Overload for parallel evaluation.
	int evaluate(Board& board, Evaluator& ev);

	/** Run timer on a separate thread that stops search when time is up. */
	void runTimer();
	/** Calculate time based on remaining time and number of moves left.
	   For first 50 moves we use 1/2 of the time, for next 50 we use 1/4,1/8... */
	std::chrono::milliseconds calcTimeForMove();

private:
	// Default time limit of 300 ms.
	Timer _timer{std::chrono::milliseconds(300)};
	std::thread _timerThread;
	std::atomic<bool> _atomicStopSearch = false;

	// TODO Change to 50000 for competition.
	static constexpr std::chrono::milliseconds s_gameDuration{50000};
	std::chrono::milliseconds _remainingTime{s_gameDuration};

	int _currentIterativeDepth = 0;
	int _moveCounter = 0;
};

int ABParallelStrategy::evaluate(Board& board, Evaluator& evaluator)
{
    return evaluator.calcEvaluation(&board);
}

/* IDEA for later. When it is not our move we could still search and
 * store new evaluations into the transposition table.
 */
void ABParallelStrategy::searchBestMove()
{
	// Track remaining time. Subtract at the end of the search.
	auto start = std::chrono::high_resolution_clock::now();
	
	_timer.resetStartTime(calcTimeForMove());	 
	_atomicStopSearch = false;
	_timerThread = std::thread(&ABParallelStrategy::runTimer, this);

	Move iterativeBestMove;
	int maxDepthSearched = 0;

	// Iterative deepening until reaches max depth or seach is stopped via timer.
    for (int depth = 1; depth <= _maxDepth; depth++) {
        _currentIterativeDepth = depth;

		#pragma omp parallel
		{
			#pragma omp single
			{
				alphabeta(0, minEvaluation(), maxEvaluation(), *_board, *_ev);
			}
		}

		// If search wasn't stopped by timer, we can update best move.
		// If for given depth the search was canceled, we can't use it (the whole tree wasn't searched).
		// We use iterativeBestMove to store best move from previous iteration.
		// TODO: implement search first move first so no need to discard. (Needs trans tables)
		if (_atomicStopSearch)
            break;

		if (_bestMove.type == Move::none) {
			std::cout << "Tried to update best move with move of type none." << std::endl;
			break;
		}

		iterativeBestMove = _bestMove;
		maxDepthSearched = depth;
    }

	// Used to stop the timer thread.
	_atomicStopSearch = true;
    _timerThread.join();

	auto timeForMove = std::chrono::duration_cast<std::chrono::milliseconds>(
		 std::chrono::high_resolution_clock::now() - start);
	_remainingTime -= timeForMove;

	_bestMove = iterativeBestMove;
	_moveCounter++;

	std::cout << "    Time for move: " << timeForMove.count() << "ms" << std::endl;
	std::cout << "    Max depth searched: " << maxDepthSearched << std::endl;
	std::cout << "    Remaining time: " << _remainingTime.count() << "ms" << std::endl;
	// print when implemented "Number of transpositions: _countTranspositions
}

int ABParallelStrategy::alphabeta(int depth, int alpha, int beta, Board& board, Evaluator& evaluator)
{	
	if (_atomicStopSearch)
		return minEvaluation();

    // Check for a losing position, return negative minimum because
    // opponents move was last. Add depth to find shortest win.
    if (!board.isValid())
        return minEvaluation() + depth;

    // Evaluation is done from opponents perspective, so negate it.
    if (depth >= _currentIterativeDepth)
        return -evaluate(board, evaluator);

    int bestEvaluation = minEvaluation() + depth;
    Move m;
    MoveList list;

	// Moves are already ordered. See move.h.
    board.generateMoves(list);

	bool leftmostChildEvaluated = false;
	// Need as we can't break out of omp task.
	bool alphaBetaCutoff = false;

	// TODO maybe try search first 2 left children sequentially...
    while (list.getNext(m)) {
		if (alphaBetaCutoff)
			break;

		// TODO try different depths away from leaves, 3 was best for me on laptop.
		// Search sequentially when 3 move away from leaves to avoid task overhead.
		if (!leftmostChildEvaluated || (depth + 3 >= _currentIterativeDepth)) {				
			board.playMove(m);
			int evaluation = -alphabeta(depth + 1, -beta, -alpha, board, evaluator);
			board.takeBack();

			// If search was stopped by timer, we can't use the result.
			if (_atomicStopSearch)
				break;

			if (evaluation > bestEvaluation) {
				bestEvaluation = evaluation;
				if (depth == 0) {
					// Critical not needed as for depth 0 we only get here only on
					// most left node, before parallel tasks are created.
					_bestMove = m;
				}
			}

			if (bestEvaluation > alpha)
				alpha = bestEvaluation;

			if (alpha >= beta) {
				alphaBetaCutoff = true;
				break;
			}

			leftmostChildEvaluated = true;
		}
		else {
			#pragma omp task firstprivate(m, beta, depth)\
				shared(bestEvaluation, alphaBetaCutoff, alpha)
            {	
				Board boardCopy = board;

				boardCopy.playMove(m);
				int evaluation = -alphabeta(depth + 1, -beta, -alpha, boardCopy, evaluator);
				boardCopy.takeBack();

				// If search was stopped by timer, we can't use the result.
				if (!_atomicStopSearch) {
					#pragma omp critical (bestEvaluation)
                    {
                        if (evaluation > bestEvaluation) {
                            bestEvaluation = evaluation;

                            if (depth == 0) {
                                _bestMove = m;
                            }
                        }

                        if (bestEvaluation > alpha)
                            alpha = bestEvaluation;

                        if (alpha >= beta)
                            alphaBetaCutoff = true;
                    }
				}
			}
		}
    }

	#pragma omp taskwait
    return bestEvaluation;
}

void ABParallelStrategy::runTimer()
{
    // Check in case it is modified elsewhere.
    while (!_atomicStopSearch) {
        if (_timer.timeUp()) {
            _atomicStopSearch = true;
            return;
        }
		// To avoid busy waiting
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

std::chrono::milliseconds ABParallelStrategy::calcTimeForMove()
{
	constexpr int numOfMovesForSplit = 50;
	int k = _moveCounter / 50 + 1;
	std::chrono::milliseconds timeForMove = std::chrono::duration_cast<std::chrono::milliseconds>(
		 s_gameDuration * std::pow(0.5, k) / numOfMovesForSplit);		 
	std::cout << "    Calculated time for move: " << timeForMove.count() << "ms" << std::endl;

	return timeForMove;
}

// register ourselves as a search strategy
ABParallelStrategy alphabetaParallelStrategy;