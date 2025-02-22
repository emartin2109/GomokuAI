/*
** EPITECH PROJECT, 2024
** B-AIA-500-BDX-5-1-gomoku-eliot1.martin
** File description:
** Type
*/

#pragma once

#include <cfloat>
#include <cstdint>
#include <string>
#include <vector>

#define BOARD_SIZE 20

#define PATERN_NBR 20
#define DEFENSIVENESS 2.0

#define TIME_PER_MOVE 4.5

const int DIRECTION[4][2] = { {0, 1}, {1, 0}, {1, 1}, {1, -1} };

enum class Piece {
    Black,
    White,
    None
};

enum class GameState {
    Won,
    Lost,
    Draw,
    InProgress
};

struct MoveInfo {
    int x, y;
    Piece piece;

    int score;

    int MatIn = -1;
};

enum Dir: uint8_t {
    Vertical = 0,
    Horizontal = 1,
    Diagonal1 = 2,
    Diagonal2 = 3
};

enum class ThreatType {
    Winning,
    Forcing,
    NonForcing
};

enum class ThreatSquare {
    Occupied,
    Empty,
    Defense,
    Os,
    Osn
};

struct ThreatPatern {
    std::vector<ThreatSquare> patern;
    ThreatType type;
    unsigned short severity;
    std::string name;
    uint8_t swapQO;
};
