#pragma once

#include "Board.hpp"

#include <functional>
#include <map>
#include <string>
#include <vector>

#define METHOD_STORE(method) [this](auto && PH1) { method(std::forward<decltype(PH1)>(PH1)); }

class Brain {
    public:
        Brain() = default;
        ~Brain() = default;

        Brain(const Brain& other) = delete;
        Brain operator=(const Brain& other) = delete;

        Brain(const Brain&& other) = delete;
        Brain operator=(const Brain&& other) = delete;


    public:
        void run();

        void startHandler(const std::vector<std::string>& args);
        void turnHandler(const std::vector<std::string>& args);
        void beginHandler(const std::vector<std::string>& args);
        void boardHandler(const std::vector<std::string>& args);
        void infoHandler(const std::vector<std::string>& args);
        void aboutHandler(const std::vector<std::string>& args);
        void endHandler(const std::vector<std::string>& args);

        void playMove();


    private:
        std::map<std::string, std::function<void(const std::vector<std::string>&)>> m_functionHandlers = {
            {"START", METHOD_STORE(startHandler)},
            {"RESTART", METHOD_STORE(startHandler)},
            {"TURN", METHOD_STORE(turnHandler)},
            {"BEGIN", METHOD_STORE(beginHandler)},
            {"BOARD", METHOD_STORE(boardHandler)},
            {"INFO", METHOD_STORE(infoHandler)},
            {"ABOUT", METHOD_STORE(aboutHandler)},
            {"END", METHOD_STORE(endHandler)}
        };

        std::bitset<400> selfBoard;
        std::bitset<400> oponentBoard;
        Board m_board{selfBoard, oponentBoard, std::bitset<400>{}, std::bitset<400>{}};
        int m_mapSize{};

        bool m_isRunning = true;
};
