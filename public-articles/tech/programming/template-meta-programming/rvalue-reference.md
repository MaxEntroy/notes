本文直来直去，通过一个简单的例子引入，讨论关于右值引用的正确理解以及std::move/std::forward的使用场景。

### 引子

我们首先来看如下代码:
```cpp
void Sum(int a, int b) {
  return a + b;
}

void Bar(int&& rr) {
  std::cout << rr << std::endl;
}

int&& rr = Sum(3, 4);
Bar(rr);  // illegal
```

这段代码逻辑非常简单，定义了一个右值引用```rr```，传递给一个函数```Bar```后者有且仅有一个右值引用的参数。代码看起来似乎没有问题，形参的类型和实参的类型一样，都是```int&&```，但实际编译失败。

那么问题来了，为什么看似一样的类型，实际却不匹配？

### 右值引用类型

为了解决上面说的问题，我们需要搞清楚一件事情，即```T&&```这个类型的语义到底是什么？

个人以为，需从以下三个角度来理解右值引用类型的语义：
- 首先，这是一个引用类型。
- 其次，这个引用类型所绑定的值类型是T。
- 最后，这个引用只能绑右值

这里我解释下这种理解的核心层次是什么：**本质是不要把左值引用和右值引用对立开，他们不是两种类型，他们是同一种类型**。
- 从类型的角度来看，不区分左值引用类型和右值引用类型，他们都是引用类型(对比值类型，指针类型)。引用类型的特点是，对于引用变量的修改会直接反映到绑定值上。
- 即不管是```T&```还是```T&&```，他们首先都是引用类型，可以绑定类型为T的变量。
- 在这个基础上，我们再来区分，```T&```类型，可以绑一个左值。```T&&```可以绑一个右值。

这其中，第2点和第3点是对绑定值的要求，即绑定值的类型要匹配，绑定值的左右值属性需要满足。

此时，我们再看上面的代码，
- 对于```Bar(int&& rr)```来说，形参是一个引用类型，绑定整型变量，绑定右值变量
- 对于```int&& rr = std::move(val)```来说，rr作为实参，不管他是什么类型，它肯定是一个左值。
- 所以，形参需要绑定右值，而实参是一个左值，无法绑定，编译失败

此时，我们再回到引子当中的问题，为什么看似一样的类型，实际却不匹配？因为对于```T&&```类型，我们不能从形式上的类型判断实参和形参类型是否一致，而是要从右值引用类型的语义判断二者是否一致。

### why std::move?

我们来修复上面代码的问题，既然绑定值需要是右值，那我们想办法生成右值即可，可增加一个```GetRValue```函数来实现。

```cpp
int Sum(int a, int b) {
  return a + b;
}

void Bar(int&& rr) {
  std::cout << rr << std::endl;
}

template <typename T>
T GetRValue(T t) {
  return t;
}

void test() {
  int&& rr = Sum(1, 2);
  Bar(GetRValue(rr));  // legal
}
```

此时，问题解决。不过既然```GetRValue```能解决问题，那么标准库提供的```std::move```还有什么作用呢？我们接着讨论。

上面的代码，我们做一点修改。

```cpp
int Sum(int a, int b) {
  return a + b;
}

void Bar(int&& rr) {
  std::cout << "rr=" << rr++ << std::endl;
}

template <typename T>
T GetRValue(T t) {
  return t;
}

void test() {
  int&& rr = Sum(1, 2);
  Bar(GetRValue(rr));
  std::cout << "rr=" << rr << std::endl;
}
// rr=3
// rr=3
```

我们发现两处rr的打印值一样，这表明Bar中对于rr做的自增没有达到预期的效果。这是Bar实现的有问题吗？
显然不是，真正的问题在于```GetRValue```类型推断的结果是[T = int]，从而返回的是值类型，发生了值拷贝，所以```Bar```中自增的变量并不是我们原始的变量，导致了效果没有达成。

如果我们把```GetRValue```换成```std::move```，问题迎刃而解。

```cpp
int Sum(int a, int b) {
  return a + b;
}

void Bar(int&& rr) {
  std::cout << "rr=" << rr++ << std::endl;
}

template <typename T>
T GetRValue(T t) {
  return t;
}

void test() {
  int&& rr = Sum(1, 2);
  Bar(std::move(rr));
  std::cout << "rr=" << rr << std::endl;
}
// rr=3
// rr=4
```

我们看下std::move的实现：

```cpp
template<typename T>
typename std::remove_reference<T>::type&& move(T&& t) {
  return static_cast<typename std::remove_reference<T>::type&&>(t);
}
```

当我们调用```std::move(rr)```时，类型推断的结果是[T = int&]，最终返回```T&&```类型，既然返回的是引用类型,那么我们```Bar```中所绑定的变量，便不是原始值的副本，从而对其的修改可以反映到原始值上。

所以，```std::move```的好处在于，即产生了右值，又保持了其引用类型的能力。而```GetRValue```则只能产生右值，而不能保证引用类型。

(2023.09 update)当然，对于上面的例子，这里的展示还不够充分。比如```GetRValue```可以产生右值，```move```也可以。但是前者不能保持引用，后者可以。那后者为什么不可以返回```T&```?即实现成如下的形式：

```cpp
template<typename T>
typename std::remove_reference<T>::type& move(T&& t) {
  return static_cast<typename std::remove_reference<T>::type&>(t);
}
```

std::move本质产生的是引用类型，它绑定的虽然是```rr```，但此时```rr```已经是```sum```结果的临时变量。不能写成上面的原因在于，```rr```虽然是一个左值，但其绑定值是右值。
同理，```move```这里产生的引用类型，需要绑定右值，只能是右值引用。

换句话说，如果返回了```T&```，语意上并不会说明这是一个可以被窃取资源的对象，相当于改变了value category. ```T&&```在保证引用类型的同时，没有改变value category.

最后，我们看下```std::move```的官方说明：
>std::move is used to indicate that an object t may be "moved from", i.e. allowing the efficient transfer of resources from t to another object.
In particular, std::move produces an xvalue expression that identifies its argument t. It is exactly equivalent to a static_cast to an rvalue reference type.

所以，我理解使用```std::move```的场景就是，如果语意上这是个 xvalue，那么我们就可以使用```std::move```来匹配其语意上的value category.

### why std::forward?

既然我们可以通过```std::move```获得右值引用，那么为什么还需要std::forward?
此时，我们换一种场景来看。

```cpp
void g(int&& v1, int& v2) {
  v1 *= 2;
  v2 *= 2;
}
```
给出上述的函数，我们要对g做一个wrapper，该如何实现？我们先看一种版本

```cpp
template<typename F, typename T1, typename T2>
void wrapper1(F f, T1&& t1, T2&& t2) {
  f(t1, t2);
}

void test() {
  int val = 3;
  int&& r = val;
  wrapper1(g, 3, r);  // illegal
}
```

关于```wrapper1```先说一点，形参类型定义为右值引用(rvalue reference to a template type parameter)，这其实是universal reference，通过reference collapsing可以保持实参的左右值属性(不详细展开，可参见cpp-primer 16.2的内容)

具体来看函数内的调用，```wrapper1```内部调用```g```，后者的第一个形参类型是```int&&```，根据我们上文的理解，这是一个引用类型，绑定整型变量，绑定右值。但是```t1```是一个左值，所以类型不匹配，编译失败。

一个自然的思路，我们使用```std::move```来产生一个右值引用类型，

```cpp
template<typename F, typename T1, typename T2>
void wrapper2(F f, T1&& t1, T2&& t2) {
  f(std::move(t1), t2);
}

void test() {
  int val = 3;
  int& r = val;
  wrapper2(g, 5, r);  // legal
}
```
这个版本没有问题，看似```std::move```解决了所有的问题。此时，我们再考虑如下的场景，我再增加一个函数```f```，它和```g```唯一的区别在于，反转了形参的类型。此时我们任然使用```wrapper2```进行间接调用

```cpp
void f(int& v1, int&& v2) {
  v1 *= 2;
  v2 *= 2;
}

void test() {
  int val = 3;
  int& r = val;
  wrapper(f, r, 5);  // illegal
}
```
此时，编译失败。因为```f```的第一个形参类型是左值引用，但是```wrapper2```内部对于```t1```做了强制转换，生成了右值引用，从而类型不匹配。

一个简单的解决办法是，再提供一个new wrapper，来完成对于第二个形参的强制转换。这么做当然可以，但是不优雅。因为
- wrapper的参数是universal reference，表明其可以接受lvalue/rvalue reference，是一种通用的类型。
- 但是函数内部却无法自动的保持实参的左右值属性，而是需要根据实际调用的函数，来支持特定版本的转换，实现不通用。

```std::forward```就是为了解决这个问题而存在的，它可以保持参数(实参)在传递过程中的左右值属性。因为形参都是左值，需要转化为其对应的实参的左右值属性。

我们看下面的实现：

```cpp
template<typename F, typename T1, typename T2>
void wrapper3(F f, T1&& t1, T2&& t2) {
  f(std::forward<T1>(t1), std::foward<T2>(t2));
}

void f(int& v1, int&& v2) {
  v1 *= 2;
  v2 *= 2;
}

void g(int&& v1, int& v2) {
  v1 *= 2;
  v2 *= 2;
}

void test() {
  int val = 3;
  int& r = val;
  wrapper3(g, 5, r);  // legal
  wrapper3(f, r, 5);  // legal
}
```

- 首先，实参可能是左值，也可能是右值。
- 但是，形参一定是左值，不管它是一个左值引用，还是右值引用。一个变量是左值还是右值，和它是什么类型没有关系。
  - 这里主要的问题就是```T&& rr = sum(1, 2);```这种表达。右值引用自然可以绑定右值。
  - 但右值引用```rr```本身是一个左值。这就导致参数继续向下传递时，丢失了实参的右值属性。
  - 此处并不能使用```std::move```，因为对于```T& r = foo;```左值引用，实参是左值，不能转化为右值引用。
  - 所以，需要一个工具，可以将这个左值形参(左值引用或者右值引用)，**自动保持和其实参的左右值类型一致**，即：
    - 形参是左值，类型是左值引用。转化后，变成右值，类型是左值引用。
    - 形参是左值，类型是右值引用。转化后，变成右值，类型是右值引用。
    - 注意，这里再次强调，这个形参不管其是左值引用还是右值引用，它都是一个左值。
    - 所以，我们要将它的左右值属性，转化为其绑定值的左右值属性。
    - 从而可以继续向下传递。

这里我们再来讨论一下，如上所言，```std::forward```和```std::move```一样，会产生右值，但类型可以是左值引用或者右值引用。那么问题来了，
既然```std::forward```一定产生右值，不管f/g，都有左值引用的参数，无法绑定右值，代码又怎么能编译通过呢？虽然这个值的类型，和实参保持了一致，但是值本身的
左右值属性，无法和嵌套函数的类型兼容。

### Value categories

Each C++ expression (an operator with its operands, a literal, a variable name, etc.) is characterized by two independent properties: a type and a value category. 

Each expression has some non-reference type, and each expression belongs to exactly one of the three primary value categories: prvalue, xvalue, and lvalue.

- a glvalue ("generalized" lvalue) is an expression whose evaluation determines the identity of an object or function;
- a prvalue (“pure” rvalue) is an expression whose evaluation
  - computes the value of an operand of a built-in operator (such prvalue has no result object), or
  - initializes an object (such prvalue is said to have a result object).
    - The result object may be a variable, an object created by new-expression, a temporary created by temporary materialization, or a member thereof. Note that non-void discarded expressions have a result object (the materialized temporary). Also, every class and array prvalue has a result object except when it is the operand of decltype;
- an xvalue (an "eXpiring" value) is a glvalue that denotes an object whose resources can be reused;
- an lvalue (so-called, historically, because lvalues could appear on the left-hand side of an assignment expression) is a glvalue that is not an xvalue;
- an rvalue (so-called, historically, because rvalues could appear on the right-hand side of an assignment expression) is a prvalue or an xvalue.

Note: this taxonomy went through significant changes with past C++ standard revisions, see History below for details.

最后这条note很重要，新的标准和过去右显著区别，需要注意。当然，主要是引入了move/forward相关的适配。

下面这张图给出了他们之间的关系，以及各自的特点:

<img width="500"  src="img/value_cat.png"/>

我们再回到```std::move```的定义：
>In particular, std::move produces an xvalue expression that identifies its argument t. It is exactly equivalent to a static_cast to an rvalue reference type.

对于```std::forward```而言，它的返回值是```static_cast<T&&>(t)```，根据xvalue的表达式的定义:
- a cast expression to rvalue reference to object type, such as static_cast<char&&>(x);

Both ```std::move``` and ```std::forward``` produce an xvalue expression.

那么我们再来看xvalue的特性：
- an xvalue (an “eXpiring” value) is a glvalue that denotes an object whose resources can be reused;
- identity
- moveable from

这里矫正了我们对于左右值属性的认知，我们再次回到代码中
```cpp
template<typename F, typename T1, typename T2>
void wrapper3(F f, T1&& t1, T2&& t2) {
  f(std::forward<T1>(t1), std::foward<T2>(t2));
}

void f(int& v1, int&& v2) {
  v1 *= 2;
  v2 *= 2;
}

void g(int&& v1, int& v2) {
  v1 *= 2;
  v2 *= 2;
}
```

```std::forward```产生了xvalue，具有identity的能力，自然左值引用可以绑定；同时具有moveable from的能力，右值引用可以绑定。注意，这里其实再说右值引用其实就不对了，
这里应该说成是可以绑定moveable from的对象的引用。

最后，再看一下```std::forward```的官方说明：
>forwards the argument to another function with the value category it had when passed to the calling function.

对比一下，```std::move```和```std::forward```都产生xvalue，但是前者只强调moveable from这个属性。后者同时强调identity和moveable from这两个属性，所以它通用，那么自然可以配合universal reference来接受左值实参，和右值实参。

```c++
template<class T>
void wrapper(T&& arg)
{
    // arg is always lvalue
    foo(std::forward<T>(arg)); // Forward as lvalue or as rvalue, depending on T
}
```

这个就是它最经典的例子，融化在血液中
- universal reference
- std::forward
- that makes universal reference a forwarding reference that preserve the **value category** of a function argument.

### std::forward实现

关于std::forward的实现我们多讨论一点，考虑下面两种std::forward实现
```cpp
template<typename T>
T&& myforward(T& param) {
  return static_cast<T&&>(param);
}

template< class T >
T&& forward( typename std::remove_reference<T>::type& t ) noexcept {
  static_cast<T&&>(t);
}
```

这里我们着重讨论，为什么第一种不行？我们可以替换一下前面的demo看一下效果
```cpp
void flip(F f, T1&& t1, T2&& t2) {
  f(myforward(t1) , myforward(t2));
  //f(std::forward<T1>(t1) , std::forward<T2>(t2));
}

void g(int&& v1, int& v2) {
  v1 *= 2;
  v2 *= 2;
}

void test() {
  int val = 3;
  int& r = val;
  flip(g, 5, r); // illegal
}
```

代码在调用flip处编译失败，我们看一下具体的原因
- 第一个参数是5，右值，类型推断[T1=int]，t1是一个左值，此时```param```的类型为```int&```，类型匹配
- 第二个参数是r，左值，类型推断[T2=int&]，t2是一个左值，此时```param```的类型为```int&&```，需要绑定一个右值，类型失配

此时，我们看std的实现，```std::remove_reference<T>::type```可以保证参数t的类型一定是左值引用，这里为什么必须是左值引用是因为，```forward```一般用于转发参数，已经接受到的参数，一定是个左值，所以这里只能使用左值引用接受。

最后，我们做一个简单的小结：
- 右值引用类型的匹配需要根据语义确定，而不是形式上的一致性。
- ```std::move```在产生右值的基础上，保证了其引用特性。
- ```std::forward```会根据传入参数的左右值属性，产生对应的引用类型。
- universal reference通常和```std::forward```配合使用，前者保证形参类型的左右值属性，后者保证形参传递过程中的左右值属性。

### Ref

[Value categories](https://en.cppreference.com/w/cpp/language/value_category)<br>
[std::move](https://en.cppreference.com/w/cpp/utility/move)<br>
[std::forward](https://en.cppreference.com/w/cpp/utility/forward)<br>
[What are rvalues, lvalues, xvalues, glvalues, and prvalues?](https://stackoverflow.com/questions/3601602/what-are-rvalues-lvalues-xvalues-glvalues-and-prvalues)<br>
[Forwarding references](https://en.cppreference.com/w/cpp/language/reference#Forwarding_references)