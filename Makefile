CXX      := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -O2 -g
INCLUDES := -Iinclude

SRC := src/arena_allocator.cpp main.cpp
BIN := main
ARENA_SRC := src/arena_allocator.cpp

TEST_DIR := tests
UNIT_TEST_SRC := $(TEST_DIR)/test_arena_allocator.cpp
UNIT_TEST_BIN := $(TEST_DIR)/arena_allocator_tests
LIFECYCLE_TEST_SRC := $(TEST_DIR)/test_arena_lifecycle.cpp
LIFECYCLE_TEST_BIN := $(TEST_DIR)/arena_lifecycle_test

all: $(BIN)

$(BIN): $(SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SRC) -o $@

run: $(BIN)
	./$(BIN)

$(UNIT_TEST_BIN): $(ARENA_SRC) $(UNIT_TEST_SRC) include/arena_allocator.hpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(ARENA_SRC) $(UNIT_TEST_SRC) -o $@

$(LIFECYCLE_TEST_BIN): $(ARENA_SRC) $(LIFECYCLE_TEST_SRC) include/arena_allocator.hpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(ARENA_SRC) $(LIFECYCLE_TEST_SRC) -o $@

test: $(UNIT_TEST_BIN)
	./$(UNIT_TEST_BIN)

test-lifecycle: $(LIFECYCLE_TEST_BIN)
	./$(LIFECYCLE_TEST_BIN)

test-all: test test-lifecycle

clean:
	rm -f $(BIN) $(UNIT_TEST_BIN) $(LIFECYCLE_TEST_BIN)

.PHONY: all run test test-lifecycle test-all clean
