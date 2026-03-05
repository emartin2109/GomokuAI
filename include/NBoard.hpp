#include <bitset>
#include <array>
#include <stdexcept>

#include "Config.hpp"
#include "Exceptions.hpp"

#define BLACK 0
#define WHITE 1

/*
class RotableBoard {
    public:
        RotableBoard(std::bitset<BOARD_SIZE> board) {

        };
    private:
        std::bitset
        
}


struct BoardsData {
    std::array<std::array<std::bitset<BOARD_SIZE>, 4>, 2> boards;

    // masks that should be binary AND to the current boards in order to give availables moves
    std::bitset<BOARD_SIZE> proximityMask;
    std::bitset<BOARD_SIZE> forcingMoveMask;
}

struct BoardData {
    
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
*/