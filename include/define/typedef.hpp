#pragma once

#include <bitset>

#include "Config.hpp"

typedef std::bitset<BOARD_SIZE> bitboard;
typedef std::bitset<9> threat_patern;
typedef std::bitset<11> encoded_threat_pattern;

typedef std::array<std::array<u_int16_t, BOARD_SIZE>, 2> linearization_table;
