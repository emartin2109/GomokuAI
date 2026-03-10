#pragma once

#include "define/typedef.hpp"
#include "precomputed/linearizationTables.hpp"


bitboard linearizeBitboard(const bitboard &b, const linearization_table &t, const LinearizationOperation &op) {
    bitboard out;

    for (size_t i = 0; i < BOARD_SIZE; ++i)
        out[t[op][i]] = b[i];

    return out;
}
