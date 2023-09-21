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

这里我们从长计议，对于```void Bar(int&& rr);```来说，实参需要传入rvalue。
- ```GetRValue```函数通过副本拷贝，产生了一个右值。
- 这里我有一个误区是：**误以为只要是函数返回，都会产生一个右值**。显然不是，只有值拷贝，即返回```T```类型时，才会有拷贝。
- 对于上面这个例子，虽然```move```是一个函数调用，但是只要返回引用，不管lvalue ref/rvalue ref，都不会产生副本，也就不会产生右值。
- 所以，上面这种写法不行的原因是，没有产生rvalue ref.

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
此时，编译失败。因为```f```的第一个形参类型是左值引入，但是```wrapper2```内部对于```t1```做了强制转换，生成了右值引用，从而类型不匹配。

一个简单的解决办法是，再提供一个new wrapper，来完成对于第二个形参的强制转换。这么做当然可以，但是不优雅。因为
- wrapper的参数是universal reference，表明其可以接受lvalue/rvalue reference，是一种通用的类型。
- 但是函数内部却无法自动的保持实参的左右值属性，而是需要根据实际调用的函数，来支持特定版本的转换，实现不通用。

```std::forward```就是为了解决这个问题而存在的，它可以保持参数(实参)在传递过程中的左右值属性。因为形参都是左值，需要转化为其对应的实参的左右值属性。

(2023.09 update)这句话怎么理解？
- 首先，实参可能是左值，也可能是右值。
- 但是，形参一定是左值，不管它是一个左值引用，还是右值引用。一个变量是左值还是右值，和它是什么类型没有关系。
  - 这里主要的问题就是```T&& rr = sum(1, 2);```这种表达。右值引用自然可以绑定右值。
  - 但右值引用```rr```本身是一个左值。这就导致参数继续向下传递时，丢失了实参的右值属性。
  - 此处并不能使用```std::move```，因为对于```T& r = foo;```左值引用，实参是左值，不能转化为右值引用。
  - 所以，需要一个工具，可以将这个左值形参(左值引用或者右值引用)，**自动保持和其实参的左右值类型一致**，即：
    - 形参是左值，类型是左值引用，转化后，还是左值引用。
    - 形参是左值，类型是右值引用，转化后，变成右值引用。
    - 注意，这里再次强调，这个形参不管其是左值引用还是右值引用，它都是一个左值。
    - 所以，我们要将它的左右值属性，转化为其绑定值的左右值属性。
    - 从而可以继续向下传递。

我们再看下面的实现

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

关于```std::forward```，cpp-primer有如下表达：
- Using ```std::forward``` to Preserve Type Information in a Call
- ```std::forward``` returns an rvalue reference to that explicit argument type. 
  - That is, the return type of ```forward<T>``` is T&&.


至于```std::move```为什么做不到，我们需要对比下二者的实现

```cpp
template<typename T>
typename remove_reference<T>::type&& move(T&& t) {
  return static_cast<typename remove_reference<T>::type&&>(t);
}

template< class T >
T&& forward( typename std::remove_reference<T>::type& t ) noexcept {
  static_cast<T&&>(t);
}
```

从实现中我们可以很明显看出来，```std::move```的返回值一定是右值引用，但是```std::forward```的返回值不一定
- 当实参为右值时，模板参数类型推断为```T```，返回值为```T&&```
- 当实参为左值时，模板参数类型推断为```T&```，返回值为```T&```

所以```std::forward```的返回值保持了实参的左右值属性。而```std::move```则没有这个能力。

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