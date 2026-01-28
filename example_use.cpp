#include "include/arena_allocator.hpp"
#include <iostream>
#include <new>
#include <string>

struct Vec3 {
  float x;
  float y;
  float z;
};

int main() {
  auto arena = arena::create_arena(512);

  auto *count = static_cast<int *>(arena->allocate(sizeof(int), alignof(int)));
  *count = 7;

  auto *pos = static_cast<Vec3 *>(arena->allocate(sizeof(Vec3), alignof(Vec3)));
  pos->x = 1.0f;
  pos->y = 2.0f;
  pos->z = 3.0f;

  auto *message = static_cast<std::string *>(
      arena->allocate(sizeof(std::string), alignof(std::string)));
  new (message) std::string("Hello from the arena!");

  std::cout << "count=" << *count << " pos=" << pos->x << "," << pos->y << ","
            << pos->z << "\n";
  std::cout << *message << "\n";
  std::cout << "used=" << arena->used() << " remaining=" << arena->remaining()
            << "\n";

  message->~basic_string();
  arena->reset();

  std::cout << "after reset -> used=" << arena->used()
            << " remaining=" << arena->remaining() << "\n";

  return 0;
}
