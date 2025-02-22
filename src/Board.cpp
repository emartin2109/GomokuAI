/*
** EPITECH PROJECT, 2024
** B-AIA-500-BDX-5-1-gomoku-eliot1.martin
** File description:
** Board
*/

#include "Board.hpp"

Board::Board(std::bitset<400> &board, std::bitset<400> &opBoard, std::bitset<400> threads, std::bitset<400> opThreads) :
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

[[nodiscard]] std::vector<uint16_t> Board::getAllMoves() const {
    bool forcedMovesDetected = false;
    std::vector<uint16_t> playablesMoves;
    std::bitset<400> both = getCompleteBitboard();

    std::vector<std::tuple<std::shared_ptr<ThreatPatern>, std::vector<uint16_t>>> activeThreats;
    activeThreats.insert(activeThreats.end(), activeThreatsP1.begin(), activeThreatsP1.end());
    activeThreats.insert(activeThreats.end(), activeThreatsP2.begin(), activeThreatsP2.end());

    for (std::tuple<std::shared_ptr<ThreatPatern>, std::vector<uint16_t>> threat : activeThreats)
        for (uint16_t move : std::get<1>(threat))
            if (!both[move]) {
                playablesMoves.push_back(move);
                forcedMovesDetected = true;
            }

    if (!forcedMovesDetected)
        for (uint16_t i = 0; i < 400; i++)
            if (!both[i] && activationZone[i] > 0) playablesMoves.push_back(i);

    return playablesMoves; 
}

GameState Board::isGameWon() {
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

double Board::computeStaticEvaluation() {
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

void Board::updateForcedMovesBoard(uint16_t &i, ThreatPatern &threatPatern, uint8_t &threatIndex) {
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

void Board::updateForcedMovesOpBoard(uint16_t &i, ThreatPatern &threatPatern, uint8_t &threatIndex) {
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

void Board::placeStone(uint16_t i) {
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

void Board::placeOpStone(uint16_t i) {
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

void Board::delSelfStone(uint16_t i) {
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

void Board::delStone(uint16_t i) {
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

bool Board::checkForThreatPatern(const std::bitset<400> &boardSelf, const std::bitset<400> &boardBoth, int startPos, int endPos, const ThreatPatern &patern, const size_t &tps, const std::vector<uint16_t> &trans, Dir dir, uint8_t &paternId, bool threatOwner) {
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
