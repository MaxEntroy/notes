#include <stddef.h>
#include <iostream>

////**
/// * 这其实是一个带名字空间的全局变量
/// * IsMoreThanPtr::value 本质是一个全局变量
/// *
/// * 用法：用于引导模板全局常量的模板类
/// * 可以理解为做了编译期计算: 用来判断一个类型的大小是否比指针类型大
/// */
///template<typename T>
///struct IsMoreThanPtr {
///  static bool value = sizeof(T) > sizeof(void *);
///};
///
////**
/// * 全局模板常量
/// */
///template<typename V>
///constexpr inline bool IsMoreThanPtr_v = IsMoreThanPtr<T>::value;

namespace template_rename {

template<typename T, size_t size>
class Array {
 public:
  T& at(int index) { return data_[index]; }
 private:
  T data_[size];
};

template<typename T>
using DefaultArray = Array<T, 16>;

template<typename T>
using DefaultPtrArray = DefaultArray<T*>;

void test_array() {
  Array<int, 16> arr;
  std::cout << arr.at(3) << std::endl;

  DefaultArray<int> de_arr;
  std::cout << de_arr.at(3) << std::endl;

  DefaultPtrArray<int> de_ptr_arr;
  std::cout << de_ptr_arr.at(3) << std::endl;
}

/**
 * 这也是个很好的例子，比如给你一个要求，实现一个工具，可以获取任意类型的指针类型
 * 很明显，对于通用性能力的抽象，有两种，第一种oo，第二种泛型。
 * 这里由于直接和类型相关，使用泛型。萃取的例子
 * 其实本质还是编译期计算, using type = T*; 这是一个statement，就是一个计算
 * 这个编译期计算，本质是借助的是模板的实例化。所以，模板实例化的核心点在这里，实例化的过程也是完成编译期计算的过程。
 *
 */
template <typename T>
struct GetPtr {
  using type = T*;
};

template <typename T>
using GetPtr_t = typename GetPtr<T>::type;

void demo() {
  GetPtr_t<int> p{nullptr};
  std::cout << p << std::endl;
}

}  // namespace template_rename

int main() {
  template_rename::test_array();
  template_rename::demo();
  return 0;
}
