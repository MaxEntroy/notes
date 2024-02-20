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

#### inline with tempalte

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