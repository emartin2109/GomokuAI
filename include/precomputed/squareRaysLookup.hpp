#pragma once

#include <cstdint>
#include <array>

#include "define/typedef.hpp"
#include "precomputed/values.hpp"

const std::array<int, 4> DIRECTIONS_VECTORS = {
    1,                      // 0° vector
    BOARD_LENGTH - 1,       // 45° vector
    BOARD_LENGTH,           // 90° vector
    BOARD_LENGTH + 1        // 135° vector
};

// WARNING ! This function does not work for board size of 1 or 2
/**
 * @brief check if a given bit index in the bitboard is valid compare to the previous one
 * 
 * @param bitIndex the index to check in the bitboard
 * @param previousRow the row of the previous bitIndex
 * @param previousCol the column of the previous bitIndex
 * @return true 
 * @return false 
 */
bool isBitIndexValid(size_t bitIndex, size_t previousRow, size_t previousCol) {
    size_t currentRow = bitIndex / BOARD_LENGTH;
    size_t currentCol = bitIndex % BOARD_LENGTH;

    if (bitIndex >= BOARD_SIZE) return false;
    if (abs(int(previousRow) - int(currentRow)) > 1 || abs(int(previousCol) - int(currentCol)) > 1) return false;

    return true;
}

/**
 * @brief complete a directional mask, the directional mask shall not extend more than 4 square in each directions
 * 
 * @param directionMask the directional mask to complete
 * @param bitIndex the index of the first bit
 * @param increment the directional vector that should be computed
 */
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

/**
 * @brief create a rayMask that extant in both directions of a given vector
 * 
 * @param bitIndex the starting point of the ray mask
 * @param increment the directional vector that will be computed alongside its inverse vector
 * @return bitboard the computed rayMask
 */
bitboard buildRayMask(size_t bitIndex, int increment) {
    bitboard rayMask = 0;

    rayMask.set(bitIndex);
    buildDirectionMask(rayMask, bitIndex, increment);
    buildDirectionMask(rayMask, bitIndex, -increment);

    return rayMask;
}

/**
 * @brief compute all four rayMasks for a given square
 * 
 * @param index the index of the square that should be computed 
 * @return std::array<bitboard, 4> an array containing all four computed rayMasks
 */
std::array<bitboard, 4> computeRaysFromSquare(size_t index) {  
    std::array<bitboard, 4> rays;
    
    for (int d = 0; d < 4; d++)
        rays[d] = buildRayMask(index, DIRECTIONS_VECTORS[d]);

    return rays;
}

/**
 * @brief build the lookup table of rayMasks for each squares on the board
 * 
 * @return std::array<std::array<bitboard, 4>, BOARD_SIZE> lookup table of rayMasks for each squares on the board
 */
std::array<std::array<bitboard, 4>, BOARD_SIZE> buildSquareRayLookup() {
    std::array<std::array<bitboard, 4>, BOARD_SIZE> squareRays;

    for (size_t i = 0; i < BOARD_SIZE; i++)
        squareRays[i] = computeRaysFromSquare(i);

    return squareRays;
}

inline const std::array<std::array<bitboard, 4>, BOARD_SIZE> SQUARE_RAYS = buildSquareRayLookup();
