#pragma once

#include <cstddef>
#include <memory>

namespace arena {

using size = std::size_t;

size calculate_alignment(size offset, size align) noexcept;

class ArenaAllocator {
public:
  explicit ArenaAllocator(size capacity);
  ~ArenaAllocator();

  ArenaAllocator(const ArenaAllocator &) = delete;
  ArenaAllocator &operator=(const ArenaAllocator &) = delete;
  ArenaAllocator(ArenaAllocator &&) = delete;
  ArenaAllocator &operator=(ArenaAllocator &&) = delete;

  void *allocate(size n, size alignment = alignof(std::max_align_t));
  void reset() noexcept;
  size used() const noexcept;
  size remaining() const noexcept;
  void destroy_arena() noexcept;

private:
  std::byte *base;
  size capacity;
  size offset;
};

std::unique_ptr<ArenaAllocator> create_arena(size capacity);

} // namespace arena
