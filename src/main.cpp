#include "utils/bitboard.hpp"
#include "utils/linearization.hpp"
#include "precomputed/linearizationTables.hpp"
#include "precomputed/squareRaysLookup.hpp"
#include "precomputed/basicMasks/proximityMask.hpp"
#include "precomputed/threatPaterns/lineSplitMaskLookup.hpp"

#include <iostream>

// 17043521

/*
10000
01000
00100
00010
00001
*/

/*
10000
00000
00000
00000
00000
*/

int main () {
    bitboard b(65536);

    std::cout << " " << threat_patern(0) << std::endl;
    std::cout << " " << std::get<0>(LINE_SPLIT_MASK[0]) << "\n" << std::get<1>(LINE_SPLIT_MASK[0]) << std::endl;

    /*
    for (size_t i = 0; i < BOARD_SIZE; i++) {
        bitboardPrinter(PROXIMITY_MASK[i]);
    }
    */

    /*

    bitboardPrinter(b);
    bitboardPrinter(SQUARE_RAYS[16][RayDirection::ROW]);

    bitboardPrinter(linearizeBitboard(b, LINEARIZATION_TABLES[COLUMN], LinearizationOperation::TRANSPOSE));
    bitboardPrinter(linearizeBitboard(SQUARE_RAYS[16][RayDirection::COLUMN], LINEARIZATION_TABLES[COLUMN], LinearizationOperation::RESTORE));

    bitboardPrinter(linearizeBitboard(b, LINEARIZATION_TABLES[MAIN_DIAG], LinearizationOperation::TRANSPOSE));
    bitboardPrinter(linearizeBitboard(SQUARE_RAYS[16][RayDirection::MAIN_DIAG], LINEARIZATION_TABLES[MAIN_DIAG], LinearizationOperation::RESTORE));

    bitboardPrinter(linearizeBitboard(b, LINEARIZATION_TABLES[ANTI_DIAG], LinearizationOperation::TRANSPOSE));
    bitboardPrinter(linearizeBitboard(SQUARE_RAYS[16][RayDirection::ANTI_DIAG], LINEARIZATION_TABLES[ANTI_DIAG], LinearizationOperation::RESTORE));
    */
    }
