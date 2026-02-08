# Arena Allocator Test Guide

## Prerequisites

- `g++` with C++20 support
- `make`

## Available Test Targets

- `make test`
Runs the core allocator unit tests in `tests/test_arena_allocator.cpp`.

- `make test-lifecycle`
Runs the lifecycle regression check in `tests/test_arena_lifecycle.cpp`, which verifies that creating and destroying an arena does not crash.

- `make test-all`
Runs both targets: `test` and `test-lifecycle`.

## Typical Workflow

1. Build and run core tests:
```bash
make test
```

2. Run lifecycle regression:
```bash
make test-lifecycle
```

3. Run everything:
```bash
make test-all
```

## Running Test Binaries Directly

After building, you can run the binaries directly:

```bash
./tests/arena_allocator_tests
./tests/arena_lifecycle_test
```

## Exit Codes

- `0`: success
- non-zero: one or more tests failed

## Notes

- Core tests validate alignment behavior, usage accounting, reset semantics, exception paths, and object construction via `make`.
- Lifecycle test isolates create/destroy behavior in a subprocess so crashes are reported as a clean test failure.
