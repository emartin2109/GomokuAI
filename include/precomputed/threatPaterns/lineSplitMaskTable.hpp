#pragma once

#include <array>

#include "define/typedef.hpp"
#include <cmath>
#include <iostream>

 std::array<std::tuple<threat_patern, encoded_threat_pattern>, 512> buildLineSplitMaskTable() {
     std::array<line_split_masks, 512> lineSplitTable;

    for (size_t i = 0; i < 512; i++) {
        threat_patern lineMask;
        encoded_threat_pattern lineMaskLimits;
        bool opStoneEncountered = false;

        int fbi = -1;
        int lbi = 9;
        
        for (size_t j = 0; j < 4; j++) {
            if (i & static_cast<size_t>(pow(2, 3 - j))) {
                fbi = 3 - j;
                lineMaskLimits.set(3 - j + 1);
                opStoneEncountered = true;
                break;
            }
        }

        if (!opStoneEncountered)
            lineMaskLimits.set(0);

        opStoneEncountered = false;

        for (size_t j = 0; j < 4; j++) {
            if (i & static_cast<size_t>(pow(2, 5 + j))) {
                lbi = 5 + j;
                lineMaskLimits.set(5 + j + 1);
                opStoneEncountered = true;
                continue;
            }
        }

        if (!opStoneEncountered)
            lineMaskLimits.set(10);

        for (int j = fbi + 1; j < lbi; j++) {
            lineMask.set(j);
        }

        lineSplitTable[i] = {lineMask, lineMaskLimits};
    }

    return lineSplitTable;
}

inline const std::array<line_split_masks, 512> LINE_SPLIT_TABLE = buildLineSplitMaskTable();
