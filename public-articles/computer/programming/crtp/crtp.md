### 题外话

一晃两个月过去，自己一篇文章没写，啥都没沉淀。上半年到目前为止，其实东西到是看了一些，pb arena allocation(done)，csapp allocator(pending)，coroutine related(done)，the art of writting efficient programmes(underway)，线上的问题复盘(总共两次，写了一篇，pending)。除了这些，自己手头还有工作，也准备系统总结前一阶段的工作，发篇km，多线并行，其实不写，就是觉得自己着实抽不出时间，再加上反正都看懂了嘛，就不愿意再沉淀了。

当然，说白了还是借口，我心里其实明白，又开始拖延症，觉得自己看懂了就行。技术细节，觉得自己看懂了和实现明白还是两回事，得逼自己都沉淀下来才行。

### 引子

最近的一项任务觉得可以用crtp来搞，准备研究一下这个东西。其实这玩意没少用，内部的rpc框架单例就是用crtp手法做的。会用没问题，但是对于其背后的设计思路，最佳实践，不甚了解，决定出手搞一下。

下面直接上wiki的内容[Curiously recurring template pattern](https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern)

#### General form

```cpp
// The Curiously Recurring Template Pattern (CRTP)
template <class T>
class X
{
    // methods within Base can use template to access members of Derived
};

class A : public X<A>
{
    // ...
};
```

从形式上来说，有一点反直觉，简单说，A存在，必须X<A>先存在。X<A>存在，必须A存在。鸡生蛋，蛋生鸡问题。我们看看wiki是怎么解释的。

>In the above example, the class ```X<A>```, though declared before the existence of the class ```A``` is known by the compiler (i.e., before class ```A``` is declared), is not actually instantiated by the compiler until it is actually called by some later code which occurs after the declaration of class ```A``` (not shown in the above example), so that at the time the class ```X<A>```is instantiated, the declaration of class ```A``` is known.

简单说，虽然定义上又先后，但class ```X```是一个模板，它不是一个类，所以上面说的依赖顺序问题，不存在。一旦实例化后，class ```A```和class ```X<A>``` 这两个类型编译器同时感知到，没问题。

下面说一下直观地认识，看下面代码：
```cpp
class MyString : public std::vector<MyString> {};
```

std::vector是一个模板，我这里定义一个MyString，继承自std::vector<MyString>。语义说不通，父类是一个vector string，派生类是一个string，语义上看起来都冲突了。这是一个很差的例子，没错，我就是要举一个这样的例子，和后面正确的例子对比看，我们才知道它到底怎么用。

再说另一个认识，代码注释也写了，基类拿到了派生类的类型，这样它可以访问成员。但是，这里千万要注意的是，```X<A>```具备获取A的能力，但是它肯定不是A。我们都知道public inheritance 表达的是一种is-a关系，可以认为```A``` is-a ```X<A>```，但反过来不行。所以，假如我们有一个```X<A>```的对象，它可以访问A的成员嘛？(注意，这里说的是在A中单独定义的成员)分两种情形
- 如果是static member，没问题。
- 但是，如果不是static member，也分两种情形
    - 如果是通过X<A>访问成员，有问题。因为X<A>不是A
    - 如果是通过A访问成员，自然没问题。
    - 这两点其实通过Static polymorphism下面讲解更好一点，这里不再赘述。

#### Static polymorphism

```cpp
template <class T> 
struct Base
{
    void interface()
    {
        // ...
        static_cast<T*>(this)->implementation();
        // ...
    }

    static void static_func()
    {
        // ...
        T::static_sub_func();
        // ...
    }
};

struct Derived : Base<Derived>
{
    void implementation();
    static void static_sub_func();
};
```

看起来很完美的一个例子，是吧。派生了给实现，父类给接口，和动态多态的思路一致。不过这个东西很容易用错。

我们可以先看一个错误的使用方式：
```cpp
void Test() {
  Base<Derived> this_is_a_base_object;
  this_is_a_base_object.interface()
}
```

这个使用有啥问题呢？问题就出在，this_is_a_base_object这个对象，它的类型是Base<Derived>，不是Derived。
那么interface内部，你把一个父类对象，强制转化为一个派生类对象，显然会导致undefined behavior.

正确的用法如下

```cpp
  Derived this_is_a_derived_object;
  this_is_a_derived_object.interface()
```

这么写自然没问题，但是还有问题，说好了叫静态多态，那你这个多态到底怎么体现？我们知道，动态多态，是通过基类的指针或者引用，指向继承体系的对象，然后调用虚函数体现的。那么，在静态多态里面，和基类指针或者引用，类似的这个角色在哪里？

我在[惯用法之CRTP](https://mp.weixin.qq.com/s/giSDMWLO0d7t3fX-ZC7eVg)看到下面这种用法

```cpp
#include <iostream>

#include <iostream>

template <typename T>
class Base{
 public:
  void interface(){
    static_cast<T*>(this)->imp();
  }
  void imp(){
    std::cout << "in Base::imp" << std::endl;
  }
};

class Derived1 : public Base<Derived1> {
 public:
  void imp(){
    std::cout << "in Derived1::imp" << std::endl;
  }
};

class Derived2 : public Base<Derived2> {
 public:
  void imp(){
    std::cout << "in Derived2::imp" << std::endl;
  }
};

class Derived3 : public Base<Derived3>{};

template <typename T>
void fun(T& base){
    base.interface();
}


int main(){
  Derived1 d1;
  Derived2 d2;
  Derived3 d3;

  fun(d1);
  fun(d2);
  fun(d3);

  return 0;
}
```

这个看起来好像没问题，```T& base```充当类类似动态多态中，基类指针的用法。但问题是，既然你这么写，那我下面这种写法不行吗？

```cpp
template <typename T>
void fun(T& base){
    base.impl();
}
```

所以上面代码的问题是，派生类当中的实现应该都是private限定，那么就没问题了，看起来和NVI idiom一致。只不过前者的实现是虚函数，后者的实现是模板。

我再总结下静态多态的用法。
```cpp
template <class T> 
class Base {
  public:
    void interface()
    {
        // ...
        static_cast<T*>(this)->implementation();
        // ...
    }
};

class Derived1 : Base<Derived1> {
  private:
    void implementation() {}
};

class Derived2 : Base<Derived2> {
  private:
    void implementation() {}
};

template <typename T>
void fun(T& base) {
    base.interface();
}

void test() {
    Derived1 der1;
    Derived2 der2;
    fun(der1);
    fun(der2);
}
```

这里我最后再说一点，也是我之前一直没理解到的地方，我们看一下它这个多态的调用，即```fun```的设计，为什么是个模板？