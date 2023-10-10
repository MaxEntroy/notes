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