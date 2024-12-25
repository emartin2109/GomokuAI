/*
** EPITECH PROJECT, 2024
** B-AIA-500-BDX-5-1-gomoku-eliot1.martin
** File description:
** Board
*/

#include "Board.hpp"

std::ostream& operator<<(std::ostream& os, const Board& board) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if ((board.bitboardSelf)[i * BOARD_SIZE + j])
                os << "x";
            else if ((board.bitboardOpponent)[i * BOARD_SIZE + j])
                os << "o";
            else
                os << "_";
        }
        os << std::endl;
    }
    return os;
}
