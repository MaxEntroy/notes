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

If we change the type of ParamType from ```T&``` to ```const T&```, things change a little, but not in any surprising way. There is no longer a need for const to be deduced as part of ```T```

```cpp
template<typename T>
void f(const T& param);

int x = 27;
const int cx = x;
const int& rx = x;

f(x);  // x is an int, T is int, ParamType is int&
f(cx); // cx is a const int, T is int, ParamType is const int&
f(rx); // rx is a reference to x as a const int, T is int, ParamType is const int&
```

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

When ParamType is neither a pointer nor a reference, we’re dealing with pass-by-value:

- As before, if expr’s type is a reference, ignore the reference part.
- If, after ignoring expr’s reference-ness, expr is const, ignore that, too.

Hence,

```cpp
template<typename T>
void f(T param);  // param is now pass by value

int x = 27;
const int cx = x;
const int& rx = x;

f(x);  // x is an int, T is int, ParamType is int
f(cx); // cx is a const int, T is const int, ParamType is int
f(rx); // rx is a reference to x as a const int, T is int, ParamType is int
```

- It is importance to recognize that const is ignored only for by-value parameters.
- As we have seen, for parameters that are references-to- or pointers-to- const, the constness is preserved during type deduction.

```cpp
template<tyname T>
void f(param);  

const char* const ptr = "Fun with pointers."  // ptr is const pointer to const object

f(ptr);  // pass arg of type const char* const
```

- The constness of what ptr points to is preserved during type deduction, but the constness of ptr itself is ignored.
- The type deduced for params will be ```const char*```

#### Case4: Array Arguments.

- It's that array types are different from pointer types.

```cpp
const char name[] = "J, P. Briggs.";  // name's type is const char[13]

const char* ptrToName = name;  // array decays to pointer
```
 
Because of the array-to-pointer decay rule, the code compiles.

```cpp
template<typename T>
void f(T param);  // template with by-value parameter

f(name);  // what types are deduced for T and param?
```

Because of the array-to-pointer decay rule, the type of an array that’s passed to a template function **by value** is deduced to be a pointer type.

But now comes a curve ball. Althrough functions can't declare parameters that are truly arrays(because of the array-to-pointer-decay rule), 

```cpp
void myFunc(int param[]);

void myFunc(int* param);  // same function as above
```

they can declare paremeters that are references to arrays. 

```cpp
template<typename T>
void f(T& param);  // template with by-reference parameter

f(name);  // pass array to f
```

the type deduced for ```T``` is the actual type of the array, ```T``` is deduced to be ```const char[13]``` and ```ParamType``` is ```const char (&)[13]```(reference to this array)

Interestingly, the ability to declare **references to arrays** enables creation of a template that deduces the number of elements that an array contains:

```cpp
template<typename T, std::size_t N>
constexpr std::size_t arrSize(T (&)[N]) noexcept {
  return N;
}
```

```constexpr``` makes its result available during compilation. That makes it possible to declare an array
with the same number of elements as a second array whose size is computed from a braced initializer.

```cpp
int keyVals[] = {1, 3, 5, 7, 9};

int mappedVals[arrSize(keyVals)];
```

The so-called compile-time computation is actually done with the help of type deduction.

#### Case5: Function Arguments

Arrars are not the only things in c++ that can decay in to pointers. Function types can decay into function
pointers, everything we've discussed regarding type deduction for arrays applies to type deduction for functions and their decay into function pointers.

```cpp
void someFunc(int, double); // type is void(int, double)

template<typename T>
void f1(T param);   // template with by-value parameter

template<typename T>
void f2(T& param);  // template with by-reference parameter

f1(someFunc)  // type deduced for T is (*)(int, double) and ParamType is (*)(int, double)

f2(someFunc)  // type deduced for T is (*)(int, double) and ParamType is (&)(int, double)
```

#### Things to Remember
- During template type deduction, arguments that are references are treated as non-references, i.e., their reference-ness is ignored.
- When deducing types for universal reference parameters, lvalue arguments get special treatment.
- When deducing types for by-value parameters, const and/or volatile arguments are treated as non-const and non-volatile.
- During template type deduction, arguments that are array or function names decay to pointers, unless they’re used to initialize references.

### Item2 Understand auto type deduction.

**auto type deduction is template type deduction**. That's true, there is a direct mapping between template type deduction and auto type deduction.

In item1, template type deduction is explained using this general function template:

```cpp
template<typename T>
void f(ParamType param);
```

and this general call:

```cpp
f(expr)
```

In the call to ```f```, compilers use ```expr``` to deduce types for ```T``` and ```ParamType```.

When a variable is declared using ```auto```, ```auto``` plays the role of ```T``` in template, and the type specifier for the variable acts as ParamType.

```cpp
auto x = 27;

template<typename T>
void func_for_x(T param);  // conceptual template for deducing x's type

func_for_x(27); // conceptual call
```

```cpp
const auto cx = x;

template<typename T>
void func_for_cx(const T cx);  // conceptual template for deducing cx's type

func_for_cx(x);  // // conceptual call
```

```cpp
const auto& rx = x;

template<typename T>
void func_for_rx(const T& rx);  // conceptual template for deducing rx's type

func_for_rx(x);  // conceptual call
```

**Deducing types for auto is almost deducing types for template.**

- Item1 divides template type deduction into 3 cases, based on the characteristics of ```ParamType```.
- There are also 3 cases for auto type deduction, the type specifier take place of the ```ParamType```.
  - Case 1: The type specifier is a pointer or reference, not a universal reference.
  - Case 2: Tye type specifier is a universal reference.
  - Case 3: The type specifier is neither a pointer nor a reference.

We've already seen examples of cases 1 and 3:

```cpp
auto x = 27;
const auto cx = x;
const auto& rx = x;
```

Case 2 works as you'd expected:

```cpp
auto&& uref1 = x;  // x is int and lvalue uref1's type is int&

auto&& uref2 = cx;  // cx is int and lvalue, uref2's type is int&

auto&& uref3 = 27;  // 27 is int and rvalue, uref3's type is int&&
```