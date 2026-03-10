#pragma once

#include <array>

#include "define/typedef.hpp"
#include <cmath>
#include <iostream>

std::array<threat_patern, 512> buildLineSplitMaskLookup() {
    std::array<threat_patern, 512> lineSplitMask;

    for (size_t i = 0; i < 512; i++) {
        threat_patern lineMask(0);
        int fbi = -1;
        int lbi = 9;
        
        for (size_t j = 0; j < 4; j++) {
            if (i & static_cast<size_t>(pow(2, 3 - j))) {
                fbi = 3 - j;
                break;;
            }
        }
        for (size_t j = 0; j < 4; j++) {
            if (i & static_cast<size_t>(pow(2, 5 + j))) {
                lbi = 5 + j;
                continue;
            }
        }

        std::cout << fbi << " " << lbi << std::endl;

        for (int j = fbi + 1; j < lbi; j++) {
            lineMask.set(j);
        }

        lineSplitMask[i] = lineMask;
    }

    return lineSplitMask;
}

inline const std::array<threat_patern, 512> LINE_SPLIT_MASK = buildLineSplitMaskLookup();