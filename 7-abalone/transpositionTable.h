#pragma once

#include "move.h"

#include <cstdint>
#include <array>
#include <memory>
#include <cstddef> 

class TranspositionTable
{
public:
    enum class TypeOfNode : uint8_t
    {
        none,
        exact,
        lower,
        upper
    };
    struct Entry
    {
        Entry();
        Entry(uint64_t key, int evaluation, unsigned int depth, TypeOfNode typeOfNode,
              Move bestMove);

        uint64_t key;
        int evaluation;
        unsigned int depth;
        TypeOfNode typeOfNode;
        Move bestMove;
    };

public:
    TranspositionTable();

    void store(uint64_t zobristHash, int evaluation, unsigned int depth, TypeOfNode typeOfNode,
               Move bestMove);

    const Entry* getEntry(uint64_t zobristHash);

    void clear();

private:
    static size_t hashFunction(uint64_t key);

private:
    inline static constexpr size_t s_numberOfEntires = 3500000;
    std::unique_ptr<std::array<Entry, s_numberOfEntires>> m_table;
};