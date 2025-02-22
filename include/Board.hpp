/*
** EPITECH PROJECT, 2024
** B-AIA-500-BDX-5-1-gomoku-eliot1.martin
** File description:
** Board
*/

#pragma once

#include <algorithm>
#include <array>
#include <bitset>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>
#include <ostream>
#include <sys/types.h>
#include <tuple>
#include <utility>
#include <vector>

#include "Type.hpp"
#include "Precompute/TranspositionTables.hpp"
#include "Precompute/SquareLimit.hpp"

class Board {
    public:
        Board(std::bitset<400> &board, std::bitset<400> &opBoard, std::bitset<400> threads, std::bitset<400> opThreads);

    public:
        void makeMove(uint16_t i) {
            this->placeStone(i);
            this->swapBoard();
            movesPlayed.push_back(i);
        }

        void undoMove() {
            if (movesPlayed.empty()) return;

            this->delStone(movesPlayed.back());
            this->swapBoard();
            movesPlayed.pop_back();
        }

        [[nodiscard]] std::vector<uint16_t> getAllMoves() const;

        GameState isGameWon();

        double computeStaticEvaluation();

        void populateThreatBoards() { bitboardSelfThreats.reset(); bitboardOpponentThreats.reset(); }

        [[nodiscard]] std::bitset<400> getCompleteBitboard() const { return bitboardSelf | bitboardOpponent; }

        void updateForcedMovesBoard(uint16_t &i, ThreatPatern &threatPatern, uint8_t &threatIndex);

        void updateForcedMovesOpBoard(uint16_t &i, ThreatPatern &threatPatern, uint8_t &threatIndex);

        void placeStone(uint16_t i);

        void placeOpStone(uint16_t i);

    public:
        friend std::ostream& operator<<(std::ostream& os, const Board& board);

    private:

        void delSelfStone(uint16_t i);

        void delStone(uint16_t i);

        void swapBoard() {
            std::swap(bitboardSelf, bitboardOpponent);
            std::swap(bitboardSelf45, bitboardOpponent45);
            std::swap(bitboardSelf90, bitboardOpponent90);
            std::swap(bitboardSelf135, bitboardOpponent135);
            std::swap(bitboardSelfThreats, bitboardOpponentThreats);

            playerNbr = !playerNbr;
        }

        static std::bitset<400> transposeBitmap(const std::bitset<400>& bits, const std::vector<uint16_t>& transpositionTable) {
            std::bitset<400> rotated;

            for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++)
                rotated[i] = bits[transpositionTable[i]];

            return rotated;
        }

        static std::vector<uint16_t> invertTranspositionTable(std::vector<uint16_t> originalTable) {
            std::vector<uint16_t> newTable;
            newTable.resize(400);

            for (int i = 0; i < 400; ++i) {
                int originalIndex = originalTable[i];
                newTable[originalIndex] = i;
            }

            return newTable;
        }

        static bool checkForWin(std::bitset<400> &board, int startPos, int endPos) {
            for (startPos = startPos; startPos <= endPos; startPos++)
                if (board[startPos] && board[startPos + 1] && board[startPos + 2] && board[startPos + 3] && board[startPos + 4]) return true;

            return false;
        }

        bool checkForThreatPatern(const std::bitset<400> &boardSelf, const std::bitset<400> &boardBoth, int startPos, int endPos, const ThreatPatern &patern, const size_t &tps, const std::vector<uint16_t> &trans, Dir dir, uint8_t &paternId, bool threatOwner);


    public:
        std::array<uint8_t, 400> activationZone;

        std::bitset<400> bitboardSelf;
        std::bitset<400> bitboardOpponent;

        std::bitset<400> bitboardSelf45;
        std::bitset<400> bitboardOpponent45;

        std::bitset<400> bitboardSelf90;
        std::bitset<400> bitboardOpponent90;

        std::bitset<400> bitboardSelf135;
        std::bitset<400> bitboardOpponent135;

        std::bitset<400> bitboardSelfThreats;
        std::bitset<400> bitboardOpponentThreats;

        int evalNbr = 0;
        std::vector<uint16_t> movesPlayed;

    private:
        // start pos
        bool playerNbr = false;
        std::array<std::array<std::shared_ptr<ThreatPatern>, 4>, 400> threatsVectorP1;
        std::array<std::array<std::shared_ptr<ThreatPatern>, 4>, 400> threatsVectorP2;
        std::vector<std::tuple<std::shared_ptr<ThreatPatern>, std::vector<uint16_t>>> activeThreatsP1;
        std::vector<std::tuple<std::shared_ptr<ThreatPatern>, std::vector<uint16_t>>> activeThreatsP2;

        std::vector<uint16_t> potForced;

        std::vector<uint16_t> invertedTransposition45;
        std::vector<uint16_t> invertedTransposition90;
        std::vector<uint16_t> invertedTransposition135;

        std::array<ThreatPatern, PATERN_NBR> threatPaterns {{
            {{ThreatSquare::Occupied, ThreatSquare::Occupied, ThreatSquare::Occupied, ThreatSquare::Occupied, ThreatSquare::Occupied}, ThreatType::Winning, 16, "(5, 1) five", 0},

            {{ThreatSquare::Defense, ThreatSquare::Occupied, ThreatSquare::Occupied, ThreatSquare::Occupied, ThreatSquare::Occupied, ThreatSquare::Defense}, ThreatType::Winning, 15, "(4, 2) open four", 0},
            {{ThreatSquare::Occupied, ThreatSquare::Occupied, ThreatSquare::Defense, ThreatSquare::Occupied, ThreatSquare::Occupied, ThreatSquare::Defense, ThreatSquare::Occupied, ThreatSquare::Occupied}, ThreatType::Winning, 15, "(4, 2) (2) open four", 0},
            {{ThreatSquare::Occupied, ThreatSquare::Occupied, ThreatSquare::Occupied, ThreatSquare::Defense, ThreatSquare::Occupied, ThreatSquare::Defense, ThreatSquare::Occupied, ThreatSquare::Occupied, ThreatSquare::Occupied}, ThreatType::Winning, 15, "(4, 2) (3) open four", 0},

            {{ThreatSquare::Defense, ThreatSquare::Occupied, ThreatSquare::Occupied, ThreatSquare::Occupied, ThreatSquare::Occupied}, ThreatType::Forcing, 14, "(4, 1) simple four", 0},
            {{ThreatSquare::Occupied, ThreatSquare::Occupied, ThreatSquare::Occupied, ThreatSquare::Occupied, ThreatSquare::Defense}, ThreatType::Forcing, 14, "(4, 1) (r) simple four", 0},
            {{ThreatSquare::Occupied, ThreatSquare::Os, ThreatSquare::Os, ThreatSquare::Os, ThreatSquare::Occupied}, ThreatType::Forcing, 14, "(4, 1) (2) simple four", 1},

            {{ThreatSquare::Empty, ThreatSquare::Defense, ThreatSquare::Occupied, ThreatSquare::Occupied, ThreatSquare::Occupied, ThreatSquare::Defense, ThreatSquare::Empty}, ThreatType::Forcing, 13, "(3, 3) open three", 0},
            {{ThreatSquare::Defense, ThreatSquare::Occupied, ThreatSquare::Occupied, ThreatSquare::Occupied, ThreatSquare::Defense, ThreatSquare::Empty}, ThreatType::Forcing, 13, "(3, 3) (r) open three", 0},
            {{ThreatSquare::Empty, ThreatSquare::Occupied, ThreatSquare::Defense, ThreatSquare::Occupied, ThreatSquare::Occupied, ThreatSquare::Defense, ThreatSquare::Occupied, ThreatSquare::Empty}, ThreatType::Forcing, 13, "(3, 3) (2) open three", 0},
            {{ThreatSquare::Occupied, ThreatSquare::Defense, ThreatSquare::Occupied, ThreatSquare::Defense, ThreatSquare::Occupied, ThreatSquare::Defense, ThreatSquare::Occupied, ThreatSquare::Empty, ThreatSquare::Occupied}, ThreatType::Forcing, 13, "(3, 3) (3) open three", 0},

            {{ThreatSquare::Defense, ThreatSquare::Occupied, ThreatSquare::Os, ThreatSquare::Os, ThreatSquare::Occupied, ThreatSquare::Defense}, ThreatType::Forcing, 12, "(3, 2) broken three", 1},
            {{ThreatSquare::Defense, ThreatSquare::Occupied, ThreatSquare::Occupied, ThreatSquare::Occupied, ThreatSquare::Defense, ThreatSquare::Empty}, ThreatType::Forcing, 12, "(3, 2) (2) broken three", 0},
            {{ThreatSquare::Empty, ThreatSquare::Defense, ThreatSquare::Occupied, ThreatSquare::Occupied, ThreatSquare::Occupied, ThreatSquare::Defense}, ThreatType::Forcing, 12, "(3, 2) (2) (r) broken three", 0},

            {{ThreatSquare::Occupied, ThreatSquare::Occupied, ThreatSquare::Occupied, ThreatSquare::Empty, ThreatSquare::Empty}, ThreatType::NonForcing, 11, "(3, 1) simple three", 0},
            {{ThreatSquare::Empty, ThreatSquare::Empty, ThreatSquare::Occupied, ThreatSquare::Occupied, ThreatSquare::Occupied}, ThreatType::NonForcing, 11, "(3, 1) (r) simple three", 0},

            {{ThreatSquare::Empty, ThreatSquare::Empty, ThreatSquare::Empty, ThreatSquare::Occupied, ThreatSquare::Occupied, ThreatSquare::Empty, ThreatSquare::Empty, ThreatSquare::Empty}, ThreatType::NonForcing, 10, "(2, 4) two", 0},

            {{ThreatSquare::Empty, ThreatSquare::Empty, ThreatSquare::Osn, ThreatSquare::Osn, ThreatSquare::Osn, ThreatSquare::Empty, ThreatSquare::Empty}, ThreatType::NonForcing, 9, "(2, 3) two", 1},

            {{ThreatSquare::Empty, ThreatSquare::Osn, ThreatSquare::Osn, ThreatSquare::Osn, ThreatSquare::Osn, ThreatSquare::Empty}, ThreatType::NonForcing, 8, "(2, 2) two", 2},

            {{ThreatSquare::Osn, ThreatSquare::Osn, ThreatSquare::Osn, ThreatSquare::Osn, ThreatSquare::Osn}, ThreatType::NonForcing, 7, "(2, 1) two", 3},
        }};

        std::array<SquareLimit, 400> precomputedSquaresLimits;

        bool m_paternFoundHorizontal = false;
        bool m_paternFoundVertical = false;
        bool m_paternFoundDiagonal1 = false;
        bool m_paternFoundDiagonal2 = false;
};
