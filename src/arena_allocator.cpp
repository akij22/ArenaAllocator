#include "../include/arena_allocator.hpp"
#include <new>
#include <stdexcept>

namespace arena {

size calculate_alignment(size offset, size align) noexcept {
  if (align == 0) {
    return 0;
  }

  const size remainder = offset % align;
  return remainder == 0 ? 0 : align - remainder;
}

ArenaAllocator::ArenaAllocator(size dimension)
    : base(static_cast<std::byte *>(::operator new(dimension))),
      capacity(dimension), offset(0) {
  if (dimension == 0)
    throw std::invalid_argument("ArenaAllocator capacity must be > 0");
}

ArenaAllocator::~ArenaAllocator() { destroy_arena(this); }

void *ArenaAllocator::allocate(size n, size alignment) {
  const size padding = calculate_alignment(this->offset, alignment);
  const size aligned_offset = this->offset + padding;

  if (aligned_offset + n > this->capacity) {
    throw std::bad_alloc();
  }

  // `current` is a pointer to the beginning of the block
  void *current = static_cast<void *>(this->base + aligned_offset);

  // Advance the offset to the end of the new allocated block
  this->offset = aligned_offset + n;
  return current;
}

void ArenaAllocator::reset() noexcept { this->offset = 0; }

size ArenaAllocator::used() const noexcept { return this->offset; }

size ArenaAllocator::remaining() const noexcept {
  return this->capacity - this->offset;
}

std::unique_ptr<ArenaAllocator> create_arena(size capacity) {
  return std::unique_ptr<ArenaAllocator>(new ArenaAllocator(capacity));
}

void destroy_arena(ArenaAllocator *arena) noexcept {
  ::operator delete(arena->get_base());
  ::operator delete(arena);
}

} // namespace arena
