#include "utils/bitboard.hpp"
#include "utils/linearization.hpp"
#include "precomputed/linearizationTables.hpp"
#include "precomputed/squareRaysLookup.hpp"

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

    bitboardPrinter(b);
//    bitboardPrinter(SQUARE_RAYS[16][RayDirection::ROW]);

//    bitboardPrinter(linearizeBitboard(b, LINEARIZATION_TABLES[COLUMN], LinearizationOperation::TRANSPOSE));
//    bitboardPrinter(SQUARE_RAYS[16][RayDirection::COLUMN]);

//    bitboardPrinter(linearizeBitboard(b, LINEARIZATION_TABLES[MAIN_DIAG], LinearizationOperation::TRANSPOSE));
//    bitboardPrinter(SQUARE_RAYS[16][RayDirection::MAIN_DIAG]);

//    bitboardPrinter(linearizeBitboard(b, LINEARIZATION_TABLES[ANTI_DIAG], LinearizationOperation::TRANSPOSE));
//    bitboardPrinter(SQUARE_RAYS[16][RayDirection::ANTI_DIAG]);
}
