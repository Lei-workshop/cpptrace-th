#include <exception>
#include <iostream>

#define MY_ASSERT(e)                                 \
  if (!(e)) {                                        \
    std::cerr << "Assertion failed: " << #e << '\n'; \
    std::terminate();                                \
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

auto main() -> int {
  bar();
  return 0;
}
