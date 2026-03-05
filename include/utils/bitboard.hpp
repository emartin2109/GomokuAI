#pragma once

#include <iostream>

#include "define/typedef.hpp"
#include "precomputed/values.hpp"

void bitboardPrinter(bitboard board) {
    std::cout << "BOARD" << ":" << std::endl;

    std::cout << BOARD_LENGTH << std::endl;
    for (size_t x = 0; x < BOARD_LENGTH; x++) {
        for (size_t y = 0; y < BOARD_LENGTH; y++) {
            std::cout << board[(BOARD_SIZE - 1) - (x * BOARD_LENGTH + y)];
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
