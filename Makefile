CXX      := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -O2 -g

SRC  := main.cpp
BIN  := main

all: $(BIN)

$(BIN): $(SRC)
	$(CXX) $(CXXFLAGS) $< -o $@

run: $(BIN)
	./$(BIN)

clean:
	rm -f $(BIN)

