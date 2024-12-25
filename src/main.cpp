#include "MinMax.hpp"
#include "Brain.hpp"
#include "Type.hpp"

#include <bitset>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <iostream>
#include <ostream>
#include <vector>

// TODO :: one patern / opti

bool getBit(const std::bitset<400>& bits, int x, int y) {
    return bits[x * BOARD_SIZE + y];
}

// Helper function to set the bit at a given position in the bitset
void setBit(std::bitset<400>& bits, int x, int y, bool value) {
    bits[x * BOARD_SIZE + y] = value;
}

// Function to rotate the bitset 90 degrees
std::bitset<400> rotate90(const std::bitset<400>& bits) {
    std::bitset<400> rotated;

    for (int x = 0; x < BOARD_SIZE; ++x) {
        for (int y = 0; y < BOARD_SIZE; ++y) {
            setBit(rotated, y, BOARD_SIZE - x - 1, getBit(bits, x, y));
        }
    }

    return rotated;
}

std::bitset<400> rotate45(const std::bitset<400>& bits) {
    size_t test = 0;
    size_t rotatedIndex = 0;
    std::bitset<400> rotated;

    size_t diagSize = 0;
    for (int diag = 0; diag < BOARD_SIZE; diag++) {
        diagSize++;
        for (size_t diagIndex = 0; diagIndex < diagSize; diagIndex++) {
            rotated[rotatedIndex++] = bits[((diag - diagIndex) * BOARD_SIZE) + (diagIndex)];
            test++;
        }
    }

    for (int diag = 1; diag < BOARD_SIZE; diag++) {
        diagSize--;
        for (size_t diagIndex = 0; diagIndex < diagSize; diagIndex++) {
            rotated[rotatedIndex++] = bits[((BOARD_SIZE - 1 - (diagIndex)) * BOARD_SIZE)  + diag + diagIndex];
            test++;
        }
    }

    return rotated;
}

std::bitset<400> transpose(const std::bitset<400>& bits, const std::vector<uint16_t>& transpositionTable) {
    std::bitset<400> rotated;

    for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++)
        rotated[i] = bits[transpositionTable[i]];

    return rotated;
}

int main() {
/*
    std::bitset<400> selfBoard{};
    std::bitset<400> oponentBoard{};
    Board board(selfBoard, oponentBoard, std::bitset<400>{}, std::bitset<400>{});

    board.makeMove(19);
    board.makeMove(0);
    board.makeMove(38);
    board.makeMove(1);
    board.makeMove(57);
    board.makeMove(2);
    board.makeMove(76);
//    board.makeMove(3);

    uint16_t bestMove = 0;
    uint16_t maxDepth = 50;
    uint16_t posEval = 0;
    std::vector<uint16_t> PrevMoves;
    std::vector<uint16_t> moves;

    std::chrono::high_resolution_clock::time_point begin = std::chrono::high_resolution_clock::now();
    for (uint16_t i = 1; i <= maxDepth; i++) {
        moves = board.getAllMoves();
        std::tuple<uint16_t, bool> newMove = MinMax::findBestMove(board, i, begin);
        if (std::get<0>(newMove) != 0) {
            bestMove = std::get<0>(newMove);
            posEval = board.evalNbr;
            PrevMoves = moves;
            if (std::get<1>(newMove)) {
                std::cout << "DEBUG Move found at depth " << i << ", " << posEval << " positions evaluated" << std::endl;
                std::cout << "DEBUG Choice made between " << PrevMoves.size() << " moves available" << std::endl; 
                break;
            }
        }
        else {
            std::cout << "DEBUG Move found at depth " << i - 1 << ", " << posEval << " positions evaluated" << std::endl;
            std::cout << "DEBUG Choice made between " << PrevMoves.size() << " moves available" << std::endl; 
            break;
        }
    }

    std::cout << board << std::endl;

    std::cout << "BestMove: " << bestMove << " | " << board.evalNbr << " positions evaluated" << std::endl;
    */
    try {
        Brain brain;
        brain.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 84;
    }

    return 0;
}
