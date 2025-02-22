#include "MinMax.hpp"

std::tuple<std::vector<std::tuple<uint16_t, double>>, uint16_t, bool> MinMax::findBestMove(Board &board, const size_t &depth, const std::chrono::high_resolution_clock::time_point &begin, const std::vector<uint16_t> &moves) {
    size_t currentDepth = 1;
    uint16_t bestMove = -1;
    std::vector<std::tuple<uint16_t, double>> moveOrder;
    double bestScore = -DBL_MAX;

    for (size_t i = 0; i < moves.size(); i++) {
        double eval = 0;
        board.makeMove(moves[i]);

        if (board.isGameWon() == GameState::Lost) {
            bestMove = moves[i];
            bestScore = DBL_MAX;
            board.undoMove();
            break;
        }

        eval = -minmaxEvalBoard(board, currentDepth + 1, depth, -bestScore, begin);

        if (eval >= bestScore) {
            bestScore = eval;
            bestMove = moves[i];
        }

        board.undoMove();

        moveOrder.emplace_back(moves[i], eval);

        if (std::chrono::duration<float>(std::chrono::high_resolution_clock::now() - begin).count() >= TIME_PER_MOVE)
            return {moveOrder, 0, false};
    }

    if (bestMove == 65535)
        bestMove = 210;

    else if (bestScore > 10000000) {
        std::cout << "DEBUG Winning line found at depth " << depth << std::endl;
        return {moveOrder, bestMove, true};
    }

    return {moveOrder, bestMove, false};
}

double MinMax::minmaxEvalBoard(Board &board, size_t currentDepth, const size_t &maxDepth, double alpha, const std::chrono::high_resolution_clock::time_point &begin) {
    if (board.isGameWon() == GameState::Lost)
        return (-DBL_MAX) + static_cast<double>(currentDepth);

    if (maxDepth < currentDepth)
        return board.computeStaticEvaluation();


    double bestScore = -DBL_MAX;
    std::vector<uint16_t> childs = board.getAllMoves();

    if (childs.empty()) return 0;

    for (auto move : childs) {
        board.makeMove(move);

        double eval = -minmaxEvalBoard(board, currentDepth + 1, maxDepth, -bestScore, begin);
        if (eval > bestScore) {
            bestScore = eval;
        }

        board.undoMove();

        if (std::chrono::duration<float>(std::chrono::high_resolution_clock::now() - begin).count() >= TIME_PER_MOVE)
            return 0;

        // alpha beta prunning
        if (bestScore > alpha || bestScore > 100000) {
            return bestScore;
        }
    }

    return bestScore;
}