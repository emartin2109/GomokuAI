#include <exception>
#include <string>

class ParsingError : public std::exception {
    public:
        ParsingError(std::string  message) : m_message(std::move(message)) {}

        [[nodiscard]] const char* what() const noexcept override {
            return m_message.c_str();
        }

    private:
        std::string m_message;
};