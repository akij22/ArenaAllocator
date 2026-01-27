#include <cstdint>
#include <iostream>
#define SHOW(arg) std::cout << arg << std::endl;

using size = std::size_t;
using u64 = uint64_t;

size calculate_alignment(size offset, size align) {

  return (offset + (align - offset % align)) - offset;
  // 13 + (8 - 13 % 8)
}

class ArenaAllocator {
private:
  // a pointer to the beginning of the block
  std::byte *base;
  size capacity;

  // It refers to the end of the current memory allocated
  size offset;

public:
  /* Default constructor, which:
   * 1.allocate raw memory with dimension `dimension`;
   * 2. set `capacity = dimension`;
   * 3. set offset equals to zero;
   *
   * */
  explicit ArenaAllocator(size dimension)
      : base(static_cast<std::byte *>(::operator new(dimension))),
        capacity(dimension), offset(0) {}

  ArenaAllocator *create_arena(size capacity) {
    auto *ptr = new ArenaAllocator(capacity);
    return ptr;
  }

  void arena_destroy(ArenaAllocator *ptr) { operator delete(ptr); }

  // TODO
  //
  // `n` = the dimension requested to allocate
  void *allocate(size n, size alignment) {

    if (offset % alignment == 0) {
      this->offset += n;
      return static_cast<void *>(base);
    }

    size padding = offset + calculate_alignment(this->offset, alignment);

    SHOW(padding);

    if (padding + n > this->capacity)
      throw;

    // Move offset to the end of the new allocated block
    this->offset = padding + n;

    this->base += padding;

    // DEBUG
    SHOW(this->offset);
    return static_cast<void *>(base);
  }
};

int main() {

  ArenaAllocator *a1 = new ArenaAllocator(100);
  a1->allocate(12, 3);
  a1->allocate(20, 8);

  delete a1;
  return 0;
}
