## 搜索

### 二分查找

#### [704.Binary Search](https://leetcode.com/problems/binary-search/)

- 两分查找的基本实现
- 前提数组有序
- 递归实现，注意两个边界条件。非递归实现也需要掌握

#### [349.Intersection of Two Arrays](https://leetcode.com/problems/intersection-of-two-arrays/)

- 这个题思路非常多，一个启示就是，在进行查找的时候，我们可以用bi-search
- 另一个问题是需要解决重复的情况

#### [34.Find First and Last Position of Element in Sorted Array](https://leetcode.com/problems/find-first-and-last-position-of-element-in-sorted-array/)

- 两分查找的衍生版本，但是更深刻
- 关键在于边界位置查询的实现，区分左右试探
- 边界查询的终止条件是找到非法区间，则上一次为边界

#### [35.Search Insert Position](https://leetcode.com/problems/search-insert-position/)

- 两分查找衍生版本，比34简单一点
- 试了几个example发现非法区间low即为最终解

#### [475.Heaters](https://leetcode.com/problems/heaters/)

- 这个题是35的变题，本质也是找插入位置，但问题是能不能把原问题转化为寻找插入位置这个问题
- 使用std::lower_bound来替换自己写寻找插入位置

#### [441.Arranging Coins](https://leetcode.cn/problems/arranging-coins/)

- 这个题，应用两分查找的思路，来进行方程解逼近，比线性快。非常巧妙。有一个注意点，mid可以向下取整，但是求和不行。千万小心
- 朴素枚举也可以解决，最简单的就是一元二次方程求解，别忘了数学

#### [367.Valid Perfect Square](https://leetcode.cn/problems/valid-perfect-square/)
- 并不是直接的两分查找，但是利用了两分查找的思路，快速搜索解空间，试探。本质不是求解，是验证。
- 注意类型转换的地方，需要先转换，再做乘法。否则乘法结束之后，无法转化。

```cpp
double val1 = 1 / 2; // wrong, val = 0
double val2 = static_cast<double>(1) / 2; // right
```

#### [69.Sqrt(x)](https://leetcode.cn/problems/sqrtx/)

- 和上一题思路一致。
- 也可以使用牛顿法。

### 暴力搜索

#### [363.Max Sum of Rectangle No Larger Than K](https://leetcode.com/problems/max-sum-of-rectangle-no-larger-than-k/)

- 承接自303/304
- 基本算法思路其实暴力搜索，枚举所有子矩阵。
- 矩阵和计算的时候，可以利用dp进行预处理。
- 这个题我第一版的思路和最终的答案，我理解其实都是O(N^4)的复杂度，但是正确解在T(n)这个级别要好。
- 53和这个题一个思路，但是类似的优化暴力搜索无法解决。

#### [350. Intersection of Two Arrays II](https://leetcode.com/problems/intersection-of-two-arrays-ii/)

- 这个题很难直接分类，我理解还是比较朴素的遍历了解空间
- 当然，这个提也可以当做数据结构的妙用。这个题依赖hashmap
- 我首选遍历的思路，借助两根指针。需要排序的预处理

## 动态规划

#### [303.Range Sum Query - Immutable](https://leetcode.cn/problems/range-sum-query-immutable/)

- 动态规划第一题
- 基本思路：
    - 状态定义
    - 状态初始化
    - 状态转移方程，计算
    - 状态应用

#### [304.Range Sum Query 2D - Immutable](https://leetcode.com/problems/range-sum-query-2d-immutable/submissions/)

- 303的基础上，进行状态抽象。
- 注意增加了辅助列来避免下表越界的情形。

#### [363.Max Sum of Rectangle No Larger Than K](https://leetcode.com/problems/max-sum-of-rectangle-no-larger-than-k/)

- 参见暴力搜索

## 数据结构

我对于题目的划分，不倾向于通过数据结构划分，因为即使相同的数据结构，也可以承载完全不同的算法，所以主要按照算法进行分类。
但同时我们也注意到，对于一些特定的数据结构，围绕该数据结构的题目有其特殊性，比如链表，树，图。所以，这里也会根据数据结构进行一个大类的划分

### 数组

[307.Range Sum Query - Mutable](https://leetcode.com/problems/range-sum-query-mutable/)
- 这个题和303一脉相承，但是超时，表明每次O(N)的更新时间复杂度无法接受。
- 最后用了一个非常巧妙地思路，作者对于更新采用了O(1)的复杂度，但是sum的时候，将时间复杂度降到了O(sqrt(n))，这也是非常显著的提升。
- 作者思路的来源也在于预处理的巧妙，利用了hash分桶的思路，缓存了每个桶的sum。
    - 更新的时候，肯定只更新某一个桶，显著的降低了更新的复杂度。如果是dp的思路，更新的复杂度在O(N).
    - 查询的时候，中间区间桶的sum可以直接累加，这里的时间复杂度从O(N)降低到了O(sqrt(N))，首位的两个桶需要单独sum

## 数学

### Newton's method

迭代公式如下，本质思路：切线是曲线的线性逼近，参见[如何通俗易懂地讲解牛顿迭代法求开方（数值分析）？](https://www.zhihu.com/question/20690553)

$x_{n+1} = x_{n} - \frac{f(x_{n})}{f^{`}(x_{n})}$

#### [69.Sqrt(x)](https://leetcode.cn/problems/sqrtx/)
- 和上一题思路一致。
- 也可以使用牛顿法。