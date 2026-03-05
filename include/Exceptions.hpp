#include <bits/stdc++.h>

class InvalidBoardSize: public std::exception {
    private:
        std::string message;

    public:

        InvalidBoardSize(const char* msg):
        message(msg) {}

        const char* what() const noexcept {
            return message.c_str();
        }
    };