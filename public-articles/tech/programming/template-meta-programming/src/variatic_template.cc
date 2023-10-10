#include <iostream>
#include <string>
#include <sstream>

//  non-variadic print function
//  A declaration for the nonvariadic version of print must be in scope when
//  the variadic version is defined. Otherwise, the variadic function will recurse
//  indefinitely.
template<typename T>
std::ostream& print(std::ostream& os, const T& t) {
  return os << t << std::endl;
}

template<typename T, typename... Args>
std::ostream& print(std::ostream& os, const T& t, const Args&... rest) {
  os << t << "|";
  return print(os, rest...);
}

// print any type we don't otherwise handle
template <typename T>
std::string debug_rep(const T &t) {
  std::ostringstream ret; // see § 8.3 (p. 321)
  ret << "[Debug]" << t ; // uses T's output operator to print a representation of t
  return ret.str(); // return a copy of the string to which ret is bound
}

template<typename... Args>
std::ostream& err_msg(std::ostream& os, const Args... rest) {
  return print(os, debug_rep(rest)...);
}

int main(void) {
  print(std::cout, 13, "hello", 0.618);
  err_msg(std::cout, 13, "hello,world", 0.618);
  return 0;
}
