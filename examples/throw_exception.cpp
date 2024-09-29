#include <iostream>
#include <stdexcept>
#include <string>

#define MY_ASSERT(e)                                                  \
  if (!(e)) {                                                         \
    std::cerr << "Assertion failed: " << #e << '\n';                  \
    throw std::runtime_error("Assertion failed: " + std::string(#e)); \
  }

auto foo(int lhs, int rhs) -> void {
  std::cout << "bar(" << lhs << ", " << rhs << ")\n";
  MY_ASSERT(lhs == rhs);
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
