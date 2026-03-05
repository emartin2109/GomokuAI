#include <array>
#include <bitset>
#include <iostream>
#include <cmath>

#include "Config.hpp"
#include "precomputed/values.hpp"

/*
FOR TRANSPOSED 0
('4096' > ('5' * 'rowNBR')) & 11111 = line to check for patern

FOR TRANSPOSED 90
('4097' > 'columnNBR') & 100001000010000100001



00000
00000
00100
00000
00000


class RotableBoard {
    public:
        RotableBoard(std::bitset<BOARD_SIZE> board) {
            rotatedBoards[TRANSPOSED0] = board;
        };
        RotableBoard() {
            rotatedBoards[TRANSPOSED0] = 0;
            rotatedBoards[TRANSPOSED45] = 0;
            rotatedBoards[TRANSPOSED90] = 0;
            rotatedBoards[TRANSPOSED135] = 0;
        }
    
    public:
        void pprint() {
            for (size_t i = 0; i < 4; i++) {
                std::cout << "BOARD" << i << ":" << std::endl;

                std::cout << BOARD_LENGTH << std::endl;
                for (size_t x = 0; x < BOARD_LENGTH; x++) {
                    for (size_t y = 0; y < BOARD_LENGTH; y++) {
                        std::cout << rotatedBoards[i][x * BOARD_LENGTH + y];
                    }
                    std::cout << std::endl;
                }
                std::cout << std::endl;
            }
            
        }

    private:
        std::array<std::bitset<BOARD_SIZE>, 4> rotatedBoards;
};
*/
