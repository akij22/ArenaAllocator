#include <cstdint>
#include <iostream>

using size = std::size_t;
using u64 = uint64_t;

class ArenaAllocator {
private:
  // A pointer to the correct offset into the arena
  std::byte *base;
  size capacity;
  size offset;

  size calculate_alignment(size offset, size align) {
    return static_cast<int>(offset / align);
  }

public:
  ArenaAllocator() = default;

  ArenaAllocator *create_arena(size capacity) {
    ArenaAllocator *ptr = static_cast<ArenaAllocator *>(operator new(capacity));

    return ptr;
  }

  void arena_destroy(ArenaAllocator *ptr) { operator delete(ptr); }

  // TODO
  void *allocate(size n, size alignment) {

    size aligned = offset + calculate_alignment(this->offset, alignment);

    if (aligned + n > this->capacity)
      throw;

    // Move offset to the end of the new allocated block
    this->offset = aligned + n;

    // Return a pointer to the start of the new allocated block
    return static_cast<void *>(base + aligned + n);
  }
};
