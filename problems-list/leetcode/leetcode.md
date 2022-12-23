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

这里我补充一点方法论，以算竞的知识点为主。我们给出一个序列区间[low, high]，low/high均为数组合法下边的边界
- 方法一：这个也是我刚开始用的办法，即[low, high]作为循环区间
    - mid = low + (high - low)/2; low = mid - 1 or high = mid + 1
    - 如果越界，high < low(low = high + 1)，此时high + 1/low为越界下标
    - 优点：mid计算公式统一
    - 缺点：会漏解，比如mid处的解。
- 方法二：也是算竞赛的思路，**下标区间(合法区间)[low, high]，二分区间[low, high)**。注意，这里high是合法下标边界。
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
    int mid = low + (high - low) / 2;
    if (nums[mid] >= x) high = mid;
    else low = mid + 1;
  }
  return low;
}

int higher_bound(const std::vector<int>& nums, int low, int high, int x) {
  while (low < high) {
    int mid = low + (high - low) / 2 + 1;
    if (nums[mid] <= x) low = mid;
    else high = mid - 1;
  }
  return low;
}
```

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

#### [349.Intersection of Two Arrays](https://leetcode.com/problems/intersection-of-two-arrays/)

- 这个题思路非常多，一个启示就是，在进行查找的时候，我们可以用bi-search
- 另一个问题是需要解决重复的情况

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

#### [35.Search Insert Position](https://leetcode.com/problems/search-insert-position/)

- 两分查找衍生版本，比34简单一点
- 试了几个example发现非法区间low即为最终解
- lower_bound方法最简单

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

#### [702.Search in a Sorted Array of Unknown Size](https://grandyang.com/leetcode/702/)

- 这个题非会员能做，所以直接放了题解的链接。
- 思路非常巧妙，多了一步，即数组元素不定，从而high不能确定，无法开始二分查找。
    - 题解假设了数组的长度为INT_MAX，从而让二分符合条件。
        - 题目中说了每个元素不大于10000，从而放INT_MAX不会和已有元素冲突。
        - 下标越界，返回2147483647即INT_MAX。证明INT_MAX是非法下标，元素个数肯定不会这么多。
        - 所以，可以设定这个数组有INT_MAX个元素，多余的元素由INT_MAX填充，而不影响查询结果。
    - 注意两分的使用条件，元素可以重复，只不过此时值不唯一。

#### [33.Search in Rotated Sorted Array](https://leetcode.com/problems/search-in-rotated-sorted-array/)

- 这个题和702一样，也是非常好的拓展题，在基础上进行思路的拓展。
- 这个题，不能直接套，要套思路。
- 方法一：先两分找pivot，pivot语义代表左边区间的最后一个元素。然后再分别找，当然这里分治可以优化为减治。减治的边界条件容易找错。
- 方法二：grandyang的思路是这样，这个数组，通过mid进行划分后，势必一半是有序，一半是非有序。对于有序区间，我们直接利用二分。注意，这个思路比较不同，需要好好理解。
    - 其核心思路是，既然整个数组不是有序，我们需要先找到一个有序的区间，进行二分。当然，target不一定在这里面，所以对于有序区间还需要使用target判断一下。

#### [81.Search in Rotated Sorted Array II](https://leetcode.com/problems/search-in-rotated-sorted-array-ii/)

- 我在找pivot时，用了遍历算法，因为没法二分找会跳过pivot
- grandyang的做法在33的基础上，巧妙的发现，如果nums[mid] < nums[high]时，右边区间有序，这个条件不完备。nums[mid] == nums[high]时，也有可能有序
- 所以，在这个基础上，还是去寻找nums[mid] < nums[high]的情形。

#### [153.Find Minimum in Rotated Sorted Array](https://leetcode.com/problems/find-minimum-in-rotated-sorted-array/)

- 33题的简化题，找pivot，保持思路一致即可。
- 方法一：我说下我的办法，我还是找了一个target，即nums[0]，用它找到一个两个相邻点，一个大于它，一个小于它，此时insertion point没有问题(不管使用哪种两分查找)。小心high + 1越界的问题
- 方法二：但是，使用grandyang的方法，即不固定target，每次比较nums[mid] and nums[right]，此时不行。只能用开区间的写法，这个我有点不太理解，暂时先记住。

#### [154.Find Minimum in Rotated Sorted Array II](https://leetcode.com/problems/find-minimum-in-rotated-sorted-array-ii/)

- 这个题我最终还是做出来了，用了一些trick的办法，但是先ac就行。
- 方法一：我用的是我自己的办法，比较好理解的办法寻找pivot，但是最后边界的处理一直有问题，所以我用了trick的办法来求解。
    - 我的办法之所以没法解决trick的问题，我认为在于闭区间的使用，代码里我显示的处理了low==mid and high==mid的情形，证明闭区间的多一次判断，会造成最后解的位置偏移。
    - 这个题最简单，最快的办法，也是用开区间解题。
- 方法二：grandyang的办法，开区间。不设定target，通过中点和右侧相比来找。中点不能和左侧相比，因为此时不管大小，最小值都有可能在左侧。

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
    dfs(n, i + 1);

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

#### [77.Combinations](https://leetcode.com/problems/combinations/)

- 基础题，利用acwing的模板求解即可。
- 方法二，利用公式。但是公式这里注意，公式只说了个数是一样的，但是这个题求的是排列集合这个元素，所以不一样。这个题的求解逻辑需要单独推到。详见[77. Combinations](https://grandyang.com/leetcode/77/)

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

```cpp
// 正确的实现
if (start == s.size() and cnt == 4) {
    auto tmp = chosen;
    tmp.pop_back();
    ret.push_back(tmp);
    return;
}
// 错误的实现
if (start == s.size() and cnt == 4) {
    chosen.pop_back();
    ret.push_back(tmp);
    return;
    // 这里返回后，chosen会退到dfs的回溯那里，会紧接着回溯掉#和上次加入的数字，由于#已经在这里回溯掉了，所以就会多回溯一个元素，回溯出错。
    // 当时之所以没考虑到是因为，以为chosen是正确答案，加入ret后，这个chosen不参与回溯。
    // 其实不是，某一个解回溯后，还有可能形成另一个解。所以，chosen不能有step in/backtracking额外的任何操作。
}
```

#### [78.Subsets](https://leetcode.com/problems/subsets/)

- 有了划分的基础，子集问题就显得非常容易了，基于排列做，没有剪枝。
- 随想录有一个好的总结是，之前两题都需要遍历到叶子才形成一个合法的path，但是这个题不需要。非叶子只要合法，就放进来。

#### [90.Subsets II](https://leetcode.com/problems/subsets-ii/)

- 生成集合的元素存在副本，如果按照之前的办法，会产生重复的集合(组合)。
- 和之前排列，以及组合的处理保持一致。如果相邻元素相等 and 不在同一个层次，此时剪枝

#### [491.Increasing Subsequences](https://leetcode.com/problems/increasing-subsequences/)

- 这个题整体思路比较清楚，但是剪枝的实现一致没实现对。
- 剪枝
    - 递增
    - 同一个子树，本层重复节点，不能反复试探。这个逻辑我一直没实现对。注意，不能排序。start的本质是下标递增，不是元素递增。
- 优化，我在进行去重的时候，是一个O(N)的做法，随想录引入了hash数组，空间换时间，从而有O(1)的时间复杂度。

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

#### [51.N-Queens](https://leetcode.com/problems/n-queens/)

- 在说这个题之前，还是说一下332这个题，这个题是一个承上启下的题目。关键是教会了我怎么样生成搜索树，即状态之间的联系怎么生成。当然carl的模板也很重要，强调了这个过程就是在第二部枚举备选集合。
- 有了332的基础，这个题感觉轻松多了。
- 说一下细节
    - vis数组，我最初的实现是一个二维的，但其实一维就够。index代表row, vis[index]代表col。不过，其实没有也ok，因为有chosen数组，可以还原vis
    - 对角线的判断，采用了以前的办法。
    - 如果使用chosen数组，其实level不必要。如果是acm直接开数组，则需要level

#### [52.N-Queens II](https://leetcode.com/problems/n-queens-ii/)

- 这个题在51的基础上，不需要chosen and ret.
- 但是必须有vis

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

#### [70.Climbing Stairs](https://leetcode.com/problems/climbing-stairs/)

- 思路
    - 朴素的思路：枚举所有的情形，关键就是怎么枚举？我的思路还是，从子问题和原问题的关系入手，发现有关系，直接找到了原问题和子问题的关系。直接递归求解即可，注意，此时我并没有直接反映到用dp做，但是这么做会超时。
    - 正确的思路：沿着朴素的思路，发现超时，即子问题存在重叠性。没有一开始就考虑dp是因为不是最优化问题，很明显是计数问题即排列组合。我是通过重叠子问题，发现了dp的特性。用dp求解即可。
    - 方法：和fib数的方法一模一样。

#### [509.Fibonacci Number](https://leetcode.com/problems/fibonacci-number/)

- 同climbling stairs

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

$x_{n+1} = x_{n} - \frac{f(x_{n})}{f^{`}(x_{n})}$

#### [69.Sqrt(x)](https://leetcode.cn/problems/sqrtx/)
- 和上一题思路一致。
- 也可以使用牛顿法。
