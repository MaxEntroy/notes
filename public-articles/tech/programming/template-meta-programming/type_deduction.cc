#include <iostream>

namespace case1 {

template<typename T>
void foo(T& param) {}  // param is reference

void test() {
  int x = 27;
  const int cx = x;
  const int& rx = x;

  foo(x);  // T is int, param's type is int&
  foo(cx);  // T is const int, param's type is const int&
  foo(rx);  // T is const int, param's type is const int&
}

template<typename T>
void bar(const T& param) {}  // param is now a ref-to-const, but it's still a reference

void test1() {
  int x = 27;
  const int cx = x;
  const int& rx = x;

  bar(x);  // T is int, param's type is const int&
  bar(cx);  // T is int, param's type is const int&
  bar(rx);  // T is int, param's type is const int&
}

}  // namespace case1

namespace case2 {

template<typename T>
void foo(T&& param) {}  // param is a universal reference

void test() {
  int x = 27;
  const int cx = x;
  const int& rx = x;

  foo(x);  // T is int&, param's type is int&
  foo(cx);  // T is const int&, param's type is const int&
  foo(rx);  // T is const int&, param's type is const int&
  foo(27);  // T is int, param's type is int&&
}

}  // namespace case2

namespace case3 {

template<typename T, std::size_t N>
constexpr std::size_t arraySize(T (&)[N]) noexcept {
  return N;
}

void test() {
  int keyVals[] = {1,3,5,7,9};
  std::cout << arraySize(keyVals) << std::endl;
}

}  // namespace case3

int main(void) {
  case1::test();
  case1::test1();
  case2::test();
  return 0;
}

