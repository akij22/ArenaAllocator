# Arena Allocator (C++)

A small C++20 project that implements a linear (bump) arena allocator for fast, contiguous memory allocation.

## Overview

This repository provides:
- A reusable `ArenaAllocator` implementation in `include/` and `src/`
- A runnable demo in `main.cpp`
- An additional usage example in `example_use.cpp`
- A simple `Makefile` for build/run/clean tasks

The allocator is designed for workloads where many small allocations are made together and can be released in one step.

## Technical Details

`ArenaAllocator` manages a preallocated memory block and tracks a moving offset:

- Allocation strategy: linear bump allocation (`allocate` advances `offset`)
- Alignment: handled per allocation through `calculate_alignment(offset, align)`
- Reset model: `reset()` clears all allocations at once by setting offset back to `0`
- Capacity checks: throws `std::bad_alloc` if the requested block does not fit
- Construction validation: throws `std::invalid_argument` for zero capacity

### Public API

Defined in `include/arena_allocator.hpp`:

- `void* allocate(size n, size alignment = alignof(std::max_align_t))`
- `void reset() noexcept`
- `size used() const noexcept`
- `size remaining() const noexcept`
- `std::byte* get_base()`
- `std::unique_ptr<ArenaAllocator> create_arena(size capacity)`
- `void destroy_arena(ArenaAllocator* arena) noexcept`

## Project Structure

```text
arena_allocator/
├── include/
│   └── arena_allocator.hpp      # Public allocator interface
├── src/
│   └── arena_allocator.cpp      # Allocator implementation
├── main.cpp                     # Primary demo executable
├── example_use.cpp              # Alternative example usage
├── Makefile                     # Build and run commands
├── main                         # Built executable artifact
├── ArenaAllocator               # Additional built executable artifact
├── *.o, *.dSYM                 # Build/debug artifacts
└── README.md
```

## Build and Run

### Requirements
- A C++20-compatible compiler (e.g., `g++`)
- `make`

### Using the Makefile

```bash
make        # Build main executable
make run    # Build and run main
make clean  # Remove main executable
```

### Build `example_use.cpp` manually

```bash
g++ -std=c++20 -Wall -Wextra -O2 -g -Iinclude src/arena_allocator.cpp example_use.cpp -o ArenaAllocator
./ArenaAllocator
```

## Usage Notes

- The arena does **not** support freeing individual allocations.
- Objects created with placement `new` should have destructors called manually before `reset()`.
- Use `used()` and `remaining()` to monitor memory pressure while allocating.

## License

No license file is currently included in this repository.
