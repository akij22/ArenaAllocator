#include <iostream>
#define SHOW(arg) std::cout << arg << std::endl;

std::size_t calculate_alignment(std::size_t offset, std::size_t align) {
  return static_cast<int>(offset / align);
}

int main() {

  std::size_t f = calculate_alignment(16, alignof(int));
  std::cout << 13 + (8 - 13 % 8) << std::endl;
  return 0;
}
