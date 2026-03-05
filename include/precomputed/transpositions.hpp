#pragma once

#include <bitset>
#include <cstdint>
#include <tuple>
#include <vector>
#include <array>
#include <unordered_map>
#include <algorithm>
#include <iostream>

#include "define/typedef.hpp"
#include "precomputed/values.hpp"

// WARNING ! This function does not work for board size of 1 or 2
bool isBitIndexValid(size_t bitIndex, size_t previousRow, size_t previousCol) {
    size_t currentRow = bitIndex / BOARD_LENGTH;
    size_t currentCol = bitIndex % BOARD_LENGTH;

    if (bitIndex >= BOARD_SIZE) return false;
    if (abs(previousRow - currentRow) > 1 || abs(previousCol - currentCol) > 1) return false;

    return true;
}

void buildDirectionMask(bitboard &directionMask, size_t bitIndex, size_t increment) {
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

bitboard buildRayMask(size_t bitIndex, size_t increment) {
    bitboard rayMask = 0;

    rayMask.set(bitIndex);
    buildDirectionMask(rayMask, bitIndex, increment);
    buildDirectionMask(rayMask, bitIndex, -increment);

    return rayMask;
}

std::array<bitboard, 4> computeRaysFromSquare(size_t index) {   
    bitboard lineMask = buildRayMask(index, 1);
    bitboard diag45Mask = buildRayMask(index, BOARD_LENGTH - 1);
    bitboard columnMask = buildRayMask(index, BOARD_LENGTH);
    bitboard diag135Mask = buildRayMask(index, BOARD_LENGTH + 1);

    return std::array<bitboard, 4>{lineMask, diag45Mask, columnMask, diag135Mask};
}

std::array<std::array<bitboard, 4>, BOARD_SIZE> buildSquareRayLookup() {
    std::array<std::array<bitboard, 4>, BOARD_SIZE> squareRays;

    for (size_t i = 0; i < BOARD_SIZE; i++)
        squareRays[i] = computeRaysFromSquare(i);

    return squareRays;
}

std::array<std::array<bitboard, 4>, BOARD_SIZE> SQUARE_RAYS = buildSquareRayLookup();
