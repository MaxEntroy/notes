#include <iostream>

void test1() {
  auto show = [](const auto& v) { std::cout << v << std::endl; };
  auto get_value = [](){ return 1; };

  show(get_value());

  auto one_cps = [](auto&& fn, const auto& v) { fn(v); };
  one_cps(show, get_value);
}

void test2() {
  auto add_op = [](const auto& lhs, const auto& rhs) {
    return lhs + rhs;
  };
  auto mul_op = [](const auto& lhs, const auto& rhs) {
    return lhs * rhs;
  };

  auto sum_of_squares = [&](const auto& lhs, const auto& rhs) {
    return add_op(mul_op(lhs, lhs), mul_op(rhs, rhs));
  };
  std::cout << sum_of_squares(3, 4) << std::endl;

  auto sum_of_squares_cps = [&](const auto& lhs, const auto& rhs) {
    return mul_cps(lhs, lhs)
  }
}

int main() {
  test1();
  test2();
  return 0;
}
