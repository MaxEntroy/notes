#include <iostream>

//  non-variadic print function
//  A declaration for the nonvariadic version of print must be in scope when
//  the variadic version is defined. Otherwise, the variadic function will recurse
//  indefinitely.
template<typename T>
std::ostream& print(std::ostream& os, const T& t) {
  return os << t;
}

template<typename T, typename... Args>
std::ostream& print(std::ostream& os, const T& t, const Args&... rest) {
  os << t << "|";
  return print(os, rest...);
}

int main(void) {
  print(std::cout, 13, "hello", 0.618);
  return 0;
}
