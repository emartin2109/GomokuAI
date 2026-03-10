#include "precomputed/squareRays.hpp"
#include "utils/bitboard.hpp"
#include "define/define.hpp"
#include "precomputed/threatPaterns/opponent.hpp"

#include <iostream>

int main () {
    std::cout << LINE_SPLIT_MASK[33] << std::endl;

    /*
    std::array<bitboard, 4> bitboards = SQUARE_RAYS[210];
    bitboardPrinter(bitboards[TRANSPOSE0] & bitboards[TRANSPOSE45] & bitboards[TRANSPOSE90] & bitboards[TRANSPOSE135]);
    bitboardPrinter(bitboards[TRANSPOSE0] ^ bitboards[TRANSPOSE45] ^ bitboards[TRANSPOSE90] ^ bitboards[TRANSPOSE135]);

    bitboardPrinter(bitboards[TRANSPOSE0]);
    bitboardPrinter(bitboards[TRANSPOSE45]);
    bitboardPrinter(bitboards[TRANSPOSE90]);
    bitboardPrinter(bitboards[TRANSPOSE135]);
    */
}
