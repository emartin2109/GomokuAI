
#include <algorithm>
#include <cstddef>

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
