#include <iostream>

using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
using size = std::size_t;

#define SHOW(arg) std::cout << arg << std::endl;

template <typename T> size calculate_padding_alignment(size offset, T type) {

  size align = alignof(type);
  int dif = offset / align;

  return static_cast<size>(offset + dif);
}

int main() {
  int a = 5;

  SHOW(calculate_padding_alignment(13, a));
  return 0;
}
