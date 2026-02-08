#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>

namespace arena {

using size = std::size_t;

size calculate_alignment(size offset, size align) noexcept;

class ArenaAllocator {
public:
  explicit ArenaAllocator(size capacity);
  ~ArenaAllocator();

  std::byte *get_base() { return this->base; }

  ArenaAllocator(const ArenaAllocator &) = delete;
  ArenaAllocator &operator=(const ArenaAllocator &) = delete;
  ArenaAllocator(ArenaAllocator &&) = delete;
  ArenaAllocator &operator=(ArenaAllocator &&) = delete;

  void *allocate(size n, size alignment = alignof(std::max_align_t));
  void reset() noexcept;
  size used() const noexcept;
  size remaining() const noexcept;

  // Builds an object using `.allocate` method
  template <typename T, typename... Args> T *make(Args... args) {
    void *mem = allocate(sizeof(T), alignof(T));
    if (mem)
      return new (mem) T(std::forward<Args>(args)...);
    else
      return nullptr;
  }

private:
  std::byte *base;
  size capacity;
  size offset;
};

std::unique_ptr<ArenaAllocator> create_arena(size capacity);
void destroy_arena(ArenaAllocator *arena) noexcept;

} // namespace arena
