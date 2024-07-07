#pragma once

class Board;

#include <cstdint>
#include <array>
#include <cstddef> 

/**
 * https://www.chessprogramming.org/Zobrist_Hashing adapted for abalone
 */
class ZobristHash
{
public:
    /**
     * Should only be used when constructing the board, function is slow.
     */
    static uint64_t calculateZobristKey(const Board& board);

    /**
     * Update zobrist with xor at field position for one color. Order is inner rings index.
     * Use allFieldToOrder array to covert from field index to order index.
     * Color is either color1 or color2. Args not checked so be careful for bounds...
     */
    inline static void updateZobristKeyField(uint64_t& zobristKey, int order, int color)
    {
        zobristKey ^= s_squares[order][color];
    }

    inline static void updateZobristSideToMove(uint64_t& zobristKey)
    {
        zobristKey ^= s_sideToMove;
    }

private:
    static void initZobristNumbers();

private:
    /**
     * For each square on board, we generate 2 random numbers for both colors.
     */
    inline static std::array<std::array<uint64_t, 2>, 61> s_squares{};
    inline static uint64_t s_sideToMove{};
};