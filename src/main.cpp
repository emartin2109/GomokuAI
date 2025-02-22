#include "Brain.hpp"

#include <exception>
#include <iostream>
#include <ostream>

int main() {
    try {
        Brain brain;
        brain.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 84;
    }

    return 0;
}
