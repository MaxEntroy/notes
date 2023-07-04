

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

### Continuation

>A function, used as a parameter by another function, and typicall used to abstract away
from "the rest of the compuation", or "what to di to finish a task".

- 形式上：函数作为参数
- 内容上：抽象了一个任务的剩余计算