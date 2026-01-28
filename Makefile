CXX      := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -O2 -g
INCLUDES := -Iinclude

SRC := src/arena_allocator.cpp main.cpp
BIN := main

all: $(BIN)

$(BIN): $(SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SRC) -o $@

run: $(BIN)
	./$(BIN)

clean:
	rm -f $(BIN)

.PHONY: all run clean
