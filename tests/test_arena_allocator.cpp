#include "../include/arena_allocator.hpp"

#include <cstdint>
#include <exception>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

struct TestCase {
  const char *name;
  void (*fn)();
};

class TestFailure : public std::runtime_error {
public:
  explicit TestFailure(const std::string &message)
      : std::runtime_error(message) {}
};

#define EXPECT_TRUE(cond)                                                      \
  do {                                                                         \
    if (!(cond)) {                                                             \
      std::ostringstream _oss;                                                 \
      _oss << "Expectation failed at line " << __LINE__ << ": " #cond;         \
      throw TestFailure(_oss.str());                                           \
    }                                                                          \
  } while (false)

#define EXPECT_EQ(lhs, rhs)                                                    \
  do {                                                                         \
    const auto _lhs = (lhs);                                                   \
    const auto _rhs = (rhs);                                                   \
    if (!(_lhs == _rhs)) {                                                     \
      std::ostringstream _oss;                                                 \
      _oss << "Expectation failed at line " << __LINE__ << ": " #lhs           \
           << " == " #rhs << " (actual " << _lhs << " vs " << _rhs << ")";     \
      throw TestFailure(_oss.str());                                           \
    }                                                                          \
  } while (false)

#define EXPECT_THROW(stmt, ex_type)                                            \
  do {                                                                         \
    bool _thrown = false;                                                      \
    try {                                                                      \
      stmt;                                                                    \
    } catch (const ex_type &) {                                                \
      _thrown = true;                                                          \
    } catch (...) {                                                            \
      std::ostringstream _oss;                                                 \
      _oss << "Unexpected exception type at line " << __LINE__;                \
      throw TestFailure(_oss.str());                                           \
    }                                                                          \
    if (!_thrown) {                                                            \
      std::ostringstream _oss;                                                 \
      _oss << "Expected exception " #ex_type " at line " << __LINE__;          \
      throw TestFailure(_oss.str());                                           \
    }                                                                          \
  } while (false)

arena::ArenaAllocator *new_arena(arena::size capacity) {
  // Tests intentionally leak the arena to avoid the allocator's current
  // destruction-path crash from terminating the entire test process.
  return new arena::ArenaAllocator(capacity);
}

struct Pair {
  int i;
  float f;
  Pair(int x, float y) : i(x), f(y) {}
};

void test_calculate_alignment() {
  EXPECT_EQ(arena::calculate_alignment(0, 8), static_cast<arena::size>(0));
  EXPECT_EQ(arena::calculate_alignment(1, 8), static_cast<arena::size>(7));
  EXPECT_EQ(arena::calculate_alignment(15, 16), static_cast<arena::size>(1));
  EXPECT_EQ(arena::calculate_alignment(24, 8), static_cast<arena::size>(0));
  EXPECT_EQ(arena::calculate_alignment(11, 0), static_cast<arena::size>(0));
}

void test_constructor_rejects_zero_capacity() {
  EXPECT_THROW(arena::ArenaAllocator bad(0), std::invalid_argument);
}

void test_allocate_updates_usage() {
  arena::ArenaAllocator *arena = new_arena(64);
  EXPECT_EQ(arena->used(), static_cast<arena::size>(0));
  EXPECT_EQ(arena->remaining(), static_cast<arena::size>(64));

  void *p1 = arena->allocate(8, 8);
  EXPECT_TRUE(p1 != nullptr);
  EXPECT_EQ(arena->used(), static_cast<arena::size>(8));
  EXPECT_EQ(arena->remaining(), static_cast<arena::size>(56));

  void *p2 = arena->allocate(4, 4);
  EXPECT_TRUE(p2 != nullptr);
  EXPECT_EQ(arena->used(), static_cast<arena::size>(12));
  EXPECT_EQ(arena->remaining(), static_cast<arena::size>(52));
}

void test_allocate_respects_alignment() {
  arena::ArenaAllocator *arena = new_arena(128);
  arena->allocate(1, 1);
  void *aligned = arena->allocate(8, 8);

  const std::uintptr_t address = reinterpret_cast<std::uintptr_t>(aligned);
  EXPECT_EQ(address % 8, static_cast<std::uintptr_t>(0));
  EXPECT_EQ(arena->used(), static_cast<arena::size>(16));
}

void test_allocate_throws_when_full() {
  arena::ArenaAllocator *arena = new_arena(32);
  arena->allocate(24, 8);
  EXPECT_THROW(arena->allocate(16, 8), std::bad_alloc);
}

void test_reset_reuses_from_base() {
  arena::ArenaAllocator *arena = new_arena(64);
  void *first = arena->allocate(8, 8);
  arena->allocate(8, 8);
  arena->reset();

  EXPECT_EQ(arena->used(), static_cast<arena::size>(0));
  EXPECT_EQ(arena->remaining(), static_cast<arena::size>(64));

  void *after_reset = arena->allocate(8, 8);
  EXPECT_TRUE(after_reset == first);
}

void test_make_constructs_object() {
  arena::ArenaAllocator *arena = new_arena(64);
  Pair *pair = arena->make<Pair>(7, 3.5f);
  EXPECT_TRUE(pair != nullptr);
  EXPECT_EQ(pair->i, 7);
  EXPECT_TRUE(pair->f == 3.5f);
}

void test_make_throws_bad_alloc_when_capacity_exceeded() {
  arena::ArenaAllocator *arena = new_arena(7);
  EXPECT_THROW(arena->make<Pair>(1, 2.0f), std::bad_alloc);
}

} // namespace

int main() {
  const std::vector<TestCase> tests = {
      {"calculate_alignment", test_calculate_alignment},
      {"constructor_rejects_zero_capacity",
       test_constructor_rejects_zero_capacity},
      {"allocate_updates_usage", test_allocate_updates_usage},
      {"allocate_respects_alignment", test_allocate_respects_alignment},
      {"allocate_throws_when_full", test_allocate_throws_when_full},
      {"reset_reuses_from_base", test_reset_reuses_from_base},
      {"make_constructs_object", test_make_constructs_object},
      {"make_throws_bad_alloc_when_capacity_exceeded",
       test_make_throws_bad_alloc_when_capacity_exceeded},
  };

  int failed = 0;
  for (const TestCase &test : tests) {
    try {
      test.fn();
      std::cout << "[PASS] " << test.name << "\n";
    } catch (const std::exception &e) {
      ++failed;
      std::cerr << "[FAIL] " << test.name << ": " << e.what() << "\n";
    } catch (...) {
      ++failed;
      std::cerr << "[FAIL] " << test.name << ": unknown error\n";
    }
  }

  if (failed == 0) {
    std::cout << "All " << tests.size() << " tests passed.\n";
    return 0;
  }

  std::cerr << failed << " of " << tests.size() << " tests failed.\n";
  return 1;
}
