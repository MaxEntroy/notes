#include <iostream>

template<
  typename T,
  typename = std::enable_if_t<
    !std::is_same_v<std::string, T>
    and
    !std::is_same_v<bool, T>
  >
>
void Print(T param) {
  if constexpr (std::is_same_v<int, T>) {
    std::cout << "This is an intergal value:" << param << std::endl;
  } else if constexpr (std::is_same_v<double, T>) {
    std::cout << "This is a double value:" << param << std::endl;
  } else {
    std::cout << "Mismatch type value:" << param << std::endl;
  }
}

int main(void) {
  int val = 3;
  double pi = 3.14;
  float e = 2.718;
  Print(val);
  Print(pi);
  Print(e);

  // bool flag = false;
  // Print(flag);

  //std::string str = "hello";
  //Print(str);
  return 0;
}

