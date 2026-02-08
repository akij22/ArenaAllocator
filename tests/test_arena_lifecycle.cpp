#include "../include/arena_allocator.hpp"

#include <cstdlib>
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  const pid_t pid = fork();
  if (pid < 0) {
    std::perror("fork");
    return 2;
  }

  if (pid == 0) {
    auto arena = arena::create_arena(64);
    auto *value =
        static_cast<int *>(arena->allocate(sizeof(int), alignof(int)));
    *value = 123;
    return 0;
  }

  int status = 0;
  if (waitpid(pid, &status, 0) < 0) {
    std::perror("waitpid");
    return 2;
  }

  if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
    std::cout << "[PASS] lifecycle create/destroy completed without crash\n";
    return 0;
  }

  if (WIFSIGNALED(status)) {
    std::cerr << "[FAIL] lifecycle create/destroy crashed with signal "
              << WTERMSIG(status) << "\n";
    return 1;
  }

  std::cerr << "[FAIL] lifecycle create/destroy exited abnormally\n";
  return 1;
}
