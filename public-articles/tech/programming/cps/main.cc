#include <iostream>

namespace tail_call {

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
 * 可以看到这个栈的深度，每一次递归调用，stack frame需要保存临时变量。
 * 最后栈回溯的时候，弹出stack frame，拿到中间值，计算返回。
 */
int fact(int n) {
  if (n <= 1)
    return 1;
  else
    return n * fact(n-1);
}


/**
 * tail_fact(5, 1)
 * tail_fact(4, 5)
 * tail_fact(3, 20)
 * tail_fact(2, 60)
 * tail_fact(1, 120)
 * return 120
 *
 * 这个没有stack frame，因为没有中间计算过程中需要保存的临时变量。
 * 所以，没有stack frame相关操作的开销，使的tail call变得高效
 *
 * 同时，我们也可以看出来，tail call的写法，没有状态，即没有需要保存的临时变量，中间状态。
 * 每一步的输入，完成计算后的输出，作为下一步计算的输入。
 *
 */
int tail_fact(int n, int ans) {
  if (n == 1)
    return ans;
  else
    return tail_fact(n - 1, n * ans);
}

int tail_fact(int n) {
  return tail_fact(n ,1);
}

void test() {
  std::cout << fact(5) << std::endl;
  std::cout << tail_fact(5) << std::endl;
}

}  // namespace tail_call

namespace cps {


}  // namespace cps

int main(void) {
  tail_call::test();
  return 0;
}
