#include "include/arena_allocator.hpp"
#include <cstddef>
#include <iostream>
#include <unistd.h>

constexpr std::size_t ONE_GB = 1024ull * 1024 * 1024;

int main() {

  arena::ArenaAllocator arena(ONE_GB);

  auto str_ptr = arena.make<std::string>("ciao");

  (void)str_ptr;

  std::cout << getpid() << std::endl;

  std::cin.get();

  return 0;
}
