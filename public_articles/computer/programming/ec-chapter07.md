## Chapter07 Templates and Generic Programming

### Intro

- C++ template mechanism is itself Turing-complete: it can be used to compute any computable value.
- that lead to template metaprogramming: the creation of programs that execute inside C++ compilers and that stop running when compilation is complet 

本章开始这里，scott meyers先介绍了模板机制的理论基础，然后借此引出了模板元编程。关于图灵完备，我多说一点，这个是用来描述规则的，即语言本身。语言本身如果可以提供无限内存，提供if-else控制，循环这样的能力，那么他就是图灵完备的，或者说如果它能模拟图灵机亦可。

这里多讨论一个点是，关于OO，大家用的都很熟，也是一般解决问题的办法。但是回过头来我们可以考虑stl的容器，都提供遍历的能力，即支持begin/end接口，那为什么不采用OO的设计，给出一个Container基类，然后再派生Sequential Container/Associative Container这样的东西呢？这个问题按下不表，等我们学习完成meta programming之后，再来对比这二者的区别，相信届时大家对于不同编程范式优劣会有更清晰的认识。

### Item 41: Understand implicit interfaces and compile-time polymorphism

- The world of object-oriented programming revolves around explicit interfaces and runtime polymorphism
- The world of templates and generic programming is fundamentally different. 
  - In that world, explicit interfaces and runtime polymorphism continue to exist, but they're less important. 
  - Instead, implicit interfaces and compile-time polymorphism move to the fore.

```cpp
template<typename T>
void doProcessing(T& w) {
  if (w.size() > 10 and w != someNastyWidget) {
    T tmp(w);
    tmp.normalize();
    tmp.swap(w);
  }
}
```

我这里先说下直观理解，oo的世界，接口(类)是显示定义的，是直接的。我们通过接口提供的功能，去实现对应的逻辑功能。
但是，模板的世界刚好反过来，先有逻辑功能，通过其中的表达式观察出模板对于类型的抽象。这也就是所谓的隐式接口。
模板如果想实例化，对应的类型必须支持这些隐式接口。

静态多态，其实就是模板实例化过程中，根据实际调用的类型，生成对应的函数。我们知道，模板只是模板，不管是模板函数，还是模板类，他们都不是函数，也不是类，没有执行的能力，只有实例化之后才行。

作者也给出了如下总结：
- What's important is that the set of expressions that must be valid in order for the template to compile is the implicit interface that T must support.
- Instantiating function templates with different template parameters leads to different functions being called, this is known as
compile-time polymorphism.

当然，隐式接口的概念确实不直接，对于这种东西，我们只能通过刻意练习来加深我们对其的理解。作者也进一步给出了解释

- An implicit interface is quite different. It is not based on function signatures. Rather, it consists of valid expressions. 
- The expressions themselves may look complicated, but the constraints they impose are generally straightforward

- Things to Remember
  - Both classes and templates support interfaces and polymorphism.
  - For classes, interfaces are explicit and centered on function signatures. Polymorphism occurs at
runtime through virtual functions.
  - For template parameters, interfaces are implicit and based on valid expressions. Polymorphism
occurs during compilation through template instantiation and function overloading resolution.

### Item45: Use Member function templates to accept all compatible types.

It is about type conversion.

- why: Smart pointers don't do well in supporting implicit conversions, because 
there is no inherent relationship among different instantiations of the same template.
We have to program them explicitly.

- best practise
  - subitem1: we need a ctor template.
  - subitem2: The generalized copy ctor is not declared explicit.
  - subitem3: We use the member initialization list to initialize SmartPtr<T>'s data member of type T* with the type U*
  held by the SmartPtr<U>.
  - subitem4: Another common role for them is in support for assignment.
  - subitem5: If you declare member templates for generalized copy construction or generalized assignment, you'll
still need to declare the normal copy constructor and copy assignment operator, too.(Member function templates are wonderful things, but they don't alter the basic rules of the language.)

```cpp
template<typename T>
class SmartPtr {
 public:
  template<typename U>
  SmartPtr(const SmartPtr<U>& other) // initialize this held ptr
      : heldPtr(other.get()) { ... } // with other's held ptr
  T* get() const { return heldPtr; }
...
 private: // built-in pointer held
  T *heldPtr; // by the SmartPtr
};
```
