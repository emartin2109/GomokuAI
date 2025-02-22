#include <sstream>
#include <string>
#include <vector>

#include "Utils.hpp"

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
