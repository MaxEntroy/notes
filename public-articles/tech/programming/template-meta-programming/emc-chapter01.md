### Item1: Understand template type duction

We can think of a function template as looking like this:

```cpp
template<typename T>
void f(ParamType param)
```

A call can look like this:

```cpp
f(expr);
```

During compilation, compilers use ```expr``` to deduce two types: one for ```T``` and one for  ```ParamType```. These types are frequently different, because ```ParamType``` often contains adornments, e.g., const or reference qualifiers.

- It's natural to expect that the type deduced for ```T``` is the same as the type of the argument passed to the function. But it doesn't always work that way.
- The type deduced for ```T``` is depedent not just on the type of expr, but also on the form of ```ParamType```.

#### Case1: ParamType is a Reference or Pointer, but not a Universal Reference

- If expr's type is a reference, ignore the reference part.
- Then pattern-match expr's type against ParamType to determine T.

ps: If param were a pointer(or a pointer to const) instead of a reference, things would work essentially the same way.

```cpp
template<typename T>
void f(T& param);

int x = 27;
const int cx = x;
const int& rx = x;

f(x);  // x is an int, T is int, ParamType is int&
f(cx); // cx is a const int, T is const int, ParamType is const int&
f(rx); // rx is a reference to x as a const int, T is const int, ParamType is const int&
```

If we change the type of ParamType from ```T&``` to ```const T&```, things change a little, but not in any surprising way.There is no longer a need for const to be deduced as part of ```T```

```cpp
template<typename T>
void f(const T& param);

int x = 27;
const int cx = x;
const int& rx = x;

f(x);  // x is an int, T is int, ParamType is int&
f(cx); // cx is a const int, T is int, ParamType is const int&
f(rx); // rx is a reference to x as a const int, T is int, ParamType is const int&

#### Case2: ParamType is a Universal Reference

They behave differently when lvalue arguments are passed in.

- If expr is an lvalue, both T and ParamType are deduced to be lvalue reference(T&).
- If expr is an rvalue, the normal rules apply.

```cpp
template<typename T>
void f(T&& param); // param is now a universal reference

int x = 27;
const int cx = x;
const int& rx = x;

f(x);  // x is an int, T is int&, ParamType is int&
f(cx); // cx is a const int, T is const int&, ParamType is const int&
f(rx); // rx is a reference to x as a const int, T is const int&, ParamType is const int&
f(27); // 27 is a rvalue(int), T is int, ParamType is int&&
```

The key point here is that the type deduction rules for universal reference parameters are different from those for parameters that are lvalue references or rvalue references.

#### Case3: ParamType is Neither a Pointer nor a Reference

When ParamType is neither a pointer nor a reference, we’re dealing with pass-by-
value:

- As before, if expr’s type is a reference, ignore the reference part.
- If, after ignoring expr’s reference-ness, expr is const, ignore that, too.

#### Case4: Array Arguments.

Be careful:  array-to-pointer decay rule

- The type of an array that’s passed to a template function **by value** is deduced to be a pointer type.

Interestingly, the ability to declare references to arrays enables creation of a template that deduces the number of elements that an array contains

所谓的编译期计算，其本质就是借助参数推到完成计算。

我总结下，如果实参是数组
1. 形参传值，推导的是指针类型
2. 形参传引用，推到的是数组类型，从而计算数组大小
3. 形参传引用 + 模板非类型参数，推到的是数组类型，非类型参数推到为数组大小

```cpp
template<typename T>
void goo(T& param) {}

// 这里增加非类型模板参数的目的是，可以进行推到，也即编译期计算
// T (&)[] 这个是啥类型呢？这个是绑定数组类型T [N]的左值引用类型
template<typename T, std::size_t N>
constexpr std::size_t arraySize(T (&)[N]) noexcept {
  return N;
}
```

#### Case5: Function Arguments

Function types can also decay into function pointers.

函数这里，我觉得和数组有点不同。对于数组而言，形参传值传引用，推导出来的类型不一样。ParamType自然也不一样。
对于函数，我觉得推到的类型是一样的。只不过ParamType不一样，后者是个引用。

- 数组类型

| 实参类型 | T | ParamType |
| :-----:| :----: | :----: |
| 传值 | 指针类型 | 指针类型 |
| 传引用 | 数组类型 | 绑定数组类型的引用 |

- 函数类型

| 实参类型 | T | ParamType |
| :-----:| :----: | :----: |
| 传值 | 指针类型 | 指针类型 |
| 传引用 | 指针类型 | 绑定指针类型的引用 |

#### Things to Remember
- During template type deduction, arguments that are references are treated as non-references, i.e., their reference-ness is ignored.
- When deducing types for universal reference parameters, lvalue arguments get special treatment.
- When deducing types for by-value parameters, const and/or volatile arguments are treated as non-const and non-volatile.
- During template type deduction, arguments that are array or function names decay to pointers, unless they’re used to initialize references.