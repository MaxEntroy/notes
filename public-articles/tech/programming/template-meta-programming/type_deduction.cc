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

template<typename T>
void foo(T param) {}  // param is now passed by value

void test() {
  int x = 27;
  const int cx = x;
  const int& rx = x;

  foo(x);  // T's and param's types are both int
  foo(cx);  // T's and param's types are both int
  foo(rx);  // T's and param's types are both int

  const char* const ptr = "func with pointers.";
  foo(ptr);  // In accord with the type deduction rule for by-value parameters,
             // the constness of ptr will be ignored, and the type deduced for param will be const char*
}

}  // namespace case3

namespace case4 {

template<typename T>
void foo(T param) {}

template<typename T>
void goo(T& param) {}

template<typename T, std::size_t N>
constexpr std::size_t arraySize(T (&)[N]) noexcept {
  return N;
}

void test() {
  const char name[] = "Kang Lee."; // name's type is const char[9]
  const char* ptrToName = name;  // array-to-pointer decay rule

  foo(name);  // name is array, but T deduced as const char*
  goo(name);  // T is deduced to be const char [13], the type deduced for T is the actual type of the array!
              // ParamType is const char (&)[13]

  int keyVals[] = {1,3,5,7,9};
  std::cout << arraySize(keyVals) << std::endl;
}

}  // namespace case4

namespace case5 {

void someFunc(int val, double dval) {}

template<typename T>
void foo(T param) {}

template<typename T>
void goo(T& param) {}

void test() {
  foo(someFunc);  // param deduced as ptr-to-func;
                  // type is void (*)(int, double)
  goo(someFunc);  // param deduced as ref-to-func;
                  // type is void (&)(int, double)
}

}  // namespace case5

int main(void) {
  case1::test();
  case1::test1();
  case2::test();
  case3::test();
  case4::test();
  case5::test();
  return 0;
}

