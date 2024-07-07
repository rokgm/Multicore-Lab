#include "zobristHash.h"
#include "board.h"

#include <random>


void ZobristHash::initZobristNumbers()
{
    // We set seed to be always the same.
    std::mt19937_64 rng(29979258);
    std::uniform_int_distribution<uint64_t> distribution(std::numeric_limits<uint64_t>::min(),
                                                         std::numeric_limits<uint64_t>::max());

    for (auto& colors : s_squares) {
        for (auto& color : colors) {
            color = distribution(rng);
        }
    }
    s_sideToMove = distribution(rng);
}

uint64_t ZobristHash::calculateZobristKey(const Board& board)
{
    initZobristNumbers();

    uint64_t key = 0;

    for (int i = 0; i < Board::RealFields; i++) {
        int color = board.getColor(i);
        if (color == Board::color1)
            key ^= s_squares[i][color];
        else if (color == Board::color2)
            key ^= s_squares[i][color];
    }

    if (board.actColor() == Board::color2)
        key ^= s_sideToMove;

    return key;
}