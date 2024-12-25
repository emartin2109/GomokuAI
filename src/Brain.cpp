#include <bitset>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#include <chrono>

#include "Board.hpp"
#include "Brain.hpp"
#include "MinMax.hpp"
#include "Type.hpp"

class ParsingError : public std::exception {
    public:
        ParsingError(std::string  message) : m_message(std::move(message)) {}

        [[nodiscard]] const char* what() const noexcept override {
            return m_message.c_str();
        }

    private:
        std::string m_message;
};

std::vector<std::string> strToWordArray(const std::string& str) {
    std::vector<std::string> words;
    std::string word;
    std::istringstream stream(str);

    while (std::getline(stream, word, ' ')) {
        std::istringstream wordStream(word);
        std::string subWord;
        while (std::getline(wordStream, subWord, ',')) {
            if (!subWord.empty()) {
                words.push_back(subWord);
            }
        }
    }

    return words;
}

void Brain::run() {
    std::string line;
    while (m_isRunning) {
        // Get the next line
        std::getline(std::cin, line);
        if (line.empty())
            break;

        if (line[line.size() - 1] == '\r')
            line.pop_back();


        // Split the line into words
        std::vector<std::string> args = strToWordArray(line);
        if (args.empty()) {
            std::cout << "ERROR Empty command" << std::endl;
            continue;
        }


        // Check if the command is valid
        if (m_functionHandlers.find(args.at(0)) == m_functionHandlers.end()) {
            std::cout << "ERROR Unknown command: \"" << args.at(0) << "\"" << std::endl;
            continue;
        }


        // Execute the command
        try {
            m_functionHandlers.at(args.at(0))(args);
        } catch (const std::exception& e) {
            std::cout << "ERROR executing command: \"" << args.at(0) << "\": " << e.what() << std::endl;
        }
    }
}

void Brain::playMove() {
    uint16_t bestMove = 0;
    uint16_t maxDepth = 50;
    uint16_t posEval = 0;

    std::chrono::high_resolution_clock::time_point begin = std::chrono::high_resolution_clock::now();
    for (uint16_t i = 1; i <= maxDepth; i++) {
        std::vector<uint16_t> moves = m_board.getAllMoves();
        std::tuple<std::vector<std::tuple<uint16_t, double>>, uint16_t, bool> newMove = MinMax::findBestMove(m_board, i, begin, moves);
        if (std::get<1>(newMove) != 0) {
            bestMove = std::get<1>(newMove);
            posEval = m_board.evalNbr;
            if (std::get<2>(newMove)) {
                std::cout << "DEBUG Move found at depth " << i << ", " << posEval << " positions evaluated" << std::endl;
                std::cout << "DEBUG Choice made between " << moves.size() << " moves available" << std::endl; 
                break;
            }
        } else {
            std::cout << "DEBUG Move found at depth " << i - 1 << ", " << posEval << " positions evaluated" << std::endl;
            std::cout << "DEBUG Choice made between " << moves.size() << " moves available" << std::endl; 
            break;
        }
/*
        // Iterative deepening
        std::vector<std::tuple<uint16_t, double>> prevMoveScore = std::get<0>(newMove);

        std::sort(prevMoveScore.begin(), prevMoveScore.end(),
          [](const std::tuple<uint16_t, double>& a, const std::tuple<uint16_t, double>& b) {
              return std::get<1>(a) < std::get<1>(b);
          });

        // Extract the uint16_t values from vector1 into vector2
        moves.clear();
        for (const auto& elem : prevMoveScore) {
            moves.push_back(std::get<0>(elem));
        }
*/
    }

    std::cout << bestMove / BOARD_SIZE << "," << bestMove % BOARD_SIZE << std::endl;
    m_board.evalNbr = 0;
    m_board.makeMove(bestMove);
}

void Brain::startHandler(const std::vector<std::string>& args) {
    int size = 0;

    try {
        size = std::stoi(args.at(1));
    } catch (const std::exception& e) {
        throw ParsingError("Cannot parse board size");
    }

    if (size < 5 || size > 50)
        throw ParsingError("Invalid board size");

    m_mapSize = size;

    std::cout << "OK" << std::endl;
}

void Brain::turnHandler(const std::vector<std::string>& args) {
    int x = 0;
    int y = 0;

    try {
        y = std::stoi(args.at(1));
        x = std::stoi(args.at(2));
    } catch (const std::exception& e) {
        throw ParsingError("Cannot parse position");
    }

    if (x < 0 || x >= m_mapSize || y < 0 || y >= m_mapSize)
        throw ParsingError("Invalid position or board not initialized");

    m_board.makeMove(y * m_mapSize + x);

    playMove();
}

void Brain::beginHandler(const std::vector<std::string>& /* UNUSED */) {
    if (m_mapSize == 0)
        throw std::runtime_error("Game not started");

    playMove();
}

void Brain::boardHandler(const std::vector<std::string>& /* UNUSED */) {
    std::string input;
    int x = 0;
    int y = 0;
    int field = 0;
    char comma = 0;

    while (m_isRunning) {
        std::getline(std::cin, input);
        if (input.empty()) {
            m_isRunning = false;
            return;
        }

        if (input[input.size() - 1] == '\r')
            input.pop_back();

        if (input == "DONE") {
            break;
        }

        std::istringstream ss(input);
        if (ss >> y >> comma >> x >> comma >> field) {
            if (comma != ',' || (field < 1 || field > 3) || (x < 0 || x >= m_mapSize || y < 0 || y >= m_mapSize)) {
                std::cerr << "Invalid position or input format: " << input << std::endl;
                continue;
            }
            if (x < 0 || x >= m_mapSize || y < 0 || y >= m_mapSize)
                throw ParsingError("Invalid position or board not initialized");
            if (field == 1) {
                m_board.placeStone(y * m_mapSize + x);
            } else if (field == 2) {
                m_board.placeOpStone(y * m_mapSize + x);
            }
        } else {
            std::cerr << "Failed to parse input line: " << input << std::endl;
        }
    }

    playMove();
}

void Brain::infoHandler(const std::vector<std::string>&  /* UNUSED */) {
}

void Brain::aboutHandler(const std::vector<std::string>& /* UNUSED */) {    // NOLINT(readability-convert-member-functions-to-static)
    std::cout << "name=\"愚蠢的蟒蛇他媽的機器人\", version=\"1.0\", author=\"Kelp Technology Inc\", country=\"GRC\"" << std::endl;
}

void Brain::endHandler(const std::vector<std::string>& /* UNUSED */) {
    m_isRunning = false;
}
