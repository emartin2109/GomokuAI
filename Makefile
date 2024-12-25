##
## EPITECH PROJECT, 2024
## Gomoku
## File description:
## Makefile
##

CXX = g++
CXXFLAGS = -W -Wall -Wextra -Wpedantic -I./include -Ofast -O3 -ffast-math
SRC_DIR = src
TARGET = pbrain-gomoku-ai

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(TARGET)

tests:
	cmake -S . -Bbuild -G "Unix Makefiles" -DBUILD_TESTS=ON && cmake --build build --target pbrain-gomoku-ai-tests

re: fclean all

.PHONY: all clean fclean tests re
