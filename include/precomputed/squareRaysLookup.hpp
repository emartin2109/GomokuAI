#pragma once

#include <cstdint>
#include <array>

#include "define/typedef.hpp"
#include "precomputed/values.hpp"
#include "precomputed/linearizationTables.hpp"
#include "utils/linearization.hpp"

const std::array<int, 4> DIRECTIONS_VECTORS = {
    BOARD_LENGTH + 1,       // 45° vector
    BOARD_LENGTH,           // 90° vector
    BOARD_LENGTH - 1,       // 135° vector
    1,                      // 0° vector
};

// WARNING ! This function does not work for board size of 1 or 2
bool isBitIndexValid(size_t bitIndex, size_t previousRow, size_t previousCol) {
    size_t currentRow = bitIndex / BOARD_LENGTH;
    size_t currentCol = bitIndex % BOARD_LENGTH;

    if (bitIndex >= BOARD_SIZE) return false;
    if (abs(int(previousRow) - int(currentRow)) > 1 || abs(int(previousCol) - int(currentCol)) > 1) return false;

    return true;
}

void buildDirectionMask(bitboard &directionMask, size_t bitIndex, int increment) {
    size_t previousRow = bitIndex / BOARD_LENGTH;
    size_t previousCol = bitIndex % BOARD_LENGTH;
    size_t nextBitIndex = bitIndex + increment;

    for (size_t i = 0; i < 4 && isBitIndexValid(nextBitIndex, previousRow, previousCol); i++) {
        directionMask.set(nextBitIndex);

        previousRow = nextBitIndex / BOARD_LENGTH;
        previousCol = nextBitIndex % BOARD_LENGTH;
        nextBitIndex += increment;
    }
}

bitboard buildRayMask(size_t bitIndex, int increment) {
    bitboard rayMask = 0;

    rayMask.set(bitIndex);
    buildDirectionMask(rayMask, bitIndex, increment);
    buildDirectionMask(rayMask, bitIndex, -increment);

    return rayMask;
}

std::array<bitboard, 4> computeRaysFromSquare(size_t index) {  
    std::array<bitboard, 4> rays;

    rays[RayDirection::ROW] = buildRayMask(index, DIRECTIONS_VECTORS[ROW]);

    rays[RayDirection::COLUMN] = linearizeBitboard(buildRayMask(index,
        DIRECTIONS_VECTORS[COLUMN]), LINEARIZATION_TABLES[COLUMN], LinearizationOperation::TRANSPOSE);

    rays[RayDirection::MAIN_DIAG] = linearizeBitboard(buildRayMask(index,
        DIRECTIONS_VECTORS[MAIN_DIAG]), LINEARIZATION_TABLES[MAIN_DIAG], LinearizationOperation::TRANSPOSE);

    rays[RayDirection::ANTI_DIAG] = linearizeBitboard(buildRayMask(index,
        DIRECTIONS_VECTORS[ANTI_DIAG]), LINEARIZATION_TABLES[ANTI_DIAG], LinearizationOperation::TRANSPOSE);

    return rays;
}

std::array<std::array<bitboard, 4>, BOARD_SIZE> buildSquareRayLookup() {
    std::array<std::array<bitboard, 4>, BOARD_SIZE> squareRays;

    for (size_t i = 0; i < BOARD_SIZE; i++)
        squareRays[i] = computeRaysFromSquare(i);

    return squareRays;
}

inline const std::array<std::array<bitboard, 4>, BOARD_SIZE> SQUARE_RAYS = buildSquareRayLookup();
