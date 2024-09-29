#include <iostream>

#define MY_ASSERT(e)                                 \
  if (!(e)) {                                        \
    std::cerr << "Assertion failed: " << #e << '\n'; \
    throw 42;                                        \
  }

auto foo(int lhs, int rhs) -> void {
  std::cout << "bar(" << lhs << ", " << rhs << ")\n";
  MY_ASSERT(lhs == rhs);  // NOLINT(hicpp-exception-baseclass)
}

auto bar() -> void {
  int lhs = 1;
  int rhs = 2;
  foo(lhs, rhs);
}

// NOLINTNEXTLINE(bugprone-exception-escape)
auto main() -> int {
  bar();
  return 0;
}
