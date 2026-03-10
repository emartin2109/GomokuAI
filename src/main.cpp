#include "utils/bitboard.hpp"
#include "utils/linearization.hpp"
#include "define/define.hpp"
#include "precomputed/linearizationTables.hpp"

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
    bitboard b(1118480);

    bitboardPrinter(b);

    bitboardPrinter(linearizeBitboard(b, LINEARIZATION_TABLES[COLUMN], LinearizationOperation::TRANSPOSE));
    bitboardPrinter(linearizeBitboard(b, LINEARIZATION_TABLES[MAIN_DIAG], LinearizationOperation::TRANSPOSE));
    bitboardPrinter(linearizeBitboard(b, LINEARIZATION_TABLES[ANTI_DIAG], LinearizationOperation::TRANSPOSE));

}
