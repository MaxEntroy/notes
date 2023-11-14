### Tail call

>In computer science, a tail call is a subroutine call performed as the final action of a procedure.

如上所言，tail call要求subroutine call作为final action. 那么，我们看下面的代码是tail call吗？

```cpp
int fact(int n) {
  if (n <= 1)
    return 1;
  else
    return n * fact(n-1);
}
```

不是，因为函数的final action是一条乘法指令，并不是一个subroutine call. 只不过，其中的一个乘数依赖于subroutine call，但final action本身并不是subroutine call.

```cpp
/**
 * fact(5)
 * 5 * fact(4)
 *     4 * fact(3)
 *         3 * fact(2)
 *             2 * fact(1)
 *                 1
 *             2
 *         6
 *     24
 * 120
 */
```

上面的注释模拟了函数执行的过程，其中
- 每一个subroutine call都伴随了一个push stack frame，当然也有stack frame的ctor。
- 回溯的时候伴随了pop stack frame，以及后者的dtor

那么tail call的写法是怎样？参见下面的代码

```cpp
int tail_fact(int n, int ans) {
  if (n == 1)
    return ans;
  else
    return tail_fact(n - 1, n * ans);
}

int tail_fact(int n) {
  return tail_fact(n ,1);
}

/**
 * tail_fact(5, 1)
 * tail_fact(4, 5)
 * tail_fact(3, 20)
 * tail_fact(2, 60)
 * tail_fact(1, 120)
 * return 120
 * /
```

- 很明显，这个实现没有stack frame这个东西，因此整个的调用过程无需考虑这一部分的开销
- 跟进一步，这个代码看起来"**更干净**"，因为它没有除了argument之外的任何local var，无状态

简单总结下：
- 尾递归，比线性递归多一个参数，这个参数是上一次调用函数得到的结果
- 尾递归每次调用都在收集结果，避免了线性递归不收集结果只能依次展开消耗内存的坏处。

### Continuation and CPS(Continuation-Passing-Style)

>A function, used as a parameter by another function, and typicall used to abstract away
from "the rest of the compuation", or "what to di to finish a task".

- 形式上：函数作为参数
- 内容上：抽象了一个任务的剩余计算

>A function of type t1 -> t2 expects to be applied to an argument value of type t1, and returns a value of type t2.
We will refer to this as a direct-style function.

>A cps version of a such a function expects to be applied to a value of type t1 and a continuation of type t2 -> ans, where 
ans is a type of answers or final results.

direct style没什么好说的，使用函数参数，完成计算，返回结果。cps style则不是直接完成所有的计算，而是将计算拆分成不同的计算流(控制流)，每次只做部分计算。
剩余的计算交给continuation

- Slogan: a direct-style function evaluates its argument and returns a value.
- Slogan: a cps function evaluates its argument and calls its continuation with a value.

下面看一个例子

```cpp
void test() {
  auto sum_of_squares_direct_style = [](int lhs, int rhs) {
    return lhs * lhs + rhs * rhs;
  };
  std::cout << sum_of_squares_direct_style(3, 4) << std::endl;

  auto add_cps = [](int lhs, int rhs, auto&& continuation) {
    return continuation(lhs + rhs);
  };
  auto mul_cps = [](int lhs, int rhs, auto&& continuation) {
    return continuation(lhs * rhs);
  };
  auto sum_of_squares_cps_style = [&](int lhs, int rhs) {
    return mul_cps(lhs, lhs, [&](int lhs_sum){
        return mul_cps(rhs, rhs, [&](int rhs_sum){
            return add_cps(lhs_sum, rhs_sum, [&](int ans) { return ans;});
        });
    });
  };
  std::cout << sum_of_squares_cps_style(3, 4) << std::endl;
}
```

计算两数的平方和，direct_style直接完成了计算。cps style首先定了control flow operator，然后使用cps style完成计算。后者更强调control flow，我们可以清楚的发现总共有3次计算。每次的lambda计算和tail call的方式一致，将本次计算的结果，传递给下一个continuation.

>Writing a function in cps-style may help to emphasize the control flow, and indeed cps can be a good way to implement a complex pattern of control flow.

对于cps的编码方式，我认为有如下两个优点

- control flow is made explicit
- saves stack space