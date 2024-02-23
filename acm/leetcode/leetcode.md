## 基础算法

### 模拟

这一类题目的思路一般比较朴素，题目中一般会给出，直接用代码模拟实现即可。

#### [67.Add Binary](https://leetcode.com/problems/add-binary/description/)

-  一刷：这个题不好处理的地方在于需要反向变量，处理下标的时候小心点即可

#### [66.Plus One](https://leetcode.com/problems/plus-one/description/)

- 一刷：模拟进位即可。刚开始使用了and_one标记，后来发现不用。

#### [977.Squares of a Sorted Array](https://leetcode.com/problems/squares-of-a-sorted-array/description/)

- 一刷
    - 方法一：这个是我的办法，找到分界点，分别处理。因为单调递增，所有二分很容易找。
    - 方法二：随想录的办法，更简单。不找分界点，前者找是因为插入的时候先插较小的数字，所以需要找。
        - 直接从两头遍历，从后向前插入，即可避免找分界点的问题。

#### [59.Spiral Matrix II](https://leetcode.com/problems/spiral-matrix-ii/)

- 一刷
    - 方法一：模拟 + 状态机
        - 我的思路我觉得还是比较简单的，代码量也少，大思路就是模拟。
        - 首先是确定外部循环
        - 其次，内部是状态机的思路。每个点，其实就是4个方向，方向定了，向前stepping即可。之后，更新状态。
    - 方法二：随想录
        - 这个办法我觉得也没啥问题，但是很明显我的做法思路更简单。
        - 我们的区别主要在于对于边界的处理，我是通过越界判断。随想录是通过确定好每一圈的边界范围。

#### [13.Roman to Integer](https://leetcode.com/problems/roman-to-integer/)

- 基本思路照着实现即可。
- 不过这个题看了题解，有一个好的办法是，进一步泛华了题目给的思路，即发现如果当前位置比后面小，直接减去即可。

#### [12.Integer to Roman](https://leetcode.com/problems/integer-to-roman/)

- 这个题，我觉得做题过程还行，确实是想清楚了再写，保持。
- 我的思路来自于radix的转换，按照这个不断的处理即可。

#### [7.Reverse Integer](https://leetcode.com/problems/reverse-integer/)

- 整型转换题，思路非常朴素。实现即可，但是有很多细节需要处理。
- 方法一：转成字符串处理。最后借助long long判断是否越界即可，但是题目要求不借助long long
- 方法二：就地转换，比string高效一点，但是也要借助long long
    - 这里有一个需要注意的点，就是**负数除法**，这个抽空需要仔细看下
    - 这里的一个技巧是，没有区分正负数，均可以处理，需要明确知道负数除法和正数除法的区别
- 方法三：官方的方法，非常巧妙，充分的考虑了数字的特点
    - INT_MIN(-2147483648)INT_MAX(+2147483647) 
    - 疑问在于这个判断 if (res > INT_MAX/10)，为什么不判断相等的情形。即如果res == INT_MAX/10
    - 那我们进一步看下，如果res == INT_MAX/10，那么此次迭代形成的最终数，假设为214748364i，如果i>7，则res最终越界。
    - 所以，我们讨论以下这个可能的i。此时res是10位数，原数字一定是1xxxxxxxxxxx/2xxxxxxxxxxx，这两种可能。因为原数字也需要保证不越界
    - 此时翻转后i只有两种可能，即1/2。所以，最终形成的res为2147483641/2147483642，由因为后者的原数字越界，不会形成这个数
    - 所以，如果res = INT_MAX/10，那么最终res只有一个可能就是2147483641，这个数不越界。所以，res = INT_MAX/10这种情形也不用判断。
- 最后，这个题还需要学习一个通用的算法就是数字反转的算法

#### [31.Next Permutation](https://leetcode.com/problems/next-permutation/) 

- 这个题，直接看了题解。如下
- step1: 从后向前寻找一个逆序
- step2: 从后向前寻找第一个大于当前数的元素
- step3: swap and reverse the rest elements.
- 如果不存在逆序，直接reverse.

#### [509.Fibonacci Number](https://leetcode.com/problems/fibonacci-number/)

- 这个题dp题，但是由于题目直接给出了公式，我划分为模拟题。按照公式实现即可。

### 二分查找

这里我补充一点方法论，以算竞的知识点为主。我们给出一个序列区间[low, high]，low/high均为数组合法下标的边界
- 方法一：这个也是我刚开始用的办法，即[low, high]作为循环区间
    - mid = low + (high - low)/2; low = mid - 1 or high = mid + 1
    - 如果越界，high < low(low = high + 1)，此时high + 1/low为越界下标
    - 优点：mid计算公式统一
    - 缺点：会漏解，比如mid处的解。
- 方法二：也是算竞的思路，**下标区间(合法区间)[low, high]，二分区间[low, high)**。注意，这里high是合法下标边界。
    - 优点：
        - 二分的终止条件是low == high，该点即为答案所在，始终落在二分区间内。
        - 后面可以看到，如果我们拓宽了非法下标，终止条件也不变。即终止条件始终唯一，不受越界的限制。可以很自然的处理无解的情形
    - 缺点：
        - mid有两种计算公式，主要是避免向左/向右寻找时造成mid和l/r重合
- 实际的写法：采用方法二的拓展版本，即[low, high + 1)，引入一个非法下标。处理无解的情形。同时采用lower_bound作为主搜索算法，同时处理相等和大于的情形。
    - 注意，如果直接使用std::lower_bound需要注意，这个库函数没有引入非法下标，所以要先判断迭代器非法的情形，即找不到的情形，否则直接解引用会出问题。
    - 注意，如果用higher_bound作为主算法，下标区间拓展为[-1, high)
    - 细节
        - **不管找到，找不到，都有low==high**，落在[low, high]这个下标区间内，所以两分的区间是[low, high)
        - 如果以lower_bound作为主搜索算法，需要小心两种找不到的情形
            - target大于序列中所有数字，即找不到第一个比target大的位置，此时落到了边界
            - 存在element > target，但不等于。此时也是找不到，但是找到了插入位置，所以还需要check
```cpp
int lower_bound(const std::vector<int>& nums, int low, int high, int x) {
  while (low < high) {
    int mid = low + (high - low) / 2;  // (low + high) >> 1
    if (nums[mid] >= x) high = mid;
    else low = mid + 1;
  }
  return low;
}

/**
[2,3,5] x = 1
low = 0, high = 3, mid = 1
low = 0, high = 1, mid = 0
low = 0, high = 0,
*/

int higher_bound(const std::vector<int>& nums, int low, int high, int x) {
  while (low < high) {
    int mid = low + (high - low) / 2 + 1;  // (low + high + 1) >> 1
    if (nums[mid] <= x) low = mid;
    else high = mid - 1;
  }
  return low;
}
```

cppreference的解释没有问题，只不过我觉得算竞的解释更容易理解，且更贴合函数的语意
- lower_bound: 在序列中找到ele >= x的数中最小的一个。所以，是下界，lower bound. 向下界寻找。
- upper_bound: 在序列中找到ele <= x的数中最大的一个。所以，是上界，upper bound. 向上界寻找。

#### [704.Binary Search](https://leetcode.com/problems/binary-search/)

- 两分查找的基本实现
- 前提数组有序
- 递归实现，注意两个边界条件。非递归实现也需要掌握
- 二刷：这里采用算竞的模板
    - 核心：主算法采用lower_bound实现
    - 注意：下标区间[low, high]，二分区间[low, high)
    - 注意：无解存在两种可能
        - 其一，搜寻到右边界。it == nums.end(); (右边界纳入了一个非法点)
        - 其二，寻找到插入位置，但是元素不等。*it != target
- 三刷：复习后，采用复习的内容。两种解法，std::lower_bound以及自己实现的lower_bound.
    - 关于上面代码的if条件判断，以```lower_bound```为例子
    - Returns an iterator pointing to the first element in the range [first, last) such that element < value (or comp(element, value)) is false
    - 也就是说在序列中找一个位置，并且是第一个位置，有ele < target，那么只要ele >= target，都要继续寻找。
- 四刷：
    - 对于算竞的理解又进一步，算竞的区间，自始至终都是开区间。只不过就是开区间的数值有区别。以长度为len的数组为例
        - 如果不含非法解，合法区间是[0, len - 1], 那么二分区间是[0, len - 1)
        - 如果包含非法解，合法区间是[0, len], 那么二分区间是[0, len)

#### [349.Intersection of Two Arrays](https://leetcode.com/problems/intersection-of-two-arrays/)

- 这个题思路非常多，一个启示就是，在进行查找的时候，我们可以用bi-search
- 另一个问题是需要解决重复的情况
- 二刷
    - 二刷用了两种办法，我觉得多用几种办法也有好处，就是可以拓宽思路。后者可以在碰到没有思路的问题时，提供帮助。
    - 再着，这个题如果用二分的思路，其实很容易计算时间复杂度。

#### [34.Find First and Last Position of Element in Sorted Array](https://leetcode.com/problems/find-first-and-last-position-of-element-in-sorted-array/)

- 二刷的心得
    - 这个题还是能学到很多技巧，按照grandyang的分类，这个题是两分的第二类题目。
    - 在两分的基础上扩展，寻找左右边界即可。只不过这个代码如何写的优雅，需要考虑。主要要处理，查不到以及查到边界的情形，别搞混了。
    - 朴素的做法，可以查到左边第一个点，然后遍历过去。借助lower_bound实现
    - 注意点
        - 左右两侧的查找，代码如何统一？
        - 插入点的寻找，一般是low <= high条件非法时，此时low(high + 1)是插入点。注意，这个题目的插入点无法区分到底是元素存在于序列的起点，还是元素不存在于序列的起点。
- 三刷心得
    - 这题用了算竞的模板，刚好两个方向都用到了
    - 同时验证了书中的技巧
        - 合法边界low, high，合法区间则是[low, high)，否则过不了。由于我保证了一定能找到，所以没有拓展边界。
        - (low + high + 1) >> 1，我调整为low + (high - low) / 2 + 1; 这个没有问题。
- 四刷心得
    - 用了算竞的思路，非常轻松。
    - 但是stl没有对应的upper_bound，它的那个不是二分的语意。
    - 实现没问题，关键是区间怎么传。
        - 二分的实现，跟要不要处理异常点没有关系，都是开区间
        - 处不处理，就看怎么传递区间了。如果包含异常点，数据合法区间[0, len - 1]
            - lower_bound，传递0, len
            - upper_bound，传递-1, len - 1
        - 最后两种非法情形
            - 一是异常点
            - 二是在区间内，但是没找到。

#### [35.Search Insert Position](https://leetcode.com/problems/search-insert-position/)

- 两分查找衍生版本，比34简单一点
- 试了几个example发现非法区间low即为最终解
- lower_bound方法最简单
- 二刷
    - 直接用了lower_bound

#### [475.Heaters](https://leetcode.com/problems/heaters/)

- 这个题是35的变题，本质也是找插入位置，但问题是能不能把原问题转化为寻找插入位置这个问题
- 使用std::lower_bound来替换自己写寻找插入位置
- 二刷
    - 暴力过了。无论如何，先过了再说。时间复杂度O(M*N)
    - 思路就是
        - 每个位置，可以根据每个heater算一个最小的radius
        - 然后，所有位置当中的最大radius，就是最后的解
    - 优化的思路，在暴力法基础上，优化每个位置最小radius的计算
        - 采用二分来计算，其实就是house在heater中找一个范围low <= house <= high
        - 这个可以用二分来找。
        - 这里还有一个优化是，lower_bound/higher_bound本来都要找
        - 但实际找一个lower_bound就行
            - lower_bound是序列中第一个大于x的数，那么前一个数一定小于x
            - 这个范围就找打了
        - 这个题还有一个注意点，注意处理边界的情况。

#### [441.Arranging Coins](https://leetcode.com/problems/arranging-coins/description/)

- 这个题，应用两分查找的思路，来进行方程解逼近，比线性快。非常巧妙。有一个注意点，mid可以向下取整，但是求和不行。千万小心
- 朴素枚举也可以解决，最简单的就是一元二次方程求解，别忘了数学
- 二刷
    - 一开始没有好思路，直接暴力模拟求解。不管也没有一遍写出来，还是要想清楚再写，白板写代码的时候，还是要仔细斟酌。
- 三刷
    - 这个题确实非常好，我觉得好的地方在于，其对于二分查找的考察，并不教条，同时提升了我对于二分查找的理解。
    - 本质是考察二分查找的思路，但是只是套原始的查找又不行。
        - 向左，向右，两分查找的思路没有变。
        - 变得是向左，还是向右查找的条件。
        - 所以，这么看，**二分可以理解为，二分区间的划分 + 向左向右的条件**。
    - 借助lower_bound/upper_bound的知识，我们很容易知道，这个题是upper_bound
        - 在一个序列中[1,2,3,...m]，找到一个点k，使得partial_sum <= n，这里相当于找的是最大点k
        - 所以，是upper_bound的思路。
    - 同时，这个题又一定有解，所以，参数的传递就是[1,n]，即有效区间，闭区间。不用多传递一个非法点。实现是开区间，不用关心。
    - 这个题还有一个点，upper_bound的实现，区间划分需要+1
        - ```if (nums[mid] <= x) low = mid``` 如果此时low == mid，那么会进入dead loop.
        - 还是满足上面的条件，但是```auto mid = (low + high + 1) >> 1;```这么写没问题
            - 原因在于，前一次计算，mid会落在high上。
            - 此时，条件不满足。low = mid，此时low会落在high上。从而不满足循环条件，跳出。
- 四刷
    - 再说一下，二分区间的划分。
    - 如果这么写```auto mid = (low + high) >> 1;```
        - 向下取整，mid可能落在low上
        - 这样，一旦向右寻找时，```low == mid```，此时并不会发生实际更新。
        - 还是满足向右寻找条件，继续更新区间，还是和之前一样。造成dead loop.
    - 正确的写法
        - ```auto mid = (low + high + 1) >> 1;```
        - 这么写，mid会落在high上
        - 如果向右寻找，```low == mid```，此时会有```low == high```成立
        - 这样，如果还是向右寻找，会造成不满足循环条件，跳出，程序正常结束，没有dead loop.
    - 尝试了数学方法求解
        - (x + 1)*x/2 <= n，n是已知，求x，一元二次方程。 
        - 从函数图像来看，开口向上。最后的函数方程转化为：x^2 + x - 2n <= 0
        - 既然<=0，结合函数开口，取交点左侧的值。所以，x向下取整数解。
```cpp
// 错误写法
low = 1,high = 5,mid = 3,sum=6
low = 1,high = 2,mid = 1,sum=1 // mid = 1，这里落在了low上，向右寻找时。low = mid，无法跳出。
low = 1,high = 2,mid = 1,sum=1
low = 1,high = 2,mid = 1,sum=1
low = 1,high = 2,mid = 1,sum=1
low = 1,high = 2,mid = 1,sum=1
low = 1,high = 2,mid = 1,sum=1
low = 1,high = 2,mid = 1,sum=1

// 正确写法
low = 1,high = 5,mid = 4,sum=10
low = 1,high = 3,mid = 3,sum=6
low = 1,high = 2,mid = 2,sum=3 // mid = 2，向右寻找时，避免mid落在low上。因为向右寻找时，对于low的更新是，low = mid. 所以，要避免mid落在low上，否则会dead loop.
```

复习一元二次方程的根:
$x = \frac{-b \pm \sqrt{b^2 - 4ac}}{2a}$

#### [367.Valid Perfect Square](https://leetcode.cn/problems/valid-perfect-square/)
- 并不是直接的两分查找，但是利用了两分查找的思路，快速搜索解空间，试探。本质不是求解，是验证。
- 注意类型转换的地方，需要先转换，再做乘法。否则乘法结束之后，无法转化。
- 二刷
    - 其实 441 和 367 属于同一类题目，这一类题目直观上理解和二分不一样。
    - 367 的分析思路是把二分区间和二分条件区别开，没有问题。
    - 这个题我又想了一种新的理解方式，还是二分。只不过把原来的序列做个变化
    - 原始序列：[1,2,3,4,5,...k]
    - 368序列：[1,3,6,10,15,...(k+1)k/2]
    - 441序列：[1,4,9,16,25,...k^2]
    - 在变化后的序列中寻找target就行。
    - 还有一点，不管是lower_bound还是upper_bound都是二分查找，不是只朝某一个方向查找。
        - 之前没理解到位，以为只是向一个方向查找。
        - 其实两个方向都找，只不过找寻的目的不一样
            - 一个是序列中大于target最小值
            - 一个是序列中小于target最大值
    - 这个题，我选lower_bound作为主算法。
        - 同时，一定有大于target最小值的解，1 就是。
        - 所以，传入区间不用包含非法点。
    - 但是，ele >= target，不一定等于，所以最后判等即可。

#### [69.Sqrt(x)](https://leetcode.cn/problems/sqrtx/)

- 和上一题思路一致。
- 也可以使用牛顿法。
- 二刷
    - 原始序列：[1,2,3,4,5,...k]
    - 368序列：[1,3,6,10,15,...(k+1)k/2]
    - 441序列：[1,4,9,16,25,...k^2]
    - 69序列：[1,4,9,16,25,...k^2]
    - 就是在69序列中找到 <= target 的最大数，这点注意，我第一次找反了，用了lower_ground
    - 采用upper_ground即可
    - 注意异常点，即x==0的情形。
- 三刷
    - 参见Newton's method

#### [702.Search in a Sorted Array of Unknown Size](https://grandyang.com/leetcode/702/)

- 这个题非会员能做，所以直接放了题解的链接。
- 思路非常巧妙，多了一步，即数组元素不定，从而high不能确定，无法开始二分查找。
    - 题解假设了数组的长度为INT_MAX，从而让二分符合条件。
        - 题目中说了每个元素不大于10000，从而放INT_MAX不会和已有元素冲突。
        - 下标越界，返回2147483647即INT_MAX。证明INT_MAX是非法下标，元素个数肯定不会这么多。
        - 所以，可以设定这个数组有INT_MAX个元素，多余的元素由INT_MAX填充，而不影响查询结果。
    - 注意两分的使用条件，元素可以重复，只不过此时值不唯一。
- 二刷
    - 还是没思路，不知道怎么处理上界的问题。
    - 但是看了之前的题解，觉得分析的非常到位。这些条件不是直接得到的，题目没有直接给。
    - 而是分析已知的条件，从条件中获得的。这点非常好，进了一步。

#### [33.Search in Rotated Sorted Array](https://leetcode.com/problems/search-in-rotated-sorted-array/)

- 这个题和702一样，也是非常好的拓展题，在基础上进行思路的拓展。
- 这个题，不能直接套，要套思路。
- 方法一：先两分找pivot，pivot语义代表左边区间的最后一个元素。然后再分别找，当然这里分治可以优化为减治。减治的边界条件容易找错。
- 方法二：grandyang的思路是这样，这个数组，通过mid进行划分后，势必一半是有序，一半是非有序。对于有序区间，我们直接利用二分。注意，这个思路比较不同，需要好好理解。
    - 其核心思路是，既然整个数组不是有序，我们需要先找到一个有序的区间，进行二分。当然，target不一定在这里面，所以对于有序区间还需要使用target判断一下。
- 二刷
    - 这个题花了我一点时间，先说下我的方法，我先先找pivot，再利用二分。
    - 这么做的原因在于，pivot寻找是一个更一般的办法，即使寻找pivot也可以单出一到题。
    - 好，首先定义，什么是pivot，我的定义如下：
        - the first number in the arr which is smaller than arr[0].
    - 做了挺长时间，在于想套用通用的解法。但，lower_bound/upper_bound的条件，其实很明确。必须有序，这个是确定的条件。
    - 实际做题时，采用了二分的找法。但也是从最朴素的二分思路开始借鉴。
        - 对于两个有序序列拼接成的序列，使用二分。
        - 二分的关键如上总结到，区间的划分+向左向右的条件。
        - 一开始的顺序向右，以次作为主条件。
    - 这个题最后强调下，能用二分，只是借鉴二分的思路，并不是标准的二分查找。因为后者需要数组有序。
- 三刷
    - 用了大部分答主的通用解法
        - 即，从mid划分，两边至少有一个区间是有序的。
        - 任务就是找到这个有序的区间，办法是compare nums[0] and nums[mid]
        - ```if (nums[0] < nums[mid])```left part is sorted. otherwise, the right part.
        - 但是，这个判断有一个问题。就是在edge case，mid的计算会落在low上，因为算术平均，向下取整。
        - [3,1] is an edge case.
            - ```low = 0, high = 1, mid = 0``` at this moment, the left part is sorted, but the judegement gets the wrong conclusion.
        - we update the judgement to: ```if (nums[0] <= nums[mid])```
- 四刷
    - 发现三刷的办法，其实有问题。莫名其妙的居然过了。
    - 三刷的问题是，在区间判断的时候，没有使用low/high，而是直接和 0 以及上界比。
    - 这么做能过的原因，我还没有仔细思考。也可能思路不对，只是碰巧过了test case。我看的题解，没有一个这样做的。
    - 正解是，区间判断用low/high去做。

#### [81.Search in Rotated Sorted Array II](https://leetcode.com/problems/search-in-rotated-sorted-array-ii/)

- 我在找pivot时，用了遍历算法，因为没法二分找会跳过pivot
- grandyang的做法在33的基础上，巧妙的发现，如果nums[mid] < nums[high]时，右边区间有序，这个条件不完备。nums[mid] == nums[high]时，也有可能有序
- 所以，在这个基础上，还是去寻找nums[mid] < nums[high]的情形。
- 二刷
    - 直接用了和33一样的做法，发现有问题。  
        - [1,1,1,1,1,1,1,2,1,1,1,1,1]
        - 上面这个数组，用二分定位pivot是错误的。
        - 因为存在可能重复的元素，导致如果mid在2的右侧，无法向左查询。
        - 如果条件写成```if(nums[mid] > target) low = mid + 1```
            - 又无法向右。
        - 所以，有重复元素，不能借鉴二分的思路。
        - 说明，不管33/81，都无法直接利用二分查找，因为这不符合二分适用的条件。
        - 实际的做法只是借鉴二分查找的思路，寻找pivot。
        - 但是，这个办法在序列中存在重复元素时，会失效。
    - 参考了网上主流的做法，采用了常见的二分查找思路，没有使用liyudong的做法，因为序列不满足有序。
        - 在 33 四刷解法的基础上，处理了```nums[low] == nums[mid]```的情形
        - 这里其实用了2 pointers的思路，自增即可。这也是2 pointers常见的做法。

#### [153.Find Minimum in Rotated Sorted Array](https://leetcode.com/problems/find-minimum-in-rotated-sorted-array/)

- 33题的简化题，找pivot，保持思路一致即可。
- 方法一：我说下我的办法，我还是找了一个target，即nums[0]，用它找到一个两个相邻点，一个大于它，一个小于它，此时insertion point没有问题(不管使用哪种两分查找)。小心high + 1越界的问题
- 方法二：但是，使用grandyang的方法，即不固定target，每次比较nums[mid] and nums[right]，此时不行。只能用开区间的写法，这个我有点不太理解，暂时先记住。
- 二刷
    - 有了之前的基础，这个题很好做。就是找pivot即可。
    - 两个边界条件
        - 数组长度是 1 的情形。
        - 数组本来就是升序的情形。
    - 同时，只有没有重复元素的时候，才可以这么找pivot.
- 三刷
    - 学到了lyd的二分在此处的应用，我之前一直想统一，看样子是统一了。
    - 还是只能借鉴算法思路，不能照搬。
    - 之前就有疑惑，为什么常见的二分可以借鉴，并求解。而lyd的就不行，看样子不是lyd的方法不行，是我没用对。
    - 因为pivot肯定在右边
        - 所以，nums[mid]需要和nums[high]进行比较即可。
        - 同时，不管是普通的二分还是lyd的二分，都有一个通用的理解： **二分mid更新 + 二分条件判断(向左/向右) + 二分边界更新**
        - 这个题，条件的写法简单。但是，二分区间的划分变了，和标准的lyd的写法有区别了.
- 四刷
    - 又做了一遍，也没一遍做对。理解上还是又难度的。
    - 这个题也是lyd的做法，和常规两分，都可以做。但是，都也只能借鉴思路，不能照搬。
    - 没做的原因在于
        - 循环条件怎么些？谁比谁？
        - 向那边找，怎么更新？也就是三刷更新的方法论。
        - 最后是```mid```更新又写错了：```mid = (low + high) >> 1```

#### [154.Find Minimum in Rotated Sorted Array II](https://leetcode.com/problems/find-minimum-in-rotated-sorted-array-ii/)

- 这个题我最终还是做出来了，用了一些trick的办法，但是先ac就行。
- 方法一：我用的是我自己的办法，比较好理解的办法寻找pivot，但是最后边界的处理一直有问题，所以我用了trick的办法来求解。
    - 我的办法之所以没法解决trick的问题，我认为在于闭区间的使用，代码里我显示的处理了low==mid and high==mid的情形，证明闭区间的多一次判断，会造成最后解的位置偏移。
    - 这个题最简单，最快的办法，也是用开区间解题。
- 方法二：grandyang的办法，开区间。不设定target，通过中点和右侧相比来找。中点不能和左侧相比，因为此时不管大小，最小值都有可能在左侧。
- 二刷
    - 在 153lyd的基础上去做。
    - 相等的时候，```if(nums[mid] == nums[high]) --high```
    - 即，对于high相等的时候，移动high。这个也是两根指针的通常做法。相等时移动。
    - 第一次试探没过，因为采用了```low = mid + 1; high = mid - 1;```的方式更新。
    - 注意，lyd的办法，不能这么更新。只能是一个不落在mid上，另一个落在mid上。
    - 上面这种是标准两分的更新办法。但，此时循环条件又不一样。
    - 所以，只能是在lyd的办法上面做调整。

这一套题目，我觉得非常的好，加深了对于二分的认知，因为不直接依赖二分的技巧，而是锻炼了二分的思路，即每次抛弃一半的解。同时，旋转数组这里，二分的技巧不要用闭区间，尤其是针对有重复元素的情形，
只能用开区间。

#### [209.Minimum Size Subarray Sum](https://leetcode.com/problems/minimum-size-subarray-sum/description/)

- 一刷
    - 缪解：我用暴力法求解的，结果超时。
    - 方法一：sliding window. 这个方法我第一次见，掌握的也不太好。本质思路是在一个循环里调整首尾边界
        - 当然，形式上还是两个循环
        - 时间复杂度O(n)，这个考虑的不是很清楚。随想录的解释不够直接。
    - 方法二：参考了[前缀和+二分算法](https://blog.csdn.net/weixin_60466670/article/details/123363464)
        - 这题很核心是要想到前缀和，因为都是正数。前缀和一定是单调递增数列，可以用两分。
        - 本质也是一个两种循环，和暴力法很接近。但是因为使用了二分查找，降低了时间复杂度量级
- 二刷
    - 还是暴力法，虽然过不了。但是，正确的写出来也小于技巧，以及对于边界的控制。
        - 暴力法就简单了，遍历所有的长度，既然是最短的长度，那就从小开始。
        - 这题还有个问题，我之前以为可以先排序，后来读题有误，subarray，不能修改数组顺序。
        - 不是有序数组。
    - 这个题最优雅的解法是双指针，时间复杂度T(N) = 2*N,即O(N)
        - 通过left/right维护一个区间，这个区间的部分和，来跟target进行比较。
        - left/right都向一个方向，即右侧递增
            - right递增，可以增加序列的元素。
            - left递增，可以减少序列的元素。
            - 所以，这里有一个非常重要的前提就是：**元素**是非负的，只有这样，两个指针向右递增时，才能增加或者减少序列部分和。
        - 实际做的时候，错了两个细节
            - left/right都是向右递增，left写成了向左。
            - 区间和的长度不是(r - l + 1)，因为r完成累加后，会先递增，此时是序列最后一个元素的下一个元素，所以不用+1

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

## 基础数据结构

我对于题目的划分，不倾向于通过数据结构划分，因为即使相同的数据结构，也可以承载完全不同的算法，所以主要按照算法进行分类。
但同时我们也注意到，对于一些特定的数据结构，围绕该数据结构的题目有其特殊性，比如链表，树，图。所以，这里也会根据数据结构进行一个大类的划分

### 数组

#### [88.Merge Sorted Array](https://leetcode.com/problems/merge-sorted-array/description/)

- 一刷：一遍过。
    - 思路没什么好说的，归并即可。
    - 不过我做的不好的地方在于，这个题目真正的考点没有发现，即就地怎么解决。
    - 最优解：倒着放。既然nums1空间足够大(这个信息没有get)，不用resize，不会破坏nums1，那么从后向前放即可。非常巧妙。

#### [18.4Sum](https://leetcode.com/problems/4sum/)

- 一刷
    - 方法一：缪解。不到黄河心不死，还是上来dfs，结果最后差10个case
    - 方法二：正解。双指针，暴力法四重循环，这个降到三重。
        - 外部两重循环，固定i和k
        - 内部移动left/right，和3sum一样
        - 但是，这个题数据设计的有技巧
            - target可能是负数，这回导致，从小到达的负数，越加越小。所以，必须判断是正数，才可以剪枝。
            - 数据会越界，转一下int64_t
        - 最后，预处理的排序。刚开始忘了，这个预处理很重要。

#### [15.3Sum](https://leetcode.com/problems/3sum/description/)

- 一刷
    - 方法一：缪解。我采用了dfs的办法，对于相同元素的去重，上来先排序。代码很简单，也没有问题。最后有2个case超时。
    - 方法二：正解。双指针。还是比较巧妙的做法，我没有想出来。随想录的办法
        - 固定i
        - 移动left/right，去凑一个三元组，判断是否为0
        - 同时，i/left/right全部需要去重。

#### [307.Range Sum Query - Mutable](https://leetcode.com/problems/range-sum-query-mutable/)

- 这个题和303一脉相承，但是超时，表明每次O(N)的更新时间复杂度无法接受。
- 最后用了一个非常巧妙地思路，作者对于更新采用了O(1)的复杂度，但是sum的时候，将时间复杂度降到了O(sqrt(n))，这也是非常显著的提升。
- 作者思路的来源也在于预处理的巧妙，利用了hash分桶的思路，缓存了每个桶的sum。
    - 更新的时候，肯定只更新某一个桶，显著的降低了更新的复杂度。如果是dp的思路，更新的复杂度在O(N).
    - 查询的时候，中间区间桶的sum可以直接累加，这里的时间复杂度从O(N)降低到了O(sqrt(N))，首位的两个桶需要单独sum

#### [27.Remove Element](https://leetcode.com/problems/remove-element/description/)

- 一刷
    - 方法一：双指针，一个从前找，一个从后找，发现一个匹配时，交换。
        - 思路不难，但是这个题花了我一点时间，主要是在数组元素的计算上。
            - 从前找，不等target，++ans
            - 循环跳出后，需要判断跳出的条件，即i==k时，该元素是否为target，也有可能++ans
    - 方法二：这个题的考点，我觉得就是数组元素的删除。
        - 链表可以直接删，但是数组删除的时候，需要移动元素。
        - 此时，可以记住需要移动的位置，直接移动到对应的位置即可。
        - 这个办法理解上最容易，也好写。
- 二刷 
    - 暴力法是自然的想法。
    - 直接看到了上次的题解，并不是2 pointers的解法。
        - 而是比较巧妙的记录hole的个数.
        - 每个元素判断。如果是一个hole，累加counter.
        - 否则，向前覆盖即可。
    - 2 pointers的解法也是直观好理解，不过元素的计数还是没有第一次就做出来。
- 三刷
    - 复习了一下二刷的办法。

#### [26. Remove Duplicates from Sorted Array](https://leetcode.com/problems/remove-duplicates-from-sorted-array/description/)

- 一刷
    - 有了 27 题的基础，做起来发现虽然题面不一样，但是做法一样。
    - 增加计数器```counter```用来统计当前元素和前序的相同个数。
    - 即使是不同的元素也没有问题，可以放在一起统计。

#### [283. Move Zeroes](https://leetcode.com/problems/move-zeroes/description/)

- 一刷
    - 这套题目，本质是想讲双指针。但是，我觉得计数法做这类题目最简单。
    - 方法二：用了2 pointers，发现有坑。因为要求1必须在前面。
        - 一开始，我是找第一个0，第一个1，swap.
        - 这么做不行，因为1有可能在0前面。这是一个illegal swap order
        - 拿到illeagal swap order，还不能同时递增下标。0是合法的，只不过1不合法，否则像[1,0,1]这样的序列就不能解决。
        - 所以，此时递增1的下标，即非法的1，在找下一个合法的1。0没有问题。

### 链表

这里整体比较熟悉，所以不特别介绍方法论。

#### [142.Linked List Cycle II](https://leetcode.com/problems/linked-list-cycle-ii/description/)

- 一刷：没思路。
    - 随想录的办法正解，快慢指针，同时论证了环的入口点位置。数学题
        - 相遇，有环
        - 从头开始，从相遇点开始，下次相遇即为入口

#### [19.Remove Nth Node From End of List](https://leetcode.com/problems/remove-nth-node-from-end-of-list/description/)

- 一刷：没有正确的思路。
    - 随想录的办法正解：快慢指针。fast先走n步，slow在跟着一起走，当fast结束时，slow的位置就是倒数第n位

#### [24.Swap Nodes in Pairs](https://leetcode.com/problems/swap-nodes-in-pairs/description/)

- 一刷：比较简单，pre配合cur解决。我看了随想录的办法，交换指针的做法实在是不怎么样。

#### [206.Reverse Linked List](https://leetcode.com/problems/reverse-linked-list/description/)

- 一刷：和203类似，需要pre和cur配合解决。这个题还需要一个next.

#### [203.Remove Linked List Elements](https://leetcode.com/problems/remove-linked-list-elements/description/)

- 一刷：没太多好说的，处理好head的情形即可

#### [707.Design Linked List](https://leetcode.com/problems/design-linked-list/description/)

- 一刷：这个题做的不好，花了一些时间。
    - 思路比较简单，设计也不多说，增加head_/tail_指针。
    - 做的不好的地方在于，对于边界情况考虑不周到，所有操作都要考虑head_/tail_异常时，这两个指针如何更新。
    - 主要就是addAtIndex/deleteAtIndex这两个函数
        - 如果节点只有一个，此时头尾在一起。
        - 删除头时的情形
        - 删除尾时的情形。
        - 由于还有头尾在一起的情形，增加了后两种的处理复杂度。
    - 随想录的办法增加dummyNode这个是好的方式，避免了头尾在一起，二刷的时候可以试一下
    
### Hash

- Hashing is the process of **transforming** any given key or a string of characters into another value.
- A hash function is any function that can be used to map data of arbitrary size to fixed-size values.

最后，我们再来看哈希表：哈希表是根据关键码的哈希值而直接进行访问的数据结构。其中，对关键码的值进行hashing，得到一个hash value。然后根据这个哈希值去完成对应的检索。

#### [219.Contains Duplicate II](https://leetcode.com/problems/contains-duplicate-ii/description/)

- 一刷：没有一遍过，题目没审清楚。就匆匆上手。
    - 这个题hash，hash的本质是为了快速检索。
        - 比如，某一个string，是否反复出现。借助现有的数据结构，比如数组，根据下标可以直接访问。但是，string无法充当下标。
        - 所以，hash(string)->hash value(int)，借助这个hash value在数组中的值，我们可以快速访问
    - 说回来，这个题一样，用value当key，来判断idx是否在范围内。由于idx都是递增，所以只需和上一个idx比较即可。
    - 这种题也是要多做，多做的目标是：保持手感，同时，增进思维的严谨程度。

#### [205.Isomorphic Strings](https://leetcode.com/problems/isomorphic-strings/description/)

- 一刷：没过。题没看懂。
    - 看了题解，才明白啥意思。s->t只能是单射，反之也必须是单射。

#### [454.4Sum II](https://leetcode.com/problems/4sum-ii/description/)

- 一刷：没过。尝试把4重降3重，同时考虑到了重复元素会产生多个解的情形。
    - 但是，正解是2重。
    - 两个两重，计算局部和。同时考虑重复元素的情形。

#### [383.Ransom Note](https://leetcode.com/problems/ransom-note/description/)

- 一刷：简单题，但是也没一把过。
    - 主要是数据结构定义的有问题， char cha_map[26];
    - 这个类型不行，每一个元素是char，0-255。如果都是一样的字母，且长度高于255，这里会溢出。
    - 换成int即可。有时候也不用太省空间，只要题目不是严格要求节省的，没必要。

#### [1.Two Sum](https://leetcode.com/problems/two-sum/description/)

- 一刷：虽然easy题，但是没那么容易，需要考虑的点还有一些。
    - 我的办法是先建立映射关系。然后处理
    - 这种办法的问题是，如果重复元素，不好弄。所以，我的哈希表value放了一个vector用来处理重复元素的下表
    - 这种办法，还要判断vector长度。比如,[3,2], target=6。3 + 3 = 6，但是3只有一个
    - 这题随想录的办法最简单，最优雅，采用了in-place的做法。
        - 3和3自己，这种就不会出问题。
        - 3和另一个3，下表也不会冲突。非常优雅，同时解决了两个不好搞的地方。

#### [202.Happy Number](https://leetcode.com/problems/happy-number/description/)

- 一刷：题目不难，最后代码写的也行。问题是题没看懂，英文的循环终止那句没看懂，后来我又仔细看了下，发现当时脑袋掉线了，in a cycle没看懂
    - 循环如果成环了，就不行，这个是not happy number的判断依据

#### [349.Intersection of Two Arrays](https://leetcode.com/problems/intersection-of-two-arrays/description/)

- 一刷：题目很简单。老实说，做的不满意，因为都不能一遍过。又是漏了题目的条件。
    - 结果是要去重的，这个信息没有get到
    - 看了其他题解，这个题还有两根指针，两分的办法，都非常好。核心还是要看一看怎么去重。
    - 排序预处理是常见的操作

#### [242.Valid Anagram](https://leetcode.com/problems/valid-anagram/description/)

- 一刷：题目很简单，但是审题审的不好。没有一遍过。
    - 都是小写字母，空间可以节省。
    - 每个字母只能用一次。这么重要的题目信息，都没有读到。还是不够耐心。

#### [36.Valid Sudoku](https://leetcode.com/problems/valid-sudoku/)

- 存储对应的数独有效位状态即可。
- grandyang的方法压缩了状态，对于每一个位置，进行编码存储。从而节省了空间开销
    - 编码技巧需要注意，下标是数组，首先下标就需要做区分，不能放到同一个位置，否则row/col无法区分。
    - 其次，字符和下标需要区分，因为都是数字，不区分无法区别。比如把数字9放在第9行和第9列，如果不区分数字和下标，都是99，看不出来。

### 字符串

#### [58.Length of Last Word](https://leetcode.com/problems/length-of-last-word/description/)

- 一刷：这个题考察识别单词的办法，利用stringstream。

#### [459.Repeated Substring Pattern](https://leetcode.com/problems/repeated-substring-pattern/description/)

- 一刷：暴力法。不过这题衍生出一道更有意思的题，即如果找最小的循环子串，怎么找？那遍历的方向就是从1开始反过来。
    - 这个题我看了下随想录的解法，有依赖的kmp的，准备二刷再看。

#### [28. Find the Index of the First Occurrence in a String](https://leetcode.com/problems/find-the-index-of-the-first-occurrence-in-a-string/description/)

- 一刷：暴力法，kmp看了一眼，差不多明白了，回头看看算法的讲解再来二刷。
    - [从头到尾彻底理解KMP](https://blog.csdn.net/v_july_v/article/details/7041827)
    - july讲的不错，但是我用了他的方法，发现有case不过。再研究一下。

#### [剑指 Offer 58 - II. 左旋转字符串](https://leetcode.cn/problems/zuo-xuan-zhuan-zi-fu-chuan-lcof/submissions/)

- 一刷：简单题，也是使用了额外空间。
    - 随想录的办法最简单，就地翻转。需要三次翻转。
    - 这个题应该是固定的思路，对于部分翻转，或者说循环移动，采用这个办法。

#### [剑指 Offer 05. 替换空格](https://leetcode.cn/problems/ti-huan-kong-ge-lcof/)

- 一刷：简单题，按照条件模拟即可。只不过不要用lambda处理。
    - 这个题随想录办法简单。双指针，本质是快慢指针，并且二者最终合二为一。

#### [541.Reverse String II](https://leetcode.com/problems/reverse-string-ii/description/)

- 一刷：花了点时间。刚开始的思路最简单，直接就地，但是另一边的下标我一直算不对。
    - 然后换了思路，使用while循环来驱动。时间复杂度不变，代码更好写。

#### [344.Reverse String](https://leetcode.com/problems/reverse-string/description/)

- 一刷：两根指针翻转，相对简单好写。

#### [151.Reverse Words in a String](https://leetcode.com/problems/reverse-words-in-a-string/description/)

- 一刷：思路比较简单，借助stringstream进行空格划分

#### [125.Valid Palindrome](https://leetcode.com/problems/valid-palindrome/)

- 回文类题目
- 我的解法采用两头同时向中心移动的方法，也有从中心向两端移动的方法，后者需要考虑元素个数的奇偶

#### [9.Palindrome Number](https://leetcode.com/problems/palindrome-number/)

- 回文题
- 思路
    - 方法一：转成字符串，但是这个方法没什么好说的
    - 方法二：就地转换。这个题依赖于对于数字的处理，具备一定的技巧。
        - 负数，直接返回false
        - 正数，技巧在于如何拿到左右两边的数字。基本做法在于拿到基本radix之后，分别做整除和取余，然后拿到left/right
        - 同时，更新radix和value
    - 方法三：基本思路也是转换，但是基于字符串，基本的算法来自于数字翻转，如果翻转后的数字和原数字相等，那么即为回文。
        - 在数字翻转的过程中，有一个需要特别注意的地方就是，要特别小心翻转的时候越界的问题。这个是一个非常典型的思路

#### [5.Longest Palindromic Substring](https://leetcode.com/problems/longest-palindromic-substring/)

- 回文题
- 思路
    - 方法一：暴力法求解。因为它是求最长的，所以我可以从最长的开始枚举，可以降低搜索空间，当然最差的情形显然不能提升。
    - 方法二：最优类问题，首选dp。
        - 回文题目，dp的设计思路都是有固定技巧的，一开始我也没有思路，对于dp，我的分析基础是，寻找原问题与子问题的关系，找到突破口。
        - 但是，这题的技巧在于，原问题和子问题怎么划分。如果是i/i+1这么划分，肯定是不行的。不是简单的线性。
        - 回文dp的设计技巧在于，首先它不是一维，它是二维的。其次，它是由两边向中间进行，空间展开。
        - 具体设计
            - 状态：dp[j][i]表示子串[j,i]的longest palindromic substring
            - 转移方程：dp[j][i] = 
                - if s[j] != s[i], dp[j][i] = 0
                - if s[j] == s[i] and j + 1 == i, dp[j][i] = 2
                - if s[j] == s[i] and j + 1 < i, dp[j][i] = dp[j + 1][i - 1] + 2
            - 边界：dp[i][i] = 1
            - 目标：max(dp[j][i])
        - 细节，为什么要区分j + 1 == 2，因为不区分这个情况，dp[j + 1][i - 1]会越界

#### [516.Longest Palindromic Subsequence](https://leetcode.com/problems/longest-palindromic-subsequence/)

- 回文题
- 思路：搜索就不说了，直接说dp
    - 这个题dp的状态设计和之前回文的做法一致。回文题看起来是个字符串，但是不是一个一维数组能搞定的，也不是严格的线性dp
    - 状态：dp[j][i]表示子串[j, i]的longest palindromic sequence
    - 转移方程：dp[j][i] = 
        - if s[j] != s[i], max(dp[j + 1][i], dp[j][i - 1])
        - if s[j] == s[i] and j + 1 == i, 2
        - if s[j] == s[i] and j + 1 < i, dp[j + 1][i - 1] + 2
    - 边界：dp[i][i] = 1
    - 目标：dp[0][n - 1]
- 这个题和substring的一些差异
    - 首先，就是状态这里，看似都一样。都是子串[j, i]的最优值。但最后的目标却不一样，因为题面也不一样。子串[j,i]的最大子串，不一定是所有串里面具有最大回文能力的串。但是序列就不一样了，序列具备前向兼容性，所以最终的目标不一样。
    - 其次，dp的核心是，通过子问题的求解，推断出更大规模问题的解。这里我在解题时候，忽略了dp的方向，导致了错误的答案。第二维下标是由内向外展开的。

#### [647.Palindromic Substrings](https://leetcode.com/problems/palindromic-substrings/)

- 一刷：这个题我觉得相对第5题，更基础一点。当然这个题也有非常考察思路的点：即dp的状态设计。
- 思路：有了前几道题的基础，这个题问题不大，很明显回文具备原问题和子问题的相似性，从中间扩展的时候，原问题依赖子问题的解。
    - 这个时候，就要考察怎么设计dp具体的值了，这里，不考虑直接求解，虽然这个题求数量，好像用数量设置dp更合理。
    - 但是，实际发现，用设置成数量，不好找原问题和子问题相似关系。
    - 这个题为什么说更基础，就基础在状态设计这里，此处的dp[j][i]就是表达回文串[j, i]是否是回文串，dp[j - 1][i + 1]作为扩大的问题，则很容易利用子问题dp[j][i]进行判断。
    - 状态：dp[j][i]表示回文串[j, i]是否是回文串
    - 转移方程：dp[j][i] = 
        - false, if s[j] != s[i]
        - true, if s[j] == s[i] and j + 1 == i
        - false, if s[j] == s[i] and dp[j + 1][i - 1] == false
    - 边界：dp[i][i] == 1
    - 目标：for (int i = 0; i < n; ++i) { for (int j = i; j < n; ++j) if (dp[i][j]) ++ans; }

#### [131.Palindrome Partitioning](https://leetcode.com/problems/palindrome-partitioning/)
- 这个题用到的点就多了。
- 首先，dfs没说的，其次利用回文dp的思路，优化搜索中对于回文的判断。当然，终极的dp看了一遍，没看懂。我觉得先放过吧。
- 回文的核心题目是647，关键是掌握回文类题目建模的核心思路。
- 二刷
    - 这个题我到是直接做出来了，不过代码写的一般般，很多点都漏掉了，调试了几次才写对。
    - 首先，dfs怎么建模。就算让你枚举，你怎么写？这个题的解题点在于，分割线(终点)是枚举的元素。
    - 其次，递归结束，是分割线到底了串的终点，无法进一步枚举
    - 紧接着，数据结构的设计，chosen是包含子串的集合，不是一个子串。
    - 最后，按照pruning-try-solve recusionly-backtracking的思路，才写对了。
    - dp的思路不太知道，先不急，把搜索扎实刷完，再看dp

### 栈/队列

#### [347.Top K Frequent Elements](https://leetcode.com/problems/top-k-frequent-elements/description/)

- 一刷：一把过。我的思路比较常规，计数-排序-拿topk，借助了priority_queue，不过看以前的办法，直接用pair，定义cmp，对vector排序也很方便。
    - 这个题还有桶排序的思路，因为桶排序每个桶是递增的，所以倒着找到k个即可。
    - 这里的排序，本质是hash的过程。即元素出现次数，当做hash key

#### [239.Sliding Window Maximum](https://leetcode.com/problems/sliding-window-maximum/)

- 一刷：这个题也有点意思，没过。
    - 缪解
        - 说下我的思路，pop的思路很简单。就是判断一下，移除的元素是否最大，如果最大移除最大，同时更新最大。
        - 所以，我的设计是一个三元组。[left, max, second_max]
        - 每次移除left，同时判断是否和max相等，不等自然简单。相等就用second_max更新max
        - 但是，这个思路有个问题，那么你的second_max怎么更新呢？这个是问题，我就卡在这了。
        - 我总不能再存third_max吧。即使存了，如果max/second_max/third_max均通过left移除了，你怎么办
        - 最终这个题没有做出来。
    - 正解
        - 随想录先提了priority_queue，这个的问题是，不能移除期望的元素。我要移除left，但是结构不支持。
        - 核心的办法是：比我多走了一步，只要是单调递减的序列，我全部都存下来。
            - 这个办法的好处是，我存了单调递减序列，我就不怕你移除。反正我有backup元素
            - 注意，一定是单调递减，是因为有可能移除max，如果单调递减，队尾不能操作。如果是队列，队尾入，队头出。
            - 对于pop元素来说，已经不关注left元素，只要不和max相等，我就no-op
            - 对于push元素，保证单调递减的能力。此时，队尾也需要支持pop。所以，需要deque。那么，这个队列单调递增也行。反正你用了deque
    - 简单总结下，正解的办法，比我多考虑一步，即把整个单调递减序列存下来，我只是想了存max/second_max，没有考虑到这一步。
    

#### [841.Keys and Rooms](https://leetcode.com/problems/keys-and-rooms/description/)

- 一刷：花了几次才过，也是挺有意思的一道题。
    - 这个题基本思路是队列的思路，有过层序遍历的经验，写这个相对容易点。
    - 同时，需要辅助hash的思路。因为这里需要快速的知道，某一个key以前是否访问过。hash可以把O(n)的时间复杂度，降低到O(1)
    - 这个题，做的不好的地方在于，没有考虑到重复元素。因为重复元素形成环之后，不断的添加，会导致死循环。需要去重。

#### [739.Daily Temperatures](https://leetcode.com/problems/daily-temperatures/description/)

- 一刷：这个题两次过，第一次忽略了相同元素存下表覆盖的问题，引入queue，问题解决。
    - 这个题我觉得出的挺好的，挺巧妙。
    - 核心思想是利用栈，暂存尚未找到warmer tempertature的元素。所以，这个题也揭示了栈的核心思路，即暂存数据。
    - 需要结合hash一起解决。

#### [150.Evaluate Reverse Polish Notation](https://leetcode.com/problems/evaluate-reverse-polish-notation/description/)

- 一刷：简单题，按照思路做即可。一遍没过，卡在数据类型，还是没考虑到。越界的问题碰到很多次了，都没有考虑到。

#### [1047. Remove All Adjacent Duplicates In String](https://leetcode.com/problems/remove-all-adjacent-duplicates-in-string/description/)

- 一刷：简单题。一遍过。用stack没问题，这个题就是栈的思路，只不过栈最后不好操作。我用了deque，随想录直接用了string，最简单。

#### [20.Valid Parentheses](https://leetcode.com/problems/valid-parentheses/description/)

- 一刷：简单题，但是也没有一把过。没有考虑都是右括号的情形。

#### [225.Implement Stack using Queues](https://leetcode.com/problems/implement-stack-using-queues/description/)

- 一刷：一把过。没什么大问题，一个队列就够了。

#### [232.Implement Queue using Stacks](https://leetcode.com/problems/implement-queue-using-stacks/description/)

- 一刷：一把过，就得达到这个标准。
    - 思路，两个栈模拟即可。

### 树

#### [669.Trim a Binary Search Tree](https://leetcode.com/problems/trim-a-binary-search-tree/description/)

- 一刷：没过。
    - 思路：一开始的思路比较朴素，找到这些不在区间范围的key，借助deleteBSTNode搞定，不过觉得这应该不是正解，没有尝试。
    - 正解：树的构造题目，考虑如何处理根，以及左右子树。
        - 首先，如果某个节点不在范围内，不能直接删除。因为它的左枝/右枝可能在范围内，所以，此时删除该节点没有问题，但同时需要更新该节点。
        - 其次，对于在区间范围的节点，没问题，递归处理左右子树即可。

#### [538.Convert BST to Greater Tree](https://leetcode.com/problems/convert-bst-to-greater-tree/description/)

- 一刷：一遍过。
    - 思路：bst中序有序，观察题目，右中左累加即可。
        - 关于接口设计，构造新树，递归处理左右子树。
        - 引入sum参数，作为累加结果。
    - 随想录的思路：思路一样，但是代码更简洁
        - 不构造新树，原树上修改
        - 引入pre，这个也是bst的常用操作，pre看似只是前一个节点，但是它是累加值，可以达到目的。

#### [108.Convert Sorted Array to Binary Search Tree](https://leetcode.com/problems/convert-sorted-array-to-binary-search-tree/description/)

- 一刷：没思路，主要是不知道如何处理平衡。看了题解
    - 正解：随想录的讲解非常好，这题本质是寻找合理的分割点，然后递归处理分割后的子区间即可。
    - 其实，这个题属于构造二叉树的题目，这一类题目都是要寻找一个合理的划分点，然后递归处理。
    - 这个题没做出来，还有一点做的不好的是，没有很好的观察题目，比如树的根节点和区间的关系。
    - 划分点来自于二分的思路。取闭区间即可。

#### [450.Delete Node in a BST](https://leetcode.com/problems/delete-node-in-a-bst/description/)

- 一刷：这个题真是折磨人啊，没过。
    - 缪解：其实也算不上缪解，这个题我的思路没问题，问题一直出在接口设计上。
        - 我是采用引用的做法，结果引发了血案，指针和引用配合使用，太容易出错，查这个问题，查了一天。
        - 回过头，我还是尝试用引用的方式做，但是这么多指针和引用配合，看的头晕了，决定不采用这个方法。回到指针本身来做。
        - 借鉴701的接口设计，但是第一遍一直没有理解删除为什么能这么做。因为插入，肯定会插入新节点，返回ok。那么删除呢？返回谁呢？
        - 一直没有搞明白，在不使用引用的前提下，如何做接口设计。
        - 这个题我尽力了，其实引用有希望，但是实在不想再搞了，因为我觉得这种方式即使过了也意义不大，因为这是最繁琐的方式，且没有学到什么。对于指针的方式，一直又没搞明白接口设计。
- 二刷：一遍过。这次系统学习了随想录的办法
    - 首先是接口设计，不管是插入，还是删除，都可以通过返回值来做。
        - 插入好理解，返回插入的节点就行。但其实不然，因为插入的题目，本质返回的也是root。如果你插在非root节点，更新有什么意义？
            - 要这么理解，如果不插入在root节点，不用更新root，但是root的左子树，有子树，是不是有可能更新。
            - 顺着这个思路去理解，每次返回更新的节点，上层更新left/right
        - 此时，删除就是一样的道理。删除节点，不一定删除root，不用更新root，但是你左子树，右子树，是不是有更新的可能
        - 插入，每次返回新插入的节点；删除，返回谁呢？
            - 返回本次的要删除位置被替换后的位置，比如root->left充当了新root，那么返回这个节点即可，简单说，新root
    - 其次，删除逻辑，这个我梳理的比较好，没什么问题。

#### [701.Insert into a Binary Search Tree](https://leetcode.com/problems/lowest-common-ancestor-of-a-binary-search-tree/description/)

- 一刷：没一遍过，调整了一下才过。
    - 思路：找到插入节点即可，基于preOrder遍历。处理上有一些细节
        - 需要保存prev，因为插入除了开辟节点，还需要维护bst关系，需要prev
        - 同时，需要保存方向。
        - 没一遍过是因为，pass by value，如果更新root，失效，pass by reference
        - 这个题其实可以更难，即avl你怎么插入？
    - 看了随想录的办法，两种解法， 第二种和我的一致。说下第一种。
        - 第一种办法比较简单，我觉得简单的地方，在于不用保留parent以及direcion.
        - 思路主要是，需要返回插入节点。

#### [235.Lowest Common Ancestor of a Binary Search Tree](https://leetcode.com/problems/lowest-common-ancestor-of-a-binary-search-tree/description/)

- 一刷：没思路。
    - 真惭愧，刚做完236，这个题依然没思路。当然236的办法肯定适用。本质还是不知道怎么利用bst的特性。
    - 随想录的讲解依然非常清晰。
        - 首先，我们利用bst的特性，分析题目，即对于lca root，一定有p->val < lca root->val < q->val
            - 这里，随想录还强调了一个细节： q->val < lca root->val < p->val
            - p和q的大小不确定谁大，谁小， 这个细节需要考虑。
        - 其次，考虑是否为充要条件。如果root满足:p->val < root->val < q->val or q->val < lca root->val < p->val,它就一定是lca root嘛？
            - 不一定，比如讲解中的例子，3/8都满足在区间[1,9]内，但不是lca root
            - 通过归纳总结(这个其实是精髓，需要证明)发现：从top-down递归时，遇到的第一个满足上述条件的节点，即为lca root
        - 紧接着，递归写法：接口设计/返回条件/单层逻辑
            - 这里的写法其实有些技巧，上面思路已经说清楚了，就是top-down manner第一个满足条件的节点。
            - 但是实际编写代码时，不能正面求解。因为p/q不能区分大小，所以反向判断，不满足遍历左子树/右子树
        - 最后，这个题减治法，根拿到解，根返回。左子树拿到解，左子树返回，右子树拿到解，右子树返回。

#### [236.Lowest Common Ancestor of a Binary Tree](https://leetcode.com/problems/lowest-common-ancestor-of-a-binary-tree/description/)

- 一刷：没思路。
    - 树的题目，我就两个思路，其一是利用递归结构分析出递归算法，其二是基于遍历算法求解。看到题目后，没有反应出应该用哪一个思路求解。
    - 看了随想录的解法，讲的非常好
        - 首先，需要bottom up manner traversal，怎么办？发现postOrder是bottom up manner，所以利用postOrder
        - 其次，其次讨论了最近公共祖先的两种情况，并说明第一种情况包含第二种，借此，统一了做法。
        - 紧接着，确定了递归的写法，接口/边界/单层逻辑
            - 这个题最终的写法，看起来并不像postOrder，因为如果要看访问root和访问left/right的时机，看起来像preOrder
            - 但本题先访问root是因为那是边界条件。所以，这里的判断依据是需要在单层逻辑中查看。
            - 同时，单层逻辑中是先访问left/right，再访问root。但话说回来，即使不访问root也没有关系。
            - 这里很明显的就是bottom up manner，到不用纠结是不是postOrder
        - 最后，强调了这个题为什么要遍历整个树。
            - 因为本题虽然有返回值，但是拿了返回值不能返回，需要做进一步判断。
            - 理论上，我们找到了lca就可以返回，但是对于单返回值的函数，目前不支持这种做法
                - 回想avl的题目，返回值即表明深度，也表明是否balanced，有多重语义，通过int可以整合，放在一起ok
                - 但是，本题的返回值root，是找到等于p/q的节点，返回了节点的地址，没有更多的语义能表明返回值是否已经是成功的lca
                    - 如果改了返回值接口，那么自然是没问题。
                    - 但是，如果用现有的接口，不行。还是需要遍历整棵树。
                    - 所以，实践不能做，只是这种接口设计下的实践不能做。
        - 总结：回溯从底向上搜索，遇到一个节点的左子树里有p，右子树里有q，那么当前节点就是最近公共祖先

#### [501.Find Mode in Binary Search Tree](https://leetcode.com/problems/find-mode-in-binary-search-tree/description/)

- 一刷：一遍过。
    - 但是我的思路不好，没有利用bst的特性。我用了最朴素的办法，遍历记录counter和max。最后遍历counter。
    - 看了随想录的办法，非常好，利用到了bst的特性，bst在中序遍历中有序，这个性质太重要了。
        - 对于一个有序向量，容易统计众数。
- 二刷：一遍没过，调整了几次才过。
    - 用了随想录的思路：中序遍历bst有序，同时维护最大连续长度，为绕后者进行更新。
    - bst惯用的idiom：保存prev指针，借助这个我们才可以判断最大连续长度。
    - 没有一遍过是因为对于逻辑处理的细节不好，prev/continuous_cnt是每次更新的，其余的需要判断更新。

#### [530.Minimum Absolute Difference in BST](https://leetcode.com/problems/minimum-absolute-difference-in-bst/description/)

- 一刷：一遍过。
    - 这个题目的思路来自于98递归的解法，看样子这个技巧比较常见，即保存路径中prev节点。

#### [98.Validate Binary Search Tree](https://leetcode.com/problems/validate-binary-search-tree/description/)

- 一刷：一遍没过，调整了思路才过。
    - 缪解：这个题我拿到，发现和avl那个题很像，需要判断左右子树，同时左右子树的节点和root需要判断。
        - 所以，很自然的按照之前avl的递归写法，用preOrder做了。
        - 结果发现，这个题有一个没考虑到的点是：只判断左右子树根节点和当前根节点的关系是不够的。因为右子树最左下节点可能不满足。
        - 发现这个问题，我意识到preOrder肯定是不行的。
    - 正解：调整思路。对于bst，中序一定单调递增。中序遍历，拿到序列，最后判断单调递增即可。
    - 随想录的办法：强调了bst就是借助中序做。看了递归的思路，存储了prev非常巧妙，基于inOrder的递归求解，二刷试试。
- 二刷：中序递归，一遍过。整体思路还是挺清晰的。引入prev非常巧妙

#### [700.Search in a Binary Search Tree](https://leetcode.com/problems/search-in-a-binary-search-tree/description/)

- 一刷：一遍过。
    - bst的思路很简单，当然，整个代码的写法基于遍历的写法。

#### [617.Merge Two Binary Trees](https://leetcode.com/problems/merge-two-binary-trees/description/)

- 一刷：没有一遍过，调整了几次才过。
    - 整个也是递归的思路，其实遍历一棵树和两棵树没有区别。采用PreOrder
    - 没处理好的地方在于，递归实参没有判断非空的情形，导致run time coredump.
    - 其实，上面这个问题的本质是，如果两颗树的root不都有效，怎么继续操作。还是递归处理，但是要给nullptr

#### [654.Maximum Binary Tree](https://leetcode.com/problems/maximum-binary-tree/description/)

- 一刷：同106。递归的结构，用递归的算法处理。
    - 找root
    - 划分区间
    - 递归处理左右子区间，即生成左右子树

#### [105.Construct Binary Tree from Preorder and Inorder Traversal](https://leetcode.com/problems/construct-binary-tree-from-preorder-and-inorder-traversal/)

- 一刷：同106，小心边界的划分。主要是麻烦在vector根据区间构造的参数这里，第二个参数是长度。

#### [106. Construct Binary Tree from Inorder and Postorder Traversal](https://leetcode.com/problems/construct-binary-tree-from-inorder-and-postorder-traversal/)

- 一刷：直接看了随想录的题解。
    - 说下思路，树的问题，我目前总结大概就两种思路
        - 思路1：递归的结构，用递归的算法求解。这里本质是要理解题目和递归的结构的关系。
        - 思路2：基于遍历算法求解。
    - 说回本题，本题的理论求解不难，关键是代码怎么写？
        - 对于递归问题/递推问题，直接想不好想明白了。其实写几个递归/递推的例子，就明白了。
        - 这个题一样，直接写代码不好写。但是，根据题目的case，尝试写几个从0-1，从1-2，从2-3的例子，就很清楚了。
        - 使用递归求解。通过递归处理前序/中序区间，来进行树的构造。
            - 题目的核心点在于，通过后根序列，找到root。
            - 然后划分序列
            - 最后归处理划分后的前序/中序区间，即处理左右子树。

#### [113. Path Sum II](https://leetcode.com/problems/path-sum-ii/description/)

- 一刷：preOrder+backtracking，单路型枚举。
    - 这个题要求是到叶子的路径，第一遍没看清，以为有路径就ok

#### [112.Path Sum](https://leetcode.com/problems/path-sum/description/)

- 一刷：这个题在513的基础上，悟到了preOrder+backtracking，以及与dfs模板的异同，直接用preOrder+backtracking，非常快。
- 二刷：这个题在一刷的基础上进行改进，改进的地方类似于dfs回溯中，找到一个解后，如何直接返回？一刷的办法在找到解后，还是会继续遍历。由于不影响bool返回值，对结果没有影响，只是影响性能。
    - 对preOrder增加返回值，如果返回true，直接返回。否则，继续preOrder试探。
    - 随想录的办法我看了，它是dfs的模板，指数型枚举。

#### [513.Find Bottom Left Tree Value](https://leetcode.com/problems/find-bottom-left-tree-value/description/)

- 一刷：层序非常简单。一遍过。
- 二刷：这题看了随想录的办法，提供了递归的做法，我觉得也很好，所以二刷。
    - 对于题意的理解，不是一定是左孩子。而是，最下面一层的第一个节点，可能是左孩子，也可能是右孩子。
    - 所以，不能直接preOrder，因为此时找到了最左孩子，但不一定是最下层。
    - 随想录的核心思想是：先找到最下层，再找本层的第一个节点
        - 遍历即可，同时记录最大高度，最大高度更新时，更新此时的最大值即可。
        - 还有一个问题，怎么保证left tree？用preOrder保证，该种遍历保证可以先访问左节点。
        - 即我们在不断向下试探的过程中，先找左孩子。从而，既能保证找到最下层，也能保证找到改层第一个节点。
        - 这个题由于记录状态，需要回溯状态变量。在preOrder退出时，回溯即可。
    - 写法上，我完全基于preOrder，理解起来比随想录的办法简单。
    - 对比一下preOrder和dfs的区别
        - 从思路上来说，都是dfs，找到一个节点就一直向下试探。
        - 写法上有区别
            - dfs的写法，按照随想录的办法，level/start，每一层试探start所有可能值，然后向下走，回溯。
            - 但是，preOrder的写法是，试探就一个值，就是root。即start就一个，但是dfs的时候，有两条路。这就没对上。
            - 我在做257时，每一层的start由root->left,root->right充当，迈步子-试探-回溯。很好写。
            - 那么经过513的实践，我发现，这里的dfs写法是，迈步子-两路试探-回溯。基本架构还是对齐的，只不过试探的时候不太一样，但是二叉树的题目又很固定，也没问题。同时，preOrder的写法，也不算指数型枚举，是单路型枚举。

#### [404.Sum of Left Leaves](https://leetcode.com/problems/sum-of-left-leaves/description/)

- 一刷：一遍过。
    - 整体思路比较简单，基于遍历来做就行。我基于preOrder做，对于左右叶子，自己不能知道，需要父节点传进来即可。
    - 随想录的办法简单看了下，有点麻烦。不如我的方式简单直接。不过，提到了非递归的栈方法，其实还是模拟递归遍历。

#### [257.Binary Tree Paths](https://leetcode.com/problems/binary-tree-paths/description/)

- 一刷：dfs过。
    - 因为有dfs的基础，所以这个题理解上并不难，就是dfs即可。
    - 但是dfs实现的时候，一开始没写对。dfs的模板pruning-try-solve recursionly-backtracking，一开始没有适配
    - 不能适配的原因在于，到底处理当前节点，还是处理left/right child。
    - 最终决定，处理child。因为child是每层的试探，所以处理了child。
    - 每一层child就两种可能，所以这题也是指数型枚举。
    - 后来觉得，不能处理当前节点的原因在于，处理当前节点没有用，因为需要判断叶子。当然，随想录应该是这么做的。不过这个题，我的原则是，找一种最容易理解的思路过了即可。
- 二刷：preOrder过
    - 这个题我受到了513的启发。昨天一刷的时候，dfs的思路很明显。
    - 今天再做的时候，完全受遍历思路的影响，尝试用遍历的思想解决。不管哪种遍历都行，选择preOrder
    - 问题就是，昨天用dfs，backtracking代码好写，但是preOrder怎么写？也非常简单，稍微改动下遍历代码，在最后backtracking即可。很简单，但昨天一直没想到。

#### [110.Balanced Binary Tree](https://leetcode.com/problems/balanced-binary-tree/description/)

- 一刷：没过。
    - 说下思路：对于树的题目，递归的结构，自然有递归的算法。我尝试规约问题，看能不能把问题，递归的划分为子问题。
        - 求左右子树的高度，然后相减判断。根节点是否平衡。
        - 但问题是，左右子树本来可能不平衡。
        - 即，平衡二叉树，要求每一个节点为根的子树，都是平衡的。所以，直接规约失败。
        - 其次，我考虑遍历算法解决，那我想到的是遍历每一个节点，然后计算该节点左右子树的高度，计算该节点的左右树高差异即可。
            - 其实这个办法是正解，但是我没有坚持，时间复杂度比O(N)高一个量级。所以，放弃了。
            - 我看了之前的解法，也确实这么做的。
    - 正解：其实规约问题的思路没有变。只不过返回值设计的巧妙。
        - 对于根节点，计算左右子树高度，计算该节点的avl值。但是没法保证左右子树是否平衡。
        - 所以，左右子树计算高度时，判断他们是否平衡。
        - 对于每个节点，递归函数如下写法
            - 计算高度，本质对于avl的计算依赖高度。
            - 如果avl不平衡，返回-1。否则，返回高度。给高层节点计算avl使用。
            - getHeight这个函数的语义是这样：root为根的二叉树，如果平衡，返回高度。否则，返回-1.
        - 随想录强调了深度和高度的区别。
            - 深度：top-down manner，决定了可以层序。
            - 高度：bottom-up manner，决定了只能后序。

#### [222.Count Complete Tree Nodes](https://leetcode.com/problems/count-complete-tree-nodes/description/)

- 一刷：层序。
    - 不过没有基于完全二叉树的性质做题，随想录给了类似的解法。

#### [226.Invert Binary Tree](https://leetcode.com/problems/invert-binary-tree/description/)

- 一刷：一遍过。基于递归的方式，直接找到了递归解。
    - 看了下回想录的解析，发现自己真是稀里糊涂的过了。
    - 随想录的思路是：遍历每一个节点，把每一个节点的左右孩子交换即可。所以，基于某一种遍历算法即可求解。
    - 根据回想录的算法，我看了自己的实现，其实就是基于后序遍历的做法。只不过visit函数是交换swap(root->left, root->right);

#### [101.Symmetric Tree](https://leetcode.com/problems/symmetric-tree/description/)

- 一刷：没过。没想到办法。
    - 树的题目，我能想到的解法大概有两种，第一种基于树的递归结构，给出递归解法。第二种，基于遍历的求解。
    - 这个题，我尝试直接给出递归解法，发现求解不了。当然，后面我发现，此时我对递归解法的认知还停留在depth of tree这种题目，即参数一定是root，就一颗树。
    - 正解：左子树，前后中遍历。右子树，右左中遍历。即拆成两颗树，分别用两种遍历方式。虽然代码还是递归的，但是不好想。
    - 迭代法：还是基于正解给出的思路，outside/inside遍历。那么，层序遍历非常好做这个题目。所以，非递归题目，层序是一个可以尝试的办法。当然，利用栈模拟也是主要的思路。
- 二刷：尝试了非递归求解，层序。
    - 非常直接。当然我也没做出来，问题出在还是忘了要当做两个子树来做。
    - 所以，虽然还是层序，但是写法迥异。每次需要入队两个节点，出队两个节点进行比较。
    - 出入的顺序，本质是遍历两棵子树的顺序。

#### [111.Minimum Depth of Binary Tree](https://leetcode.com/problems/minimum-depth-of-binary-tree/description/)

- 一刷：没过。一时没想明白，直接看了过去的办法。
    - 这个题求的高度，是最短路径所代表的树的高度。如果一棵树，只有右子树，显然不能用左右最小+1的方式。因为此时，只有右子树这一条路径，求max可以，但是求min不行。
    - 正解是分别判断。只有当左右子树均存在，才可以std::min求一个最小+1
    - 当然，这个题也悟到了，层序其实提供了非递归的解法。
    - 随想录的办法也学习了：即找最低点。在层序遍历时，如果一个节点，左右孩子都没有，那么找到了最低点。此时的深度即为最短路所代表的深度。
- 二刷：层序遍历。最低点即为最小深度。最低点的判断：叶子即为最低点。

#### [104.Maximum Depth of Binary Tree](https://leetcode.com/problems/maximum-depth-of-binary-tree/description/)

- 一刷：本质就是求树的高度。
    - 树是具备递归结构的，所以首选办法递归解决。非常简单，考虑好递归的边界即可。
    - 随想录还是采用了层序的办法，也行。
- 二刷：用了非递归的办法，即层序遍历。
    - 这个题求最大深度，其实没有多余要考虑的，最大深度就是树的深度。

#### [117.Populating Next Right Pointers in Each Node II](https://leetcode.com/problems/populating-next-right-pointers-in-each-node-ii/description/)

- 一刷：层序。解法同116，看样子我采用了非常通用的办法。

#### [116.Populating Next Right Pointers in Each Node](https://leetcode.com/problems/populating-next-right-pointers-in-each-node/description/)

- 一刷：层序。

#### [515.Find Largest Value in Each Tree Row](https://leetcode.com/problems/find-largest-value-in-each-tree-row/description/)

- 一刷：层序。

#### [429.N-ary Tree Level Order Traversal](https://leetcode.com/problems/n-ary-tree-level-order-traversal/description/)

- 一刷：层序。

#### [637.Average of Levels in Binary Tree](https://leetcode.com/problems/average-of-levels-in-binary-tree/description/)

- 一刷：层序。

#### [199.Binary Tree Right Side View](https://leetcode.com/problems/binary-tree-right-side-view/description/)

- 一刷
    - 缪解：我开始尝试preOrder解决，以为只是把右孩子打出来，那么久正常遍历，不操作左子树。后来发现，这个题真正问的是，把每一层最后一个节点打出来。
    - 正解：层序，打出每一层最后的节点。这个题我记录了last去处理。当然随想录的办法也简单，主要是通用
        - 没有一遍过。
        - 层序遍历的基本算法掌握好，要会灵活调整。比如，这个题先入队左右孩子，才能尝试更新last
        - 别老忘了**队头出队 + 有效指针才入队**

#### [107.Binary Tree Level Order Traversal II](https://leetcode.com/problems/binary-tree-level-order-traversal-ii/description/)

- 一刷：和102思路一样，头插法即可。

#### [102.Binary Tree Level Order Traversal](https://leetcode.com/problems/binary-tree-level-order-traversal/description/)

- 一刷：没过。我对层序并不陌生，问题是，每一层的节点怎么获取？
    - 随想录给出了解法，while刚开始时，队列中待出队的元素个数，即为当前层的节点个数。
    - 这个办法怎么想到的呢？其实，我觉得是通过递推，找规律发现的。这里应该试一些case，尝试从一般的例子出发，看能不能找到一般性的结论，大致是这样的方法。
    - 这个题还有个问题，指针这里，不要用const auto&这样的方式获取top元素。直接auto获取，没有性能开销的。
    - coredump的原因在于，拿了引用，相当于拿到了top的地址。但是，pop结束之后，top就失效了，可以随意操作。所以不能拿引用
    - 只能是auto拿一个值，这个变量的值放了node地址。
    - 这个题我看了我以前的做法，会记录一个last节点，也行。

#### [94.Binary Tree Inorder Traversal](https://leetcode.com/problems/binary-tree-inorder-traversal/description/)

- 一刷：递归法。没什么好说的，注意递归的边界。
- 二刷：非递归。
    - 模拟递归的思路，不断先访问节点，然后向左走。再向右即可。

#### [144.Binary Tree Preorder Traversal](https://leetcode.com/problems/binary-tree-preorder-traversal/description/)

- 一刷：递归法。
- 二刷：非递归。
    - 模拟递归的思路。不断向左走。出栈时访问。然后向右走。

#### [145.Binary Tree Postorder Traversal](https://leetcode.com/problems/binary-tree-postorder-traversal/description/)

- 一刷：递归法。
- 二刷：非递归。
    - 模拟递归的思路。不断向左走。然后出栈，向右走，再入栈。最后第二次出栈，访问节点。
    - 所以这里，每个节点出栈两次，第一次出栈是为了拿到节点的右指针，由于还不能访问，所以继续入栈。
    - 等到第二次出栈时，即可访问。
    - 这里需要增加一个辅助数据结构，表明节点到底第几次出栈。由于有了出栈次数表示，也不用真的出栈。
    - 代码实现的注意点
        - 先判断第二次出栈的情形。并且是while判断，只要有第二次出栈的，一定要先全部真的出栈。
        - 这里的判断条件是and
        - 这里没有使用随想录的办法，我的办法整体形式统一，好理解。好理解的原因在于，就是模拟递归。

## 搜索

搜索这里的知识点我说一下，没有按照随想录的方式来汇总，是因为后者的范畴较小，而是按照算竞的思路来汇总。

- 搜索本质上是穷举法，对问题进行求解时，需要在问题对应的状态空间进行映射与遍历。
    - 我们讨论各种搜索算法，其实是在讨论采用何种策略对状态空间进行遍历。
    - 对于随想录而言，只讨论dfs这一种搜索策略，同时给出该种实现的模板。
    - 当然，还有bfs以及其他启发式搜索策略，不在随想录的讨论范围内。
- 何谓回溯？
    - 随想录的讨论以回溯展开，回溯和我们上面说的搜索算法有什么区别？是不是一回事？
    - Backtracking is a class of algorithms for finding solutions to some computational problems, notably constraint satisfaction problems, that incrementally builds candidates to the solutions, and abandons a candidate ("backtracks") as soon as it determines that the candidate cannot possibly be completed to a valid solution
    - 上面是wiki的解释，从这个角度来说，是搜索无疑，逐步试探，构造最终的解。
    - 为什么叫回溯？就是在搜索试探过程中，如果发现某一步的试探，已经不可能构成最终的解，那么会放弃在当前分支的试探，回到搜索树的上一层，尝试新的试探。这个放弃-回到上一层-新试探的方式，是回溯的核心。本质是剪枝，优化搜索效率。
- 递归又是什么？搜索，回溯，递归这三个术语放到一起的时候，究竟表达什么？
    - 递归/递推是程序设计技巧，和回溯，搜索这些算法思想没有直接关系。
    - 递归多和回溯，搜索有联系的原因在于，它求解问题的方式符合后者遍历解空间的方式。
    - 递推 vs 递归
        - 以己知的"问题边界"为起点向"原问题"正向推导的扩展方式就是递推，bottom-up manner.
        - 然而 在很多时候，推导的路线难以确定。
        - 这时以"原问题"为起点尝试寻找把状态空间缩小到己知的"问题边界"的路线，再通过该路线反向回溯的遍历方式就是递归，top-down manner
        - fib数列就是最好例子来理解这两种方式的差异

### DFS

这里先讲acwing的三道题，这个非常基础，并且这三道题我反复琢磨，对于理解dfs的一般做法大有裨益。

#### [92.递归实现指数型枚举](https://www.acwing.com/problem/content/94/)

- 二刷：说一下这个题，本质还是想统一随想录的方法和算竞的方法。
    - 这个题按照上面的说法，也是能统一，算竞的每一层[start, end]只有不选或者选当前数，这两种可能。
    - 算竞给出[start, end]这个区间，本质也是要枚举当前层所有可能的选择。
    - 但是，这个题不会像上个题一样，有两种写法。上个题本质上可以用指数型枚举/组合型枚举求解，但是这个题只有指数型求解这一个思路。
    - 可以考虑n = 3的情形，就会发现这个题用组合型枚举在思路上的根本错误。所以，写法只有一种。

- 三刷：主要是在二刷的基础上，再此思考枚举型和组合型的区别
  - 枚举型，每一个元素选或者不选，它是以元素为视角
  - 组合型，并不是以元素为视角，而是以chosen的一个试探槽位为视角，不同的元素可以放到这个槽位，所以是组合型。、
    - 对于这个槽位，没办法不放元素。肯定要放一个。所以，这个题组合法做不了
- 四刷：本以为确实搞明白了，看样子又是搞混了，我的问题主要还是出在统一算竞和随想录的方法上。
    - 首先，新一轮复习，我做dfs的思路主要是按照随想录的办法，核心就是关注树的层次和宽度。
    - 其次，我下意识的使用了这个办法来做题。我们可以看下面的代码
    - 紧接着说以下思路：   
        - 这个题初看，组合题，因为在乎顺序，所以考虑采用组合题的模板。不放，可以理解为放一个空元素，所以还是组合题。
        - 当然，或者可以cn1 + cn2 + ... cnn，这样也可以，问题就是这种选多少个不定的题目，边界没法确定
        - 但是，存在不放的情形，树的深度不会向下。那么通过chosen来获取level就不可以，可以显示指定level
        - 同时，因为不考虑顺序，每一层的宽度[start,stop]，start需要从上一层试探的元素下一个开始
        - 最后，发现start和level同质，保留一个。
    - 这里说下问题：
        - 对于组合题，核心思路是以元素为视角，试探每一个槽位。
        - 那么问题来了，每一个元素试探某一个位置的的时候，都有不放的情形，这样显然会造成重复。所以，下面的代码有问题。
    - 正解
        - 就是指数型的思路，以每个元素作为搜索树的一层，每一层有两个选择，放或者不放。
        - 从搜索树的角度，一下就清楚了。
            - 指数型，每一层的宽度是2，对于某个元素，就是放或者不放。
            - 组合型，每一层的宽度是[start,stop]，通常start = i + 1;
            - 指数型，每一层的宽度是[1,n]，所以，需要引入vis数组标记之前的元素是否放入
            - 组合型和指数型宽度的差异在于他们是否去重。
- 五刷：看了四刷的讲解，已经非常清楚了。我再补充一下
    - 对于指数型：完全以元素为视角，对于每一个元素，就是放或者不放。即存在不放的可能，这个显著区别于其他题目。
    - 对于非指数型，其实是以槽位为视角。对于每一个槽位，用所有可能的当前元素，进行试探。
        - 从这个角度理解，wrong answer除了之前说的，每一个元素试探，都有不放的情形，导致重复。
        - 还有另一个问题是，既然以槽位为视角。每次试探的是可行解，可行解位于[start, stop]，不试探不是一个可行解，所以这么做也不行。
        - 所以，对于不放的情形，非指数型没法做，是因为可行解理解，不能表达没法放的情形，因为无论如何需要在槽位进行试探。

```cpp
#include <iostream>
#include <vector>

void solve(int n);
std::vector<int> chosen;

int main(void) {
  int n;
  std::cin >> n;
  solve(n);
  return 0;
}

/**
// wrong answer
void dfs(int n, int start) {
  if (start > n) {
    for (const auto& val : chosen) std::cout << val << " ";
    std::cout << std::endl;
    return;
  }

  for (int i = start; i <= n; ++i) {
    // not choosen
    dfs(n, i + 1);  // 每一个元素试探当前位置，都会有不放的情形，造成重复。

    // choose
    chosen.push_back(i);
    dfs(n, i + 1);
    chosen.pop_back();
  }
}
*/

// right answer
void dfs(int n, int level) {
  if (level > n) {
    for (const auto& val : chosen) std::cout << val << " ";
    std::cout << std::endl;
    return;
  }

  dfs(n, level + 1);

  chosen.push_back(level);
  dfs(n, level + 1);
  chosen.pop_back();
}

void solve(int n) {
  chosen.clear();
  chosen.reserve(n);
  dfs(n, 1);
}
```

#### [93.递归实现组合型枚举](https://www.acwing.com/problem/content/95/)

- 有了上面的铺垫，这个题就清楚多了。
- 其实，搜索这一块，主要的思路就是组合型和枚举型。指数型还是比较特殊。
- 对于该题，那就是试探的思路。
- 组合和排列最大的差异在于，是否有序，组合这里按照一个顺序遍历，保证了有序性。

```cpp
#include <iostream>
#include <vector>

void solve(int n, int m);
std::vector<int> chosen;

int main(void) {
  int n, m;
  std::cin >> n >> m;
  solve(n, m);
  return 0;
}

void dfs(int n, int m, int start) {
  if (chosen.size() == m) {
    for (const auto& val : chosen) std::cout << val << " ";
    std::cout << std::endl;
    return;
  }

  for (int i = start; i <= n; ++i) {
    chosen.push_back(i);
    dfs(n, m, i + 1);
    chosen.pop_back();
  }
}

void solve(int n, int m) {
  chosen.clear();
  chosen.reserve(m);
  dfs(n, m, 1);
}
```

#### [94.递归实现排列型枚举](https://www.acwing.com/problem/content/96/)

- 组合的思路，已经总结完了。排列因为考虑顺序，所以每层都是最大宽度，同时引入vis标记之前选过的元素。
- 排列这里，level和start都不用

```cpp
#include <iostream>
#include <vector>

void solve(int n);
std::vector<int> chosen;
std::vector<bool> vis;

int main(void) {
  int n;
  std::cin >> n;
  solve(n);
  return 0;
}

void dfs(int n) {
  if (chosen.size() == n) {
    for (const auto& val : chosen) std::cout << val << " ";
    std::cout << std::endl;
    return;
  }

  for (int i = 1; i <= n; ++i) {
    if (vis[i]) continue;

    chosen.push_back(i);
    vis[i] = true;

    dfs(n);

    chosen.pop_back();
    vis[i] = false;
  }
}

void solve(int n) {
  chosen.clear();
  chosen.reserve(n);
  vis.clear();
  vis.resize(n, false);
  dfs(n);
}
```

#### [46.Permutations](https://leetcode.com/problems/permutations/)

- 排列型枚举，dfs基本题
- 不过有```next_permutation```也可以用
- 由于考虑不同序列，所以每次需要从头开始遍历，那么需要记录path上已经访问的元素，存储index即可。
- 二刷
    - 考虑组合题，使用start，本质是做了剪枝。即搜索的时候，采用线性顺序搜索备选集，不会出现[1,2],[2,1]，don't look back manner
    - 排列题，采用look back manner，即[1,2]/[2,1]都是合法解。
        - 但是look back的时候，需要判断当前chosen的元素，不能在放进来，因为不能重复放.
        - [1,2,3,4,5]，比如有[2,4]，下一步试探时，还是从1-5，但是2肯定不能放了，因为试探了，4也不能放。
        - 引入vis，状态和chosen保持一致，起一个hash的作用，方便快速查找

#### [47.Permutations II](https://leetcode.com/problems/permutations-ii/)

- 首先排序，保证升序。
- 去重序列生成的核心逻辑在于
    - 相同元素，在同一个层次，不用反复试探。
    - 注意同一个层次的限制
    - 方法一：预处理排序，判断相邻元素是否一致同时不在一条子树上，即可
    - 方法二：从组合那里学来的，更加通用的处理方法。利用hash过滤本次同一个元素的反复试探，hash数组空间换时间。
        - 这个方法有一个注意点是，一定要先隔离出本层可以试探的节点，组合那里用start做了隔离。
        - 排列这里，start不管用。只能用vis数组。
        - 这个方法更通用，我比较喜欢。当然，由于这个题也比价基础，所以两种方法都需要熟练掌握。
- 二刷
    - 感觉还是比较简单的。在46的基础上去重
    - 去重的原则就一条：相同元素在同一个层次，不反复试探。
    - 增加层次试探集合，该集合局部变量，无状态，所以也没有回溯一说。

#### [77.Combinations](https://leetcode.com/problems/combinations/)

- 一刷
    - 基础题，利用acwing的模板求解即可。
    - 方法二，利用公式。但是公式这里注意，公式只说了个数是一样的，但是这个题求的是排列集合这个元素，所以不一样。这个题的求解逻辑需要单独推到。详见[77. Combinations](https://grandyang.com/leetcode/77/)
- 二刷
    - 思路上没问题，做的也很快。但，不是最优解。漏了一个剪枝
    - 为啥没想到呢？因为忽略了一个重要因素，组合不考虑元素顺序，所以我们只能按照线性顺序，这单一的顺序进行遍历。
        - 这会产生剪枝的条件，比如我当前槽位试探最后一个元素，那意味着，下一层没有可试探的元素。

#### [39.Combination Sum](https://leetcode.com/problems/combination-sum/)

- 这个题是组合基础题，根据模板，剪枝条件一定要确定清楚。
- 模板的起点下界是递增的，但是该题是非递减。模板的终点是n - (k - chosen.size()) + 1，但是该题是n，因为该题的区间长度不限，所以起点的上界是n
- 二刷
    - 二刷做起来反而没有那么轻松，想了一会
    - 组合题，单向遍历，去重
    - 但是这个题还有一个重复的可能，就是如果元素重复，又因为这个题可以重复选。
        - 比如，[2,2,3],target=6。可能造成[2,2,2],[2,2,2]这样重复的序列
        - 所以，题目的要求很重要，元素去重。这样不会造成上面的情形。
    - 序列重复可能有多重原因，一种是排列导致的，比如[2,2,3]/[3,2,2]，本质是一个排列
    - 另一个就是元素重复。所以，要学到处理他们的技巧
    - 思路：一上来，没有搞太明白。最后的解题的钥匙在于，确定每层的宽度，深度这两个突破口来进行思考
        - 组合去重，用单向遍历。
        - 元素可以重复选，单向不递增即可。
        - 同时，题目要求了元素不重复，可以避免重复组合的生成。
- 三刷
    - 一遍没过，调整了下。题目会意错了。
    - 题目的意思是，每个元素可以用多次，但是组合的搜索顺序不能变。
    - start的更新是i，常规组合题是i + 1
    - 这个题还有一个点是，没有level的限制，如果元素有0，肯定会stackoverflow，不过题目给了限制，所以不用考虑

#### [40.Combination Sum II](https://leetcode.com/problems/combination-sum-ii/)

- 这个题对应47题，重复元素的组合生成。上面是重复元素的排列生成。
- 方式一样，避免相同元素在同一个层次的试探。
- 注意，vis的用法和排列不同，前者两种剪枝用到了这个结构，但是组合只用到了一种。使用过的元素不会重复使用，这个在排列中用startIndex保证。
- 二刷
    - 思路采用了随想录的做法，我觉得随想录讲的也非常的清楚。即同一层的试探需要剪枝，同一树枝的试探则不用。
    - 同时，随想绿优化的做法更好，不用used数组，就是判断同层即可。
    - 备选集需要排序。
    - 剪枝start边界不要判断(start >= sz)，这里会漏解
    - 总结：对于元素重复的处理，主要是利用排序+同层剪枝

#### [216.Combination Sum III](https://leetcode.com/problems/combination-sum-iii/)

- 这个题和前两个兄弟题的区别是，前两个题也是产生组合，但是对于组合的长度没有限制。这个题给了限制。
- 此时，可以用acwing的模板。
- 注意，由于组合的长度有限制，注意宽度的剪枝。
- 二刷
    - 整体来说，比上一题简单。主要是很多限制都没有
    - 元素重复的限制。
    - 元素可以重复用的限制。
    - 增加了一个k层的限制，这个比较好处理，同时k层的限制有剪枝优化。
- 新思路
    - 这个题最大的get是又进一步搞清了指数型和组合型的差异。
    - 上面的总结主要是说，组合型不能使用与指数型的原因。因为组合型没有不选的能力。
    - 这个题，说的是指数型可以应用于组合型解题。
    - 但是，这里的适用是有条件的
        - 元素不能重复，否则去重的逻辑，需要支持。(当然支持了也没问题)
        - 每个元素只能出现一次。因为指数型就是选或者不选
    - 技巧上注意
        - 指数型的层次是固定的，就是把所有元素展开成n层，每层对于每个元素，就是选或者不选。
        - 层次递归返回条件要后判断，否则会漏解。
- 三刷
    - 方法一：很明显，指数型。但是没有一遍过，调整了一下。
        - 几个问题，首先是9个数，层次最大到9。这个递归边界漏掉了
        - 其次，这个题递归返回的条件是k层满足，同时sum满足。要求和acwing的指数型有一些差异
    - 方法二：组合型。从槽位试探的角度来理解。一遍过，但其中有些犹豫
        - 这个level，其实在组合型没有用。因为组合型是线性试探，最后没有元素试探了，自然会结束。
        - 但是，如果指数型没有level，会一直试探，暴栈。
        - 这个题也能用指数型，也能用组合型的原因在于。acwing那道题，都不放是一个合法解。组合型模拟都不放的情形时，会出现重复，无法拿到正解。
        - 但这个题，任意一个合法解，都需要放置元素，所以组合型肯定可以做。
        - 其实二刷的讲解也很清楚，本刷又确定了。指数型能力要强于组合型。acwing-92是组合型不能解决，但指数型可以。本题是都行。
        - 最后，这个题可以剪枝。

```cpp
// 组合型
void dfs(int k, int target, int sum, int start) {
    int level = chosen.size();
    if (sum > target or level > k) return;
    if (sum == target and level == k) {
        ret.push_back(chosen);
        return;
    }

    for (int i = start; i <= 9 - (k - level) + 1; ++i) {
        chosen.push_back(i);
        dfs(k, target, sum + i, i + 1);
        chosen.pop_back();
    }
}
// 指数型
void dfs(int k, int target, int level, int sum) {
    if (target == sum and chosen.size() == k) {
        ret.push_back(chosen);
        return;
    }
    if (level > 9) return;

    dfs(k, target, level + 1, sum);

    chosen.push_back(level);
    dfs(k, target, level + 1, sum + level);
    chosen.pop_back();
}
```

#### [17.Letter Combinations of a Phone Number](https://leetcode.com/problems/letter-combinations-of-a-phone-number/)

- 这个题有两种思路理解：
    - 方法一：组合法
        - 首先需要判断是排列还是组合，这个题是组合，因为需要保证次序唯一性
        - 深度，不需要。宽度，起点下界是start，上界不剪枝。
        - 注意class static var的初始化
    - 方法二：不区分组合还是排列
        - 组合和排列的区别在于，对于同一个集合的元素遍历，是否考虑次序。
        - 本题显然不是一个集合的问题，每一层有一个独立的集合，需要全部遍历。
        - 上面区分排列和组合，意思其实在于剪枝。这个题，由于集合不同，无法剪枝
- 二刷
    - 二刷时没有一刷的考虑，感觉思路上，尤其是搜索的思路还是比较简单的。
    - 因为每一层的备选集都不一样，所以我觉得其实是排列的思路，没有剪枝的逻辑。
    - 组合本质还是剪枝了重复元素构成的序列
- 三刷
    - 思路非常简单，还是没有一遍过。
    - 空串的情况没有处理正确，如果不特殊处理，最终集合会加入一个空串，但题目要求是空集
    - static const member initialization一开始也没写对
    - 这个题不需要start，组合的办法，可以理解为按照线性顺序遍历是做了剪枝

#### [131.Palindrome Partitioning](https://leetcode.com/problems/palindrome-partitioning/)

- 这个题第一次没做出来，主要是没有找到解题钥匙，即没有转化问题。随想录的思考过程非常好
- 切割问题，可以抽象为组合问题。但不完全一样，因为组合只要递归到最后的层次，肯定是解，但切割不是。
    - 上面说的组合，也只是说，起点的下界需要递增，这个是切割的特性，切割只能向后切，不能向前切
    - 类似组合的原因在于，切割点，可以类似从组合中选一个元素出来。
    - 第k刀在第k-1刀的基础上进行枚举所有可能的切割点。这个是核心思路。
    - 同时，第k刀的枚举点是否合理，取决于这个串是否为回文。如果不是，直接剪枝。尝试第k刀的下一个切割点。
- 如何模拟切割线。这个转化为切割起点即可。
- 切割问题如何终止。知道存在非法的切割起点，即切割起点枚举到了序列终点。即起点的下界越界了，这个是第一次使用起点下界的条件作为递归边界。
- 二刷的心得
    - 首先，忘了之前的组合的原理，完全就是最朴素的思路，没有太想模板的事。
    - 其次，就是枚举题，看怎么枚举。需要一个完备的划分，这个完备很重要，即划分到底。
    - 再者，划分的核心技术解决，如何把子串找出来，因为你不枚举子串，因为枚举子串的思路无法构成一个划分，所以本质是枚举划分。
    - 最后，非常巧妙的把划分点，转化为本次子串的起点/终点，即利用子串的起点和终点，来标识划分。
- 三刷
    - 三刷就很轻松了，解题的钥匙也知道，就是分割点。枚举分割点即可。
    - 分割点作为本次枚举的终点，下次枚举的起点。
    - 分割点的终点是没有地方可以划分，这是递归边界。
    - 不过这个题也没有一次过，判断回文那里写错了，不过自己定位还是比较快，非常典型的!=导致的问题，用<也是有原因的。

#### [93.Restore IP Addresses](https://leetcode.com/problems/restore-ip-addresses/)

- 这个题在131的基础上就容易理解多了。前者求解划分，划分需要是回文，本题一样，划分需要满足ip地址的需求。
- 边界条件有不同，本题划分只要求是4个。
- 子串转数字，随想录的解法提供了更好的办法，就地转。不要用stringstream
- 有一个特别需要注意的点，回溯的时候，对于chosen集合，不要给出额外的修改，否则会影响回溯。
- 二刷
    - 确实，这个题有了上面划分字符串的基础，思路很明显，就是分割题。枚举分割点(终点)即可
    - 这个题有一些麻烦的地方，还是围绕那4步骤来说。
    - pruning: 常规的是2个，leading zero + number range，但是还有一个就是已经选到的个数，题目要求最终是4个。
    - 剩余的步骤，在实现上能简单点，依靠的是我用了vector<string>来表达chosen，而不是string.
    - 递归边界也要小心，上面剪枝的条件只保证，小于等于4个，但是题目要求是4个，所以最后边界这里做判断。
    - 难度适中，但是细节多。
- 三刷
    - 有了131的基础，这个题确实简单很多，枚举划分即可，划分的重点不再强调。
    - 不过这个题细节很多，我做的好的地方在于，遇到问题排查的都很快，也没花太多时间。
    - 这个题的排查中也暴露两个有意义的问题
        - stoi会抛异常，这点在实际工作中注意，用atoi也是有道理的。
        - **backtracking过程中，中间状态除了try current/backtracking这两步修改，其余地方不要修改**。
            - 类似本题，如果在插入结果时pop_back，那么回溯后还会pop_back，导致coredump
            - 如果要修改，就保存临时变量即可。
    - 这个题需要level，因为题目限制了最终结果即ip地址只能由4段组成

#### [78.Subsets](https://leetcode.com/problems/subsets/)

- 有了划分的基础，子集问题就显得非常容易了，基于排列做，没有剪枝。
- 随想录有一个好的总结是，之前两题都需要遍历到叶子才形成一个合法的path，但是这个题不需要。非叶子只要合法，就放进来。
- 二刷
    - 这个题反而做的没那么容易，因为跟在前面的划分问题做，但其实这个题和划分问题关系不大。
    - 关系不大是因为，划分要求的是一个连续的结果集合
        - [1,2,3]有[1,2,3] and [1], [2,3] and [1,2], [3]
        - 但是子集肯定会有[1,3]这样的元素，这显然不是连续的。
        - 所以，这个题我一开始没做出来是因为，卡在划分了。后来划了下st，才发现很简单的组合题。
    - 不过这题也好，起码进一步明确了和划分的区别。

#### [90.Subsets II](https://leetcode.com/problems/subsets-ii/)

- 生成集合的元素存在副本，如果按照之前的办法，会产生重复的集合(组合)。
- 和之前排列，以及组合的处理保持一致。如果相邻元素相等 and 不在同一个层次，此时剪枝
- 二刷
    - 78的基础上，就一个点：相同元素，同层不试探即可。
    - 注意排序预处理

#### [491.Increasing Subsequences](https://leetcode.com/problems/increasing-subsequences/)

- 这个题整体思路比较清楚，但是剪枝的实现一致没实现对。
- 剪枝
    - 递增
    - 同一个子树，本层重复节点，不能反复试探。这个逻辑我一直没实现对。注意，不能排序。start的本质是下标递增，不是元素递增。
- 优化，我在进行去重的时候，是一个O(N)的做法，随想录引入了hash数组，空间换时间，从而有O(1)的时间复杂度。
- 二刷
    - 子集问题，节点生成时，保留状态。
    - 这个题有一些注意点
        - 首先，不能排序处理相同节点对于同层的试探。引入集合处理，集合需要是栈变量，不带状态。本质是和同层处理定义在一起。即然无状态，没有回溯一说
        - 其次，节点保留状态有条件，必须是两个元素才行。
    - 这个题我又复习了下以前的做法，发现还是当前的做法最简单，核心点是，相同的元素同层不能反复试探。
    - 以前怎么解决这个问题，有一个vis数组，记录以前访问过的元素。
        - 如果这个元素已经访问过，那么和它相同的元素，本层可以放。
        - 因为相同元素放在了不同层次。
        - 但是，我如今的做法，在一个for循环中，肯定是当前层都可以放的元素，天然过滤了以前放过的元素。

#### [332.Reconstruct Itinerary](https://leetcode.com/problems/reconstruct-itinerary/)

- 这个题我在二刷的时候，觉得非常容易，思路也非常简单，但是这要感谢一刷不放弃的精神，不断的调整，融合不同的题目，思考的深度够了，最后解决了问题，也真的搞明白了。
- 这个题，我倾向于用carl的思路，整个题目从他的角度还是归到了回溯法，而没有归到图论，我也是这个看法。
    - 其实图论的dfs算法，我理解是在图这个结构上，应用递归的一种做法。当然，图的结构比较特殊，和搜索树一致。但是图的关联关系非常自然，以直接的形式给出来。
    - 但是，正常的搜索算法，状态与状态的关联，也就是所谓的边，不是自然的存在。
    - 以这个题来说，我用图来组织数据。但是搜索树和这个图并没有关系，图按照carl的理解，就是每层备选集的集合。即，图只是每层备选的集合。
    - 所以，这个题我的解法非常好的一点是，备选集我用图来组织，非常好。
- 这个题的一些关键思路
    - 备选集如何组织，我接住了图的知识
    - 边界条件，这个是所有边，当且仅当访问一遍。
    - 如何优化，不用找到所有解，这个是有技巧的
        - 首先边按字母序排序
        - 只要找到一个解，整个程序结束。但是由于递归函数，不能直接结束，只能递归返回。这个题在sudoku时，get了另一种递归返回的办法。
- 三刷
    - 没有直接做出来，说一下问题。
    - 其实整个思路还是比较简单的，反而没有之前的dfs那么麻烦。这个题不好搞的地方在于，对于pruing,ds这些条件的转化
        - 比如，图你怎么组织？
        - 都知道要剪枝，要去重边？这个怎么做？字母序怎么处理
        - dfs找到一条路就返回，返回值怎么设计？
        - 每条边当且仅当访问一次，怎么保证？
    - 下面分别说一下
        - 图是用领接表组织，我这次写的时候，数据结构设计的是正确的，但是对于边没有处理正确。本质是按算竞的思路，存的是adj_node and properties of the edge，这个对我有干扰。其实都理解成edge也没有问题。
        - 剪枝，主要是要对边去重，并且还要求边当且仅当使用一次，我有一个非常好的设计是引入了seq标识边，配合vis数组去重。
        - dfs返回，这个只能引入返回值。返回值怎么设计，主要是true的情形，即每条边当且仅当访问一次，怎么判断？
            - 由于有剪枝，每条边不会重复访问，所以每条边当且仅当访问一次，就是访问边的数量和整体边数量一致。
            - 这些都是细节，需要把问题转化为可以操作的东西。
        - 每条边当且仅当访问一次，上面说过了。

#### [51.N-Queens](https://leetcode.com/problems/n-queens/)

- 在说这个题之前，还是说一下332这个题，这个题是一个承上启下的题目。关键是教会了我怎么样生成搜索树，即状态之间的联系怎么生成。当然carl的模板也很重要，强调了这个过程就是在第二部枚举备选集合。
- 有了332的基础，这个题感觉轻松多了。
- 说一下细节
    - vis数组，我最初的实现是一个二维的，但其实一维就够。index代表row, vis[index]代表col。不过，其实没有也ok，因为有chosen数组，可以还原vis
    - 对角线的判断，采用了以前的办法。
    - 如果使用chosen数组，其实level不必要。如果是acm直接开数组，则需要level
- 二刷
    - 整个搜索的流程没有感觉到难度，主要是剪枝条件的实现。
    - 行不用处理，列也比较好处理，主要是对角线。
    - 我的办法是，列单独放了哈希表处理，对角线把之前的皇位位置存下来，然后和当前试探比较。
        - 这个不是最好的办法，时间复杂度O(1) + O(N)，没有上升量级，但是可以优化
        - 最好的办法是，存一个列放置位置即可。因为，数组offset是行位置，每一个元素是列位置，等价于存储了之前放置的皇后位置。
        - 对角线的判断是看行差异和列差异是否一致即可。

#### [52.N-Queens II](https://leetcode.com/problems/n-queens-ii/)

- 这个题在51的基础上，不需要chosen and ret.
- 但是必须有vis
- 二刷
    - 同51二刷，不过这个题有返回值，但是递归不能有返回值，因为这个题并不是只找一条路。
    - 还是需要找到所有路。

#### [37.Sudoku Solver](https://leetcode.com/problems/sudoku-solver/)

这个题非常好，在dfs的时候，很多需要注意的点，都有。但是遗憾的是，我在一刷的时候，其实已经快到终点了，但是最后放弃了。
其实，在往前走一步，仔细分析以下，这个问题就解决了，还是缺乏耐心，解bug才是最能提升能力的时候。

- 先说思路
    - 核心问题是如何解决：生成树的边。即状态之间如何迭代。
    - 对于传统的chosen数组，level + 1，一般用来建立生成树的边，即迭代状态，但是这个题是个二维的。
    - 思路也很自然，如果一个变量不能迭代状态，那就再增加一个变量，况且这个题本身用二维数组展示，用(i,j)来想到状态的迭代并不难，这点我也做到了。
- 技巧
    - 递归边界
        - 所有二维点枚举完成。这个想到了。
        - 如果已经找到答案，返回。这个也想到了，搞了一个全局的ans变量。
    - 当前层试探
        - 枚举1-9即可
        - 注意：chosen，作为试探结构，在找到结果时，必须马上保存，否则前者会回溯，丢失结果信息。这个也想到了。
- 问题
    - 对于vis数组，没有预先初始化。这个直接导致了有bad case.
        - 我当时的思路是，对于非数字的数组，直接更新就好。但是这个的问题是，如果数字在前，非数字在后。那么前面数组选择的时候，不会受到约束。
        - 另一个我没想到的点是，对于试探结构，一般都不用回溯。这个题必须回溯！
            - 回想，当前在(i, j + 1)，假如这个位置的试探，没有一个解。且board[i][j]没有回溯。
            - 那么，回溯到(i, j)试探了新的元素，再进行(i, j + 1)试探时，由于board[i][j]已有数字，不再试探。从而，错过了一次试探机会。
            - 当然，除非不用board[i][j]来进行判断。这点特别小心
- 新技能get
    - 对于dfs拿到结果解直接返回的case，我们知道此时是无法直接返回的，因为递归调用导致了stack frame嵌套，goto无法解决，只能递归返回。
    - 递归返回时，由于可能会触发新的dfs，此时要避免这种情况。
        - 方法一：我之前是利用全局ans，增加一个递归边界。这样即使触发了新的dfs，由于之前已经标记过ans，此时dfs不会真的试探，会利用新的递归边界，直接返回。
        - 方法二：也是这次新get的，给dfs一个返回值。这样，递归返回时，根本不会触发新的dfs. 上面的办法是，触发了，但是加了新的边界条件。
- 二刷
    - 这个题花了一番周折才过，主要还是细节做的不好。
    - 这个题接口设计对我没什么问题，就是按照dfs(level, start)的思路
        - level本质是控制search tree的深度，即当前的试探点，所以需要(i,k)
        - start本质是本层可以试探的元素集合，start标明起始位置，本题从1-9，先验，不需要。
    - 这个题做的不好的地方在于，剪枝条件，3个条件都是似是而非，一个都没写对。
        - row/col这个，做的不好，二维没意识到。关键是，排查的时候，也没有进一步排查，丧失了一个进步机会。
        - 对于这个反复做的题目，不是第一遍做，所以如果想涨功，一定是在debug时获得。
        - 对于box，这个自己debug出来。其实，debug时发现最大的问题就是，元素放不进去。
            - 现在想，就是剪枝条件有问题，第一时间没意识到。
            - 通过debug init发现的。最后，定位到这里。
        - 还有一处就是，init忘了，不过这个自己debug出来，不提。
    - 其它细节
        - dfs找到一条路就返回的处理是通过返回值
        - 字符和整型这种转换，收敛，毕竟多处写时不一致    

#### [207.Course Schedule](https://leetcode.com/problems/course-schedule/)

- 这个题最好的办法使用下面的bfs去解。
- dfs就是多提供一个方法
    - ```bool dfs(x)```语义是从x点进行dfs，判断是否有环
    - 核心思路：对于x，从x处进行dfs，在backtracking到x之前，不能再次访问到x
        - 对于DAG，可以有多个节点指向x，所以需要```visited```记录访问状态，避免重复dfs
        - 对于DAG，可以有多个节点指向x，但x在backtracking到x之前，不会再次访问x
    - 解法：对于```vis```状态的扩展，先试探标记，等到backtracking发现没有多次试探，那么此时正常标记即可。

#### [278. 数字组合](https://www.acwing.com/problem/content/280/)

搜索这里，又有了进一步的认知。
- 算竞里面提到的指数型枚举，方法非常直接，但是我一直和代码随想录的方法结合的不太好。
- 代码随想录给出了搜索的通用做法，由于是个通用的做法，势必将各种情形融合在一套代码中。
- 简单说，算竞的做法更精细，更巧妙。代码随想录的做法则更通用。
- 这个题最优解是dp，但是讲搜索也是非常合理的。
    - 方法一：按照算竞的思路，每个数要么放，要么不放，指数选择。
    - 方法二：按照随想录的思路，每一层找好起始位置和结束位置，进行枚举试探即可。
        - 其实方法二也有level的概念，只不过vector不用显示指定，如果数组则需要
- 二刷，再说下上面说的，方法一和方法二的区别。
    - 方法一：方式非常直接，指数型枚举，每个数就两种可能，放或者不放
    - 方法二：其实是组合的思路，每个数都会选，不存在不选的可能，看看选哪一个组合出来，这个组合的和满足要求。
    - 所以，方法一和方法二在思路上有根本的区别。
    - 当然，随想录的方法也可以使用，即当前层的[start, end]对于当前层来说，就是不选，和这个数。

```cpp
void dfs1(const vector<int>& nums, int target, int level, int sum) {
  if (sum == target) {
    ++ans;
    for (const auto& val : chosen) { cout << val << ","; }
    cout << endl;
    return;
  }
  if (sum > target) return;
  if (level == nums.size()) return;

  // not choose
  {
    dfs1(nums, target, level + 1, sum);
  }

  // choose
  {
    chosen.emplace_back(nums[level]);
    dfs1(nums, target, level + 1, sum + nums[level]);
    chosen.pop_back();
  }
}

void dfs2(const vector<int>& nums, int target, int start, int sum) {
  if (sum == target) {
    ++ans;
    for (const auto& val : chosen) { cout << val << ","; }
    cout << endl;
    return;
  }
  if (sum > target) return;
  if (start == nums.size()) return;

  for (int i = start, sz = nums.size(); i < sz; ++i) {
    chosen.emplace_back(nums[i]);
    dfs2(nums, target, i + 1, sum + nums[i]);
    chosen.pop_back();
  }
}
```

### BFS

#### [207.Course Schedule](https://leetcode.com/problems/course-schedule/)

- 题面是判断图中是否存在环，可以转化为top sort
- 这个题基础top sort
- 注意边如何组织，我是用acwing的建议方式，把边压到一个一维数组里面。看了答案之后，我采用method2/method3的方式来组织图更方便。

#### [210. Course Schedule II](https://leetcode.com/problems/course-schedule-ii/)

- 这个题也是基础top sort，基于bfs搞定

## 动态规划

关于最优子结构，wiki有如下解释

>In computer science, a problem is said to have optimal substructure if an optimal solution can be constructed from optimal solutions of its subproblems. This property is used to determine the usefulness of greedy algorithms for a problem

我做如下理解
- 原问题的最优解包含子问题的最优解。从而我们可以通过子问题的最优解推导出原问题的最优解。
- 也即dp中后面的状态，可以通过前面的状态推到出来。
- 这点很重要的原因在于，如果没有这个性质，那么状态之间无法推到，就无法决策。所以，dp状态的寻找很关键。

关于滚动数组，有如下理解
- 用途：常见于dp状态更新
- 目的：节省存储空间
- 做法：使用固定的存储空间进行状态更新。
    - 每一次操作仅保存有用信息
    - 新的元素不断循环更新，使数组空间看上去像被滚动利用

#### [1049.Last Stone Weight II](https://leetcode.com/problems/last-stone-weight-ii/description/)

- 一刷：随想录学习。
    - 这个题昨天尝试dfs过，思路有了。没有想到背包是因为没明白问题如何转化。
    - 所以，这个题巧妙的一点是，把问题转化了：本题其实就是尽量让石头分成重量相同的两堆，相撞之后剩下的石头最小，这样就化解成01背包问题了
        - 从这个点来说，重量相当的两堆石头。可以进一步理解成，大家都想总重量一半靠拢，这样肯定是最接近的。
        - 所以，最终转化为，容量为一半时，质量也恰好一半。即恰好装满一半背包时的情形。
        - 但是，这个题可不是恰好，因为恰好需要肯定能对半分。但是这个题不一定，所以，它和target sum相似，但不完全一样。
        - 最终，这个题是这样的，尽量装满一半时的价值，因为价值和容量一样，所以这里的核心不在于讨论价值，而在于如何尽量装满一半。
        - 这个题还是需要结合target sum再理解一下。因为，这种题放到搜索，本质是划分，如何找到一个近似的划分，所以这里我理解核心不是讨论价值，而是借助dp来讨论划分。
        - 因为，约接近一半的划分，其实价值就越大，因为物品的容量和价值一样。所以，这种题，找到的是一个最大划分。
        - 再回过头看，容量为一半时的最大价值，因为容量和价值一样，所以，这个说的是，找一个最近接容量的划分。
        - dp[k]表示容量为k时的一个最大划分，这个划分的容量最大，最靠近k。背包的问法是容量为k时的最大价值。但是价值和容量一样。
        - 所以，这个题其实找接近某一个容量的一个最大划分，这个划分的容量接近target value.

#### [494.Target Sum](https://leetcode.com/problems/target-sum/description/)

- 一刷：随想录学些。但是没过。
    - 说下思路，没有理解。只能强行记住。
    - 没过的原因在于，忽略了负数绝对值的情形，这也是我觉得这题做的不好的地方。这其实和dp无关，考察对于边界情况的处理
- 二刷：参考了[494. Target Sum](https://leetcode.com/problems/target-sum/description/)
    - 这里我说一下，由于目前在学习dp，所以和之前的办法还不能融合的特别好，即这个题，到底用搜索还是dp来做。
    - 在这一章，肯定是dp了。但是这样往往会错失最容易理解的办法。
    - 说回题目，看了下上面的题解，很快启发我。因为搜索我觉得我搞得比较透，所以对于背包来说，其实就是指数型枚举。
        - 先可以尝试搜索，一题多解。这样也能更好的理解各种方法的差异。
        - 这个题典型的指数型枚举，每个数都必须放，只不过以前的是放或者不放，这次是必须放，选择在于放本身以及相反数。
        - 这个题没处理好的是边界，不过自己最后debug解决了。

#### [416.Partition Equal Subset Sum](https://leetcode.com/problems/partition-equal-subset-sum/description/)

- 一刷：随想录学习。但是也没过。
    - 思路主要学习了随想录的办法，0-1背包，和指数型枚举具备内在一致性。所以指数型枚举的搜索题也可以考虑背包。
    - 这个题的解法还是比较巧妙的，即容量为sum/2的背包，看它的价值能否是 sum/2
        - 如果可以，找到了一个集合划分
        - 如果不可以，没找到。
        - 容量和价值一样，是这个题最大的特点。
    - 做的不好的地方
        - 背包的模板给忘了，滚动数组dp[k] = max(dp[k], dp[k - nums[i]] +nums[i])，我写成了dp[k] = max(dp[k - 1], ...)
        - 想想滚动数组怎么更新的，nums[i]不放，不用更新的。
        - 还有初始化的vector大小开多少，可以优化。这个也是没考虑到的点

#### [96.Unique Binary Search Trees](https://leetcode.com/problems/unique-binary-search-trees/)

- 一刷：随想录学习。状态定义比较难想到。
    - dp[i]表示节点为i个个数时，共有的布局个数，也即可以组成的不同bst个数
    - dp[i] = sum(for each j in [1, i] then dp[i] += dp[j - 1] * dp[i - j])，注意j是闭区间。左右子树可能为空
    - 完全是通过归纳，发现的重叠子问题规律，这个题突破口也是重叠子问题。
    - 一维数组，二维展开
    - 初始化注意，空节点也是一颗二叉树。

#### [343.Integer Break](https://leetcode.com/problems/integer-break/description/)

- 一刷：随想录学习。这个题解题的钥匙是重叠子问题。
    - 我刚开始做的时候，处理不好的地方是，不知道怎么用dp来表示，即状态不知道怎么抽象
        - 个数不确定，这个是核心，不知道怎么处理
    - 开了随想录的解法后，发现打开的方式应该是这样
        - 首先，题面是求个数，不是最优化。所以，dp的状态一定是总数
        - 其次，既然是总数，那么就尝试看看，是不是会有重叠子问题。
        - 最后，发现对于一个数n，可以这么寻找
            - 对于任意的k(1 <= k and k < n)
            - k * (n - k)，分解成两个数的乘积。但是，也可能多个数，所有还有
            - k * dp[n - k]
            - 对于dp[n - k]又有如上的步骤，非常类似递归每一步求解一个子问题的情形，这样算出了所有。
    - 整体状态定义还是比较抽象，即不容易感知底层细节，但是抽象的状态非常准确。
    - 注意状态转移方式，一维数组，两维展开

#### [63. Unique Paths II](https://leetcode.com/problems/unique-paths-ii/description/)

- 一刷：有了63的基础，很容易过了。
    - 这个题如果是搜索，思路非常简单。碰到不满足的情形，执行对应的剪枝即可。
    - dp这里，对于状态转移要做调整。
        - 如果不好直接分析，我们可以分析最后一个点，如果它的上面路被堵死了，那么dp[i - 1][j] == 0。同理，左面的处理方式一样
        - 说会状态转移本身，看起来dp[i][j] = dp[i - 1][j] + dp[i][j - 1]不用变，只要前面的状态根据障碍物更新即可。
        - 很显然，刚开始分析这么看没问题，但实际，这么做行不通。
        - 还是考虑状态本身dp[i][j]，表示有多少条路可以到(i, j), 如果当前点有障碍物，肯定到不了。否则，就依赖于(i - 1, j)和(i, j - 1)的状态即可。他们都有障碍物，那就是0，也没问题。

#### [62. Unique Paths](https://leetcode.com/problems/unique-paths/)

- 一刷：思路学习
    - 这个题还挺好的，比较清晰的展现搜索和dp的区别。
    - 搜索思路
        - 题意是求总和，很自然想到搜索，枚举解空间即可。
        - 这个题怎么枚举？采用随想录模板，确定好level和start
            - level就是(i, k)
            - start就是向右和向下
            - 和sudoku的方向一致
            - 这个题我刚开始没反应过来枚举的写法，会想到sudoku，一下豁然开朗
    - 当然，这个题会超时。超时的原因在于时间复杂度，另一个重要的原因是重叠子问题。
    - 所以，虽然是求总和类题目，但是因为重叠子问题的情形，我们采用dp来做。
    - dp思路
        - 状态怎么找？因为有了fib的基础，反向找，发现最后一个状态，只能是两个方向来，都是迈一步，所以有dp[i][j] = dp[i - 1][j] + dp[i][j - 1]
        - 初值定义，边界都是1
        - 状态转移，这个题已经不是线性dp，正常行优先即可。
        - 最后结果，是dp[m - 1][n - 1]
    - 滚动数组
        - 先系统学习了下滚动数组，简言之，考虑到dp在进行状态转移时，当前状态只依赖之前的一个或某几个状态，没有必要把全部状态都存储下来。这种进行空间优化，节约存储的一种办法叫做滚动数组。
        - dp[k] = dp[k] + dp[k - 1](dp[k] += dp[k - 1])


#### [746. Min Cost Climbing Stairs](https://leetcode.com/problems/min-cost-climbing-stairs/description/)

- 一刷：思路学习。
    - 这个题用dp没有太多问题，直接是最优化问题。
    - 对于状态定义，没有一瞬间想到的原因在于，原问题和子问题的关系，没有找到，因此推不出来。
        - 如果要换回搜索，很容易就想到了，还是迈步子。第n阶，只能从第n-1阶和n-2阶过来。
        - 但是，这两种方法，有成本了。所以，有一个决策在。非常典型的dp
    - 多说下dp的思路，其实重叠子问题，最优子结构，暂时我都没有意识。只是从题面看是最优化，所以决定这个方向。
    - 随想录在后面强调了遍历的顺序，也即推到顺序，这个挺重要的。
        - 算竞将这种推到方式归纳为线性dp
    - 这个题的理解上，也需要仔细点。比如，刚开始的初值怎么定义，最终求解的到底是dp[?].
    - 这个题和climbing stairs一脉相承，一起做有助于理解dp
- 二刷
    - 这个题的突破口，其实是最优子结构，重叠子问题还不好直接看出来。
    - 到底第n阶的最优解，肯定是第n - 1的最优解 + 迈一步的开销 and n - 2的最优解 + 迈两步的开销
    - 所以，你能看出来，这个问题具有最优子结构的性质。
    - 可以考虑，如果第n阶，不一定是n - 1阶的最优解或者n - 2阶的最优解，那就不是最优子结构。
    - 从这个角度看，这个性质有点类似贪心。但是贪心的问题是，不向前看的，只看当前，所以它和原问题没有关系。
    - 但是dp的最优子结构，要求看原问题，这点有别贪心。只不过，当前问题的最优，需要原问题也是最优。

#### [70.Climbing Stairs](https://leetcode.com/problems/climbing-stairs/)

- 一刷
    - 朴素的思路：枚举所有的情形，关键就是怎么枚举？我的思路还是，从子问题和原问题的关系入手，发现有关系，直接找到了原问题和子问题的关系。直接递归求解即可，注意，此时我并没有直接反映到用dp做，但是这么做会超时。
    - 正确的思路：沿着朴素的思路，发现超时，即子问题存在重叠性。没有一开始就考虑dp是因为不是最优化问题，很明显是计数问题即排列组合。我是通过重叠子问题，发现了dp的特性。用dp求解即可。
    - 方法：和fib数的方法一模一样。
- 二刷
    - 这题做的不好，因为没有一遍过。对于dp的边界没有考虑好，犯了和fib类似的问题。
    - 说下思路，上来一看，就全部可行解，那自然的想法是枚举。
    - 至于怎么能转换到dp，有如下思路：
        - dp具有重叠子问题，和最优子结构两个性质。
        - 先尝试划分下子问题，不管是否重叠，然后推到一下，子问题和原问题是否存在转换关系，从而工程转化方程。
        - 对于第n阶，只有两种办法你能过来，从n-1阶，迈一步。从n-2阶，迈两步。这是固定的。所以有，dp[n] = dp[n - 1] + dp[n - 2]
        - 这里需要注意的事，从n-2阶，除了迈两步，还可以迈一步。但是，这么考虑不对，因为我是从原问题出发来的，迈一步的情形会合并到n-1阶中，所以不能这么看

#### [509.Fibonacci Number](https://leetcode.com/problems/fibonacci-number/)

- 同climbling stairs
- 二刷
    - 做的不好，讨论这个题目时，我觉得也不用说dp，因为太明显了。
    - 做的不好的的地方在于，如果只使用两个变量，没有把变量的赋值顺序搞清楚。
        - 每次迈一步
        - 需要先计算下一次的f1值

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

#### [118. Pascal's Triangle](https://leetcode.com/problems/pascals-triangle/)

- 基础题，非常直接的递推结构。

#### [300.Longest Increasing Subsequence](https://leetcode.com/problems/longest-increasing-subsequence/)

- 一刷，这个是系统刷dp的第一题。
- 思路
    - 上来一看，觉得dfs可以搞定，最后超时，时间复杂度O(N!)，此时我还不会记忆化搜索
    - 先复习了算竞的思路，知道用dp求解的应用场景。
        - 问题范围：最值。符合最优化问题，可以尝试用dp
        - dp三要素：最优子结构(当然，这个不是直接看出来的，而是先定义了状态，看看是否满足)，重复子问题，无后效型，满足，这样基本可以使用dp
    - 方法
        - 状态：dp[i]表示包含nums[i]的lis
        - 转移方程：dp[i] = dp[j] + 1, if nums[j] < nums[i]; else dp[i] = 1; finally, dp[i] = max(dp[j] + 1)
        - 边界 初始化：dp[0] = 1
        目标 结果：ans = max(dp[i])
        - 结果这里注意，不一定是dp[n - 1]，因为根据状态定义，最优的结果不一定是dp[n - 1]
    - 分类：线性dp(dp的阶段，沿着各个维度线性增长)

#### [1143.Longest Common Subsequence](https://leetcode.com/problems/longest-common-subsequence/)

- 思路
    - 朴素的思路，上来先dfs，超时。
    - 最优化问题，可以考虑dp
        - 子问题与原问题具有相似性，可以进一步考虑归纳。可简单验证具备重叠子问题特性，当然,我觉得更一般的思路还是子问题与原问题的相似性。
        - 最优子结构，这个我目前没法直接看出，只能是先入为主，考虑用dp求解，然后写一些状态转移方程，从而看出来。
    - 方法：
        - 状态：dp[i][j]表示前缀子串str1[1, i]与str2[1, j]的lcs
        - 转移方程：dp[i][j] = max(dp[i-1][j], dp[i][j-1], dp[i-1][j-1] + 1(if str1[i] == str2[j]));
        - 边界：dp[i][0] = dp[0][j] = 0
        - 目标：dp[M][N]
    - 分类：线性dp
    - 细节
        - dp的状态我没有设计对，和标准答案有diff，我还是设计成和lis一致的，其实不对。
        - 暂时确实没有什么更好的办法理解，只能先硬着头皮先做题。

这里简单对lis和lcs做一个简单的总结，因为它们两题目看起来类似，但是状态设计差别非常多。
- lis的状态，不是序列的状态。用算竞的阶段划分来看，它的阶段是子序列的结束位置。由于不是序列的状态，所以最后的结果还需要再遍历一遍。
- lcs的状态，是序列的状态。用阶段划分来看，已处理的前缀子串长度。
- lcs的状态更自然，更容易想到。为什么lis不能这么设计呢？我尝试了一下，发现lis如果要保持序列的状态，它需要把整个序列存下来，没必要。不然正解的做法简单。
    - 更进一步，lis需要比较cur和整个前缀子串的元素，所以表示如果需要保持序列状态，就需要存储整个前缀子串。
    - lcs只需要比较两个序列的当前元素，不需要回看前缀子串的元素，所以可以保持整个序列的状态。

#### [120.Triangle](https://leetcode.com/problems/triangle/)

- 这个题是非常好的一道题目，充分的体现了dp的思路，很多性质都有非常明显的表现。
- 思路
    - 朴素的思路：上来看了一眼，贪心求解，每个集合把最小的找出来。不对，其实倒不是思路不对，关键是每层贪心的节点可能没有边，不符合题意。
    - dfs的思路求解，每层要剪枝，只有有边的元素，才能枚举，这里不用for循环了，迭代。超时。当然，后期可以用记忆化搜索搞定，同时这里超时的原因是因为重叠子问题，节点回溯后，重复的状态会反复计算。
    - 最后，上dp。不多说，子问题有相似性，且重叠，同时最优子结构可以试出来。
- 方法
    - 状态：dp[i][j]表示从[0,0]到[i,j]所有可能路径的最短路。
    - 转移方程：dp[i][j] = min(dp[i-1][j], dp[i-1][j-1]) + tri[i][j]
    - 边界：这里要注意，上一层可能dp[i-1][j]越界。同时，我增加了行列，来避免对于下标的特殊处理，但是行和列的初值不一样。
    - 目标：最后一层的最短路。

#### [5.Longest Palindromic Substring](https://leetcode.com/problems/longest-palindromic-substring/)

- 参考上面的解答。

## 图论

### 拓扑排序

#### [207.Course Schedule](https://leetcode.com/problems/course-schedule/)

- 题面是判断图中是否存在环，可以转化为top sort
- 这个题基础top sort
- 注意边如何组织，我是用acwing的建议方式，把边压到一个一维数组里面。看了答案之后，我采用method2/method3的方式来组织图更方便。

#### [210. Course Schedule II](https://leetcode.com/problems/course-schedule-ii/)

- 这个题也是基础top sort，基于bfs搞定

## 数学

### Newton's method

迭代公式如下，本质思路：切线是曲线的线性逼近，参见[如何通俗易懂地讲解牛顿迭代法求开方（数值分析）？](https://www.zhihu.com/question/20690553)

- 过点$(x_{n},f(x_{n}))$的切线方程是：$f(x) = f^{'}(x_{n})(x - x_{n}) + f(x_{n})$
- 令切线方程$f(x) = 0$，有$x = x_{n} - \frac{f(x_{n})}{f^{'}(x_{n})}$
- 也即迭代公式：$x_{n+1} = x_{n} - \frac{f(x_{n})}{f^{'}(x_{n})}$

牛顿公式也有一些适用限制
- 二阶可导，否则，并不是100%收敛。
- 收敛点的选择很重要，我看到在一个一正一负区间内，可以加速求解。
 
#### [69.Sqrt(x)](https://leetcode.cn/problems/sqrtx/)

实际求解采用如下方式：
- 变换：sqrt(c) -> let x = sqrt(c) -> $x^{2} - c = 0$
- 求解：令$f(x) = x^{2} - c$，则解为f(x)与坐标轴的交点。
- 过程：切线逼近
    - 取f(x)上一点 $(x_{n}, f(x_{n}))$ 作为初始值
    - 代入迭代公式，进行迭代。其中 $f(x) = x^{2} - c$
    - 有 $x_{n + 1} = x_{n} - \frac{x_{n}^{2} - c}{2x_{n}}$
    - 最终的表达式：$x_{n + 1} = \frac{1}{2}(x_{n} + \frac{c}{x_{n}})$
- 实现：多说一点
    - 初值的选择，我选的 1.
    - 变量的迭代，靠自己更新即可，不需借助其他变量。
    - pre的作用只是保留 上一次的迭代值，用来做条件判断

```c++
double pre = 0.0, cur = 1.0;
while (fabs(pre - cur) > 1e-6) {
    pre = cur;
    cur = 1.0 / 2 * (1.0/cur*x + cur);
}
return (int)cur;
```