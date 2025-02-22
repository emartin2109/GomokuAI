/*
** EPITECH PROJECT, 2024
** B-AIA-500-BDX-5-1-gomoku-eliot1.martin
** File description:
** AI
*/

#pragma once

#include "Board.hpp"

#include <cfloat>
#include <cstddef>
#include <cstdint>
#include <chrono>
#include <tuple>
#include <vector>

class MinMax {
    public:
        static std::tuple<std::vector<std::tuple<uint16_t, double>>, uint16_t, bool> findBestMove(Board &board, const size_t &depth, const std::chrono::high_resolution_clock::time_point &begin, const std::vector<uint16_t> &moves);

    private:
        static double minmaxEvalBoard(Board &board, size_t currentDepth, const size_t &maxDepth, double alpha, const std::chrono::high_resolution_clock::time_point &begin);
};
