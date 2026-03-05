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
#include "Precomputed.hpp"

class SquareLimit {
    public:
        SquareLimit(size_t boardSize, int squareIndex) : 
            lowerXLimit(squareIndex - squareIndex % 20), upperXLimit(((squareIndex + boardSize) - (squareIndex + boardSize) % 20) - 1),
            lowerYLimit(squareIndex % 20), upperYLimit((boardSize * (boardSize - 1)) + (squareIndex % boardSize)) {

                lowerPrimaryDiagLimit = squareIndex - std::min((squareIndex - lowerXLimit) % 20, (squareIndex - lowerYLimit) / 20) * (boardSize + 1);
                upperPrimaryDiagLimit = squareIndex + std::min((upperXLimit - squareIndex) % 20, (upperYLimit - squareIndex) / 20) * (boardSize + 1);

                lowerSecondaryDiagLimit = squareIndex - std::min((upperXLimit - squareIndex) % 20, (squareIndex - lowerYLimit) / 20) * (boardSize - 1);
                upperSecondaryDiagLimit = squareIndex + std::min((squareIndex - lowerXLimit) % 20, (upperYLimit - squareIndex) / 20) * (boardSize - 1);
        }

        SquareLimit() = default;
    
    public:
        int lowerXLimit = 0;
        int upperXLimit = 0;

        int lowerYLimit = 0;
        int upperYLimit = 0;

        int lowerPrimaryDiagLimit = 0;
        int upperPrimaryDiagLimit = 0;

        int lowerSecondaryDiagLimit = 0;
        int upperSecondaryDiagLimit = 0;
};

class Board {
    public:
        Board(std::bitset<400> &board, std::bitset<400> &opBoard, std::bitset<400> threads, std::bitset<400> opThreads) :
            bitboardSelf(board), bitboardOpponent(opBoard),
            bitboardSelfThreats(threads), bitboardOpponentThreats(opThreads),
                invertedTransposition45(invertTranspositionTable(TRANSPOTITION_45)),
                invertedTransposition90(invertTranspositionTable(TRANSPOTITION_90)),
                invertedTransposition135(invertTranspositionTable(TRANSPOTITION_135)) {

                    bitboardSelf45 = transposeBitmap(bitboardSelf, TRANSPOTITION_45);
                    bitboardSelf90 = transposeBitmap(bitboardSelf, TRANSPOTITION_90);
                    bitboardSelf135 = transposeBitmap(bitboardSelf, TRANSPOTITION_135);

                    bitboardOpponent45 = transposeBitmap(bitboardOpponent, TRANSPOTITION_45);
                    bitboardOpponent90 = transposeBitmap(bitboardOpponent, TRANSPOTITION_90);
                    bitboardOpponent135 = transposeBitmap(bitboardOpponent, TRANSPOTITION_135);

                    for (int i = 0; i < 400; i++) {
                        precomputedSquaresLimits[i] = SquareLimit(BOARD_SIZE, i);
                        activationZone[i] = 0;
                    }

                    potForced.reserve(5);
        }

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

        [[nodiscard]] std::vector<uint16_t> getAllMoves() const {
            bool forcedMovesDetected = false;
            std::vector<uint16_t> playablesMoves;
            std::bitset<400> both = getCompleteBitboard();

            for (std::tuple<std::shared_ptr<ThreatPatern>, std::vector<uint16_t>> threat : activeThreatsP1) {
                for (uint16_t move : std::get<1>(threat)) {
                    if (!both[move]) {
                        playablesMoves.push_back(move);
                        forcedMovesDetected = true;
                    }
                }
            }

            for (std::tuple<std::shared_ptr<ThreatPatern>, std::vector<uint16_t>> threat : activeThreatsP2) {
                for (uint16_t move : std::get<1>(threat)) {
                    if (!both[move]) {
                        playablesMoves.push_back(move);
                        forcedMovesDetected = true;
                    }
                }
            }

            if (!forcedMovesDetected) {
                for (uint16_t i = 0; i < 400; i++)
                    if (!both[i] && activationZone[i] > 0) playablesMoves.push_back(i);
            }

            return playablesMoves;
                
        }

        GameState isGameWon() {
            int lastMove = movesPlayed.back();

            SquareLimit limiter = precomputedSquaresLimits[lastMove];

            int startCheck = std::max(lastMove - 4, limiter.lowerXLimit);
            int endCheck = std::min(lastMove, limiter.upperXLimit - 4);
            if (checkForWin(bitboardOpponent, startCheck, endCheck)) return GameState::Lost;

            startCheck = std::max(invertedTransposition45[lastMove] - 4, int(invertedTransposition45[limiter.upperSecondaryDiagLimit]));
            endCheck = std::min(int(invertedTransposition45[lastMove]), invertedTransposition45[limiter.lowerSecondaryDiagLimit] - 4);
            if (checkForWin(bitboardOpponent45, startCheck, endCheck)) return GameState::Lost;

            startCheck = std::max(invertedTransposition90[lastMove] - 4, int(invertedTransposition90[limiter.upperYLimit]));
            endCheck = std::min(int(invertedTransposition90[lastMove]), invertedTransposition90[limiter.lowerYLimit] - 4);
            if (checkForWin(bitboardOpponent90, startCheck, endCheck)) return GameState::Lost;

            startCheck = std::max(invertedTransposition135[lastMove] - 4, int(invertedTransposition135[limiter.upperPrimaryDiagLimit]));
            endCheck = std::min(int(invertedTransposition135[lastMove]), invertedTransposition135[limiter.lowerPrimaryDiagLimit] - 4);
            if (checkForWin(bitboardOpponent135, startCheck, endCheck)) return GameState::Lost;

            return GameState::InProgress;
        }

        double computeStaticEvaluation() {
            double result = 0.0;

            if (playerNbr) {
                for (std::tuple<std::shared_ptr<ThreatPatern>, std::vector<uint16_t>> threat : activeThreatsP1)
                    result += std::pow(1.8, std::get<0>(threat)->severity);
                for (std::tuple<std::shared_ptr<ThreatPatern>, std::vector<uint16_t>> threat : activeThreatsP2)
                    result -= DEFENSIVENESS * (std::pow(1.8, std::get<0>(threat)->severity));
            } else {
                for (std::tuple<std::shared_ptr<ThreatPatern>, std::vector<uint16_t>> threat : activeThreatsP1)
                    result -= DEFENSIVENESS * (std::pow(1.8, std::get<0>(threat)->severity));
                for (std::tuple<std::shared_ptr<ThreatPatern>, std::vector<uint16_t>> threat : activeThreatsP2)
                    result += std::pow(1.8, std::get<0>(threat)->severity);
            }

            evalNbr++;

            return result;
        }

        void populateThreatBoards() { bitboardSelfThreats.reset(); bitboardOpponentThreats.reset(); }

        [[nodiscard]] std::bitset<400> getCompleteBitboard() const { return bitboardSelf | bitboardOpponent; }

        void updateForcedMovesBoard(uint16_t &i, ThreatPatern &threatPatern, uint8_t &threatIndex) {
            const SquareLimit &limiter = precomputedSquaresLimits[i];
            size_t tps = threatPatern.patern.size() - 1;

            int startCheck = std::max(static_cast<int>(i - tps), limiter.lowerXLimit);
            int endCheck = std::min(static_cast<int>(i), limiter.upperXLimit);

            if (!m_paternFoundHorizontal) {
                if (checkForThreatPatern(bitboardSelf, bitboardSelf | bitboardOpponent, startCheck, endCheck, threatPatern, tps, TRANSPOTITION_0, Dir::Horizontal, threatIndex, playerNbr))
                    m_paternFoundHorizontal = true;
            }

            if (!m_paternFoundDiagonal1) {
                startCheck = std::max(static_cast<int>(invertedTransposition45[i] - tps), static_cast<int>(invertedTransposition45[limiter.upperSecondaryDiagLimit]));
                endCheck = std::min(static_cast<int>(invertedTransposition45[i]), static_cast<int>(invertedTransposition45[limiter.lowerSecondaryDiagLimit] - tps));
                if (checkForThreatPatern(bitboardSelf45, bitboardSelf45 | bitboardOpponent45, startCheck, endCheck, threatPatern, tps, TRANSPOTITION_45, Dir::Diagonal1, threatIndex, playerNbr))
                    m_paternFoundDiagonal1 = true;
            }
            if (!m_paternFoundVertical) {
                startCheck = std::max(static_cast<int>(invertedTransposition90[i] - tps), static_cast<int>(invertedTransposition90[limiter.upperYLimit]));
                endCheck = std::min(static_cast<int>(invertedTransposition90[i]), static_cast<int>(invertedTransposition90[limiter.lowerYLimit] - tps));
                if (checkForThreatPatern(bitboardSelf90, bitboardSelf90 | bitboardOpponent90, startCheck, endCheck, threatPatern, tps, TRANSPOTITION_90, Dir::Vertical, threatIndex, playerNbr))
                    m_paternFoundVertical = true;
            }

            if (!m_paternFoundDiagonal2) {
                startCheck = std::max(static_cast<int>(invertedTransposition135[i] - tps), static_cast<int>(invertedTransposition135[limiter.upperPrimaryDiagLimit]));
                endCheck = std::min(static_cast<int>(invertedTransposition135[i]), static_cast<int>(invertedTransposition135[limiter.lowerPrimaryDiagLimit] - tps));
                if (checkForThreatPatern(bitboardSelf135, bitboardSelf135 | bitboardOpponent135, startCheck, endCheck, threatPatern, tps, TRANSPOTITION_135, Dir::Diagonal2, threatIndex, playerNbr))
                    m_paternFoundDiagonal2 = true;
            }
        }

        void updateForcedMovesOpBoard(uint16_t &i, ThreatPatern &threatPatern, uint8_t &threatIndex) {
            SquareLimit limiter = precomputedSquaresLimits[i];
            size_t tps = threatPatern.patern.size() - 1;

            int startCheck = std::max(static_cast<int>(i - tps), limiter.lowerXLimit);
            int endCheck = std::min(static_cast<int>(i), limiter.upperXLimit);

            if (!m_paternFoundHorizontal) {
                if (checkForThreatPatern(bitboardOpponent, bitboardSelf | bitboardOpponent, startCheck, endCheck, threatPatern, tps, TRANSPOTITION_0, Dir::Horizontal, threatIndex, !playerNbr))
                    m_paternFoundHorizontal = true;
            }

            if (!m_paternFoundDiagonal1) {
                startCheck = std::max(static_cast<int>(invertedTransposition45[i] - tps), static_cast<int>(invertedTransposition45[limiter.upperSecondaryDiagLimit]));
                endCheck = std::min(static_cast<int>(invertedTransposition45[i]), static_cast<int>(invertedTransposition45[limiter.lowerSecondaryDiagLimit] - tps));
                if (checkForThreatPatern(bitboardOpponent45, bitboardSelf45 | bitboardOpponent45, startCheck, endCheck, threatPatern, tps, TRANSPOTITION_45, Dir::Diagonal1, threatIndex, !playerNbr))
                    m_paternFoundDiagonal1 = true;
            }
            if (!m_paternFoundVertical) {
                startCheck = std::max(static_cast<int>(invertedTransposition90[i] - tps), static_cast<int>(invertedTransposition90[limiter.upperYLimit]));
                endCheck = std::min(static_cast<int>(invertedTransposition90[i]), static_cast<int>(invertedTransposition90[limiter.lowerYLimit] - tps));
                if (checkForThreatPatern(bitboardOpponent90, bitboardSelf90 | bitboardOpponent90, startCheck, endCheck, threatPatern, tps, TRANSPOTITION_90, Dir::Vertical, threatIndex, !playerNbr))
                    m_paternFoundVertical = true;
            }

            if (!m_paternFoundDiagonal2) {
                startCheck = std::max(static_cast<int>(invertedTransposition135[i] - tps), static_cast<int>(invertedTransposition135[limiter.upperPrimaryDiagLimit]));
                endCheck = std::min(static_cast<int>(invertedTransposition135[i]), static_cast<int>(invertedTransposition135[limiter.lowerPrimaryDiagLimit] - tps));
                if (checkForThreatPatern(bitboardOpponent135, bitboardSelf135 | bitboardOpponent135, startCheck, endCheck, threatPatern, tps, TRANSPOTITION_135, Dir::Diagonal2, threatIndex, !playerNbr))
                    m_paternFoundDiagonal2 = true;
            }
        }
    
        void placeStone(uint16_t i) {
            // set the activation zone
            for (uint16_t y = std::max(i - (2 * BOARD_SIZE), precomputedSquaresLimits[i].lowerYLimit); y < std::min(i + (2 * BOARD_SIZE), precomputedSquaresLimits[i].upperYLimit); y++) {
                for (uint16_t x = std::max(i - 2, precomputedSquaresLimits[i].lowerXLimit); x < std::min(i + 2, precomputedSquaresLimits[i].upperXLimit); x++) {
                    activationZone[(y - (y % BOARD_SIZE)) + (x % BOARD_SIZE)] += 1;
                }
            }

            bitboardSelf.set(i);
            bitboardSelf45.set(invertedTransposition45[i]);
            bitboardSelf90.set(invertedTransposition90[i]);
            bitboardSelf135.set(invertedTransposition135[i]);

            for (uint8_t z = 0; static_cast<size_t>(z) < PATERN_NBR; ++z) {
                updateForcedMovesBoard(i, threatPaterns[z], z);
                updateForcedMovesOpBoard(i, threatPaterns[z], z);
            }

            m_paternFoundVertical = false;
            m_paternFoundHorizontal = false;
            m_paternFoundDiagonal1 = false;
            m_paternFoundDiagonal2 = false;
        }

        void placeOpStone(uint16_t i) {
            // set the activation zone
            for (uint16_t y = std::max(i - (2 * BOARD_SIZE), precomputedSquaresLimits[i].lowerYLimit); y < std::min(i + (2 * BOARD_SIZE), precomputedSquaresLimits[i].upperYLimit); y++) {
                for (uint16_t x = std::max(i - 2, precomputedSquaresLimits[i].lowerXLimit); x < std::min(i + 2, precomputedSquaresLimits[i].upperXLimit); x++) {
                    activationZone[(y - (y % BOARD_SIZE)) + (x % BOARD_SIZE)] += 1;
                }
            }

            bitboardOpponent.set(i);
            bitboardOpponent45.set(invertedTransposition45[i]);
            bitboardOpponent90.set(invertedTransposition90[i]);
            bitboardOpponent135.set(invertedTransposition135[i]);

            for (uint8_t z = 0; static_cast<size_t>(z) < threatPaterns.size(); ++z) {
                updateForcedMovesBoard(i, threatPaterns[z], z);
                updateForcedMovesOpBoard(i, threatPaterns[z], z);
            }

            m_paternFoundVertical = false;
            m_paternFoundHorizontal = false;
            m_paternFoundDiagonal1 = false;
            m_paternFoundDiagonal2 = false;
        }

    public:
        friend std::ostream& operator<<(std::ostream& os, const Board& board);

    private:

        void delSelfStone(uint16_t i) {
            for (uint16_t y = std::max(i - (2 * BOARD_SIZE), precomputedSquaresLimits[i].lowerYLimit); y < std::min(i + (2 * BOARD_SIZE), precomputedSquaresLimits[i].upperYLimit); y++) {
                for (uint16_t x = std::max(i - 2, precomputedSquaresLimits[i].lowerXLimit); x < std::min(i + 2, precomputedSquaresLimits[i].upperXLimit); x++) {
                    activationZone[(y - (y % BOARD_SIZE)) + (x % BOARD_SIZE)] -= 1;
                }
            }

            bitboardSelf.reset(i);
            bitboardSelf45.reset(invertedTransposition45[i]);
            bitboardSelf90.reset(invertedTransposition90[i]);
            bitboardSelf135.reset(invertedTransposition135[i]);

            for (uint8_t z = 0; static_cast<size_t>(z) < threatPaterns.size(); ++z) {
                updateForcedMovesBoard(i, threatPaterns[z], z);
                updateForcedMovesOpBoard(i, threatPaterns[z], z);
            }

            m_paternFoundVertical = false;
            m_paternFoundHorizontal = false;
            m_paternFoundDiagonal1 = false;
            m_paternFoundDiagonal2 = false;
        }

        void delStone(uint16_t i) {
            for (uint16_t y = std::max(i - (2 * BOARD_SIZE), precomputedSquaresLimits[i].lowerYLimit); y < std::min(i + (2 * BOARD_SIZE), precomputedSquaresLimits[i].upperYLimit); y++) {
                for (uint16_t x = std::max(i - 2, precomputedSquaresLimits[i].lowerXLimit); x < std::min(i + 2, precomputedSquaresLimits[i].upperXLimit); x++) {
                    activationZone[(y - (y % BOARD_SIZE)) + (x % BOARD_SIZE)] -= 1;
                }
            }

            bitboardOpponent.reset(i);
            bitboardOpponent45.reset(invertedTransposition45[i]);
            bitboardOpponent90.reset(invertedTransposition90[i]);
            bitboardOpponent135.reset(invertedTransposition135[i]);

            for (uint8_t z = 0; static_cast<size_t>(z) < threatPaterns.size(); ++z) {
                updateForcedMovesBoard(i, threatPaterns[z], z);
                updateForcedMovesOpBoard(i, threatPaterns[z], z);
            }

            m_paternFoundVertical = false;
            m_paternFoundHorizontal = false;
            m_paternFoundDiagonal1 = false;
            m_paternFoundDiagonal2 = false;
        }

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
            // Construct a new transposition table
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

        bool checkForThreatPatern(const std::bitset<400> &boardSelf, const std::bitset<400> &boardBoth, int startPos, int endPos, const ThreatPatern &patern, const size_t &tps, const std::vector<uint16_t> &trans, Dir dir, uint8_t &paternId, bool threatOwner) {
            for (startPos = startPos; startPos <= endPos; startPos++) {
                uint8_t QO = patern.swapQO;
                bool paternCorrect = true;

                for (size_t i = 0; i < tps + 1; i++) {
                    if (patern.patern[i] == ThreatSquare::Occupied && !boardSelf[startPos + i]) {
                        paternCorrect = false;
                        potForced.clear();
                        break;
                    }

                    if (patern.patern[i] == ThreatSquare::Empty && boardBoth[startPos + i]) {
                        paternCorrect = false;
                        potForced.clear();
                        break;
                    }

                    if (patern.patern[i] == ThreatSquare::Defense) {
                        if (boardBoth[startPos + i]) {
                            paternCorrect = false;
                            potForced.clear();
                            break;
                        }
                        potForced.push_back(trans[startPos + i]);
                    }

                    else if (patern.patern[i] == ThreatSquare::Os && !boardSelf[startPos + i]) {
                        if (QO == 0 || boardBoth[startPos + i]) {
                            paternCorrect = false;
                            potForced.clear();
                            break;
                        }
                        QO--; 
                        potForced.push_back(trans[startPos + i]);
                    }

                    else if (patern.patern[i] == ThreatSquare::Osn && !boardSelf[startPos + i]) {
                        if (QO == 0 || boardBoth[startPos + i]) {
                            paternCorrect = false;
                            potForced.clear();
                            break;
                        }
                        QO--;
                    }
                }

                if (paternCorrect) {
                    std::shared_ptr<ThreatPatern> newPatern = std::make_shared<ThreatPatern>(threatPaterns[paternId]);
                    if (threatOwner) {
                        const std::shared_ptr<ThreatPatern> oldPatren = threatsVectorP1[startPos][dir];
                        threatsVectorP1[startPos][dir] = newPatern;
                        for (uint16_t i = 0; i < static_cast<uint16_t>(activeThreatsP1.size()); i++) {
                            if (std::get<0>(activeThreatsP1[i]) == oldPatren) {
                                activeThreatsP1.erase(activeThreatsP1.begin() + i);
                                break;
                            }
                        }
                        activeThreatsP1.emplace_back(newPatern, potForced);
                    } else {
                        const std::shared_ptr<ThreatPatern> oldPatren = threatsVectorP2[startPos][dir];
                        threatsVectorP2[startPos][dir] = newPatern;
                        for (uint16_t i = 0; i < static_cast<uint16_t>(activeThreatsP2.size()); i++) {
                            if (std::get<0>(activeThreatsP2[i]) == oldPatren) {
                                activeThreatsP2.erase(activeThreatsP2.begin() + i);
                                break;
                            }
                        }
                        activeThreatsP2.emplace_back(newPatern, potForced);
                    }
                    return true;
                }

                if (threatOwner) {
                    const std::shared_ptr<ThreatPatern> oldPatren = threatsVectorP1[startPos][dir];
                    if (oldPatren != nullptr) {
                        threatsVectorP1[startPos][dir] = nullptr;
                        for (uint16_t i = 0; i < static_cast<uint16_t>(activeThreatsP1.size()); i++) {
                            if (std::get<0>(activeThreatsP1[i]) == oldPatren) {
                                activeThreatsP1.erase(activeThreatsP1.begin() + i);
                                break;
                            }
                        }
                    }
                } else {
                    const std::shared_ptr<ThreatPatern> oldPatren = threatsVectorP2[startPos][dir];
                    if (oldPatren != nullptr) {
                        threatsVectorP2[startPos][dir] = nullptr;
                        for (uint16_t i = 0; i < static_cast<uint16_t>(activeThreatsP2.size()); i++) {
                            if (std::get<0>(activeThreatsP2[i]) == oldPatren) {
                                activeThreatsP2.erase(activeThreatsP2.begin() + i);
                                break;
                            }
                        }
                    }
                }
            }

            return false;
        }


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
