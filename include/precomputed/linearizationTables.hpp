#pragma once

#include <array>

#include "config.hpp"
#include "define/typedef.hpp"
#include "define/enums.hpp"
#include "precomputed/values.hpp"

enum LinearizationOperation {
    TRANSPOSE=0,
    RESTORE=1
};

linearization_table buildBitboardLinearizationTableColumn()
{
    size_t index = 0;
    linearization_table table{};

    for (size_t c = 0; c < BOARD_LENGTH; ++c) {
        for (size_t r = 0; r < BOARD_LENGTH; ++r) {
            size_t from = r * BOARD_LENGTH + c;
            size_t to = index++;

            table[LinearizationOperation::TRANSPOSE][from] = to;
            table[LinearizationOperation::RESTORE][to] = from;
        }
    }

    return table;
}

linearization_table buildBitboardLinearizationTableMainDiag()
{
    size_t index = 0;
    linearization_table table{};

    for (int diag = -(int)BOARD_LENGTH + 1; diag < (int)BOARD_LENGTH; ++diag) {
        for (size_t r = 0; r < BOARD_LENGTH; ++r) {
            int c = r - diag;

            if (c < 0 || c >= (int)BOARD_LENGTH)
                continue;

            size_t from = r * BOARD_LENGTH + c;
            size_t to = index++;

            table[LinearizationOperation::TRANSPOSE][from] = to;
            table[LinearizationOperation::RESTORE][to] = from;
        }
    }

    return table;
}

linearization_table buildBitboardLinearizationTableAntiDiag()
{
    size_t index = 0;
    linearization_table table{};

    for (int diag = 0; diag <= 2 * (int)(BOARD_LENGTH - 1); ++diag) {
        for (size_t r = 0; r < BOARD_LENGTH; ++r) {
            int c = diag - r;

            if (c < 0 || c >= (int)BOARD_LENGTH)
                continue;

            size_t from = r * BOARD_LENGTH + c;
            size_t to = index++;

            table[LinearizationOperation::TRANSPOSE][from] = to;
            table[LinearizationOperation::RESTORE][to] = from;
        }
    }

    return table;
}

std::array<linearization_table, 3> buildLinearizationTables() {
    std::array<linearization_table, 3> linearizationTables;

    linearizationTables[RayDirection::COLUMN] = buildBitboardLinearizationTableColumn();
    linearizationTables[RayDirection::MAIN_DIAG] = buildBitboardLinearizationTableMainDiag();
    linearizationTables[RayDirection::ANTI_DIAG] = buildBitboardLinearizationTableAntiDiag();

    return linearizationTables;
}

// WARNING ! This variable with not be able to compute correctly if the board size is 65536 (2^16) or more
const inline std::array<linearization_table, 3> LINEARIZATION_TABLES = buildLinearizationTables();
