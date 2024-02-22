### Variadic Templates

A variadic template is a template function or class that can take **a varying number of parameters**.
The varying parameters are known as a parameter pack, There are two kinds of parameter packs:
- A template parameter pack represents zero or more
template parameters
- A function parameter pack represents zero or more
function parameters.

```cpp
// Args is a template parameter pack; rest is a function parameter pack
// Args represents zero or more template type parameters
// rest represents zero or more function parameters
template <typename T, typename... Args>
void foo(const T &t, const Args& ... rest);
```

####  Writing a Variadic Function Template

Best practice: Variadic functions are used when we know neither the number nor the types of the arguments
we want to process. 

- Variadic functions are often recursive (§ 6.3.2, p. 227). The first call processes the
first argument in the pack and calls itself on the remaining arguments
- A declaration for the nonvariadic version of print must be in scope when
the variadic version is defined. Otherwise, the variadic function will recurse
indefinitely.

#### Pack Expansion

Aside from taking its size, the only other thing we can do with a parameter pack is to
expand it.

Expanding a pack separates the pack into its constituent elements,
applying the pattern to each element as it does so.

### inline with tempalte

According to [Does it make any sense to use inline keyword with templates?](https://stackoverflow.com/questions/10535667/does-it-make-any-sense-to-use-inline-keyword-with-templates)

- For non fully specialized function templates, i.e. ones that carry at least one unknown type, you can omit inline, and not receive errors, but still they are not inline. 
- For full specializations, i.e. ones that use only known types, you cannot omit it.

```cpp
#ifndef TPL_H
#define TPL_H
template<class T> void f(T) {}
template<class T> inline T g(T) {}

template<> inline void f<>(int) {} // OK: inline
template<> int g<>(int) {} // error: not inline
#endif
```

But, I test the code below in my test env. It works well. So, the conclusion may not be true.

I check the cppreference for more details,
>Whether an explicit specialization of a function or variable(since C++14) template is inline/constexpr(since C++11)/constinit/consteval(since C++20) is determined by the explicit specialization itself, regardless of whether the primary template is declared with that specifier. Similarly, attributes appearing in the declaration of a template have no effect on an explicit specialization of that template:(since C++11)

```cpp
template<class T>
void f(T) { /* ... */ }
template<>
inline void f<>(int) { /* ... */ } // OK, inline
 
template<class T>
inline T g(T) { /* ... */ }
template<>
int g<>(int) { /* ... */ }         // OK, not inline
 
template<typename>
[[noreturn]] void h([[maybe_unused]] int i);
template<> void h<int>(int i)
{
    // [[noreturn]] has no effect, but [[maybe_unused]] has
}
```

The above code shows that it dost not make any sense to use ```inline``` keyword with templates.

But, as proposed rule of thumb: Write inline if you mean it and just be consistent. It makes you think less about whether to or not to just because you can. (This rule of thumb is conforming to Vandevoorde's/Josuttis's C++ Template: The Complete Guide).

So, I think it's ok to use ```inline``` keyword with templates just as other scenarios.

[Explicit (full) template specialization](https://en.cppreference.com/w/cpp/language/template_specialization)<br>
[Does it make any sense to use inline keyword with templates?](https://stackoverflow.com/questions/10535667/does-it-make-any-sense-to-use-inline-keyword-with-templates)

### ODR.

The episode is related to last episode.

- Only one definition of any variable, function, class type, enumeration type, concept(since C++20) or template is allowed in any one translation unit (some of these may have multiple declarations, but only one definition is allowed).
- One and only one definition of every non-inline function or variable that is odr-used (see below) is required to appear in the entire program (including any standard and user-defined libraries). The compiler is not required to diagnose this violation, but the behavior of the program that violates it is undefined.

There can be **more than one definition** in a program of each of the following: class type, enumeration type, **inline function**, **inline variable(since C++17),** templated entity (template or member of template, but not full template specialization).

The reason why someone believes that it does make sense to use ```inline``` with template is that it doesn't violate ODR. Because it's an inline template which means there can be more than one definition.

But explicit declaration with ```inline``` doesn't mean the template will be inlining. No ```inline``` declaration doesn't mean the template will not be inlining too. 

That ```inline``` template can violate ODR is true, but ```inline``` declaration has nothing to do with it.

Write inline if you mean it and just be consistent. That's all.

### Constraining templates

- ```std::enable_if``` gives you a way to force compilers to behave as if a particular tem‐
plate didn’t exist. Such templates are said to be disabled. 
- By default, all templates are enabled, but a template using std::enable_if is enabled only if the condition speci‐
fied by std::enable_if is satisfied. 

If a template is not enabled, template initialization will not be performed and the corresponding function will not be generated.

Obviously, there is no corresponding function label. The subsequent function call will fail and we'll get an error messasge ```no matching function for call to XXX```.

```cpp
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

  // std::string str = "hello";
  // Print(str);
  return 0;
}
```