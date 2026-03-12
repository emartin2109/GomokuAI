#pragma once

#include "Config.hpp"
#include "define/typedef.hpp"
#include "precomputed/values.hpp"

#include <array>
#include <iostream>

std::array<bitboard, BOARD_SIZE> buildProximityMask() {
    std::array<bitboard, BOARD_SIZE> o;

    for (int i = 0; i < BOARD_SIZE; i++) {
        bitboard b;

        for (int y = std::max(static_cast<int>(i / BOARD_LENGTH) - 2, 0); 
        y < std::min(static_cast<int>(i / BOARD_LENGTH) + 3, static_cast<int>(BOARD_LENGTH)); y++) {
            for (int x = std::max(static_cast<int>(i % BOARD_LENGTH) - 2, 0);
            x < std::min(static_cast<int>(i % BOARD_LENGTH) + 3, static_cast<int>(BOARD_LENGTH)); x++) {
                b.set(y * BOARD_LENGTH + x);
            }
        }
        std::cout << std::endl;

        o[i] = b;
    }

    return o;
}

inline const std::array<bitboard, BOARD_SIZE> PROXIMITY_MASK = buildProximityMask();
