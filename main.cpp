#include "arena_allocator.hpp"

#include <iostream>
#include <new>
#include <string>

struct Example {
  double x;
  int y;
};

int main() {
  auto arena = arena::create_arena(256);

  auto *value = static_cast<int *>(arena->allocate(sizeof(int), alignof(int)));
  *value = 42;

  auto *example = static_cast<Example *>(
      arena->allocate(sizeof(Example), alignof(Example)));
  example->x = 3.14;
  example->y = 21;

  auto *message_buffer =
      static_cast<std::string *>(
          arena->allocate(sizeof(std::string), alignof(std::string)));
  new (message_buffer) std::string("Arena allocators are fast!");

  std::cout << *value << ", " << example->x << ", " << example->y << "\n";
  std::cout << *message_buffer << "\n";
  std::cout << "used=" << arena->used() << " remaining=" << arena->remaining()
            << "\n";

  message_buffer->~basic_string();
  arena->reset();
  std::cout << "after reset -> used=" << arena->used() << " remaining="
            << arena->remaining() << "\n";

  return 0;
}
