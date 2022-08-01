## 基础算法

### 模拟

这一类题目的思路一般比较朴素，题目中一般会给出，直接用代码模拟实现即可。

#### [13.Roman to Integer](https://leetcode.com/problems/roman-to-integer/)

- 基本思路照着实现即可。
- 不过这个题看了题解，有一个好的办法是，进一步泛华了题目给的思路，即发现如果当前位置比后面小，直接减去即可。

#### [12.Integer to Roman](https://leetcode.com/problems/integer-to-roman/)

- 这个题，我觉得做题过程还行，确实是想清楚了再写，保持。
- 我的思路来自于radix的转换，按照这个不断的处理即可。

#### [31.Next Permutation](https://leetcode.com/problems/next-permutation/) 

- 这个题，直接看了题解。如下
- step1: 从后向前寻找一个逆序
- step2: 从后向前寻找第一个大于当前数的元素
- step3: swap and reverse the rest elements.
- 如果不存在逆序，直接reverse.

### 二分查找

#### [704.Binary Search](https://leetcode.com/problems/binary-search/)

- 两分查找的基本实现
- 前提数组有序
- 递归实现，注意两个边界条件。非递归实现也需要掌握

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

#### [307.Range Sum Query - Mutable](https://leetcode.com/problems/range-sum-query-mutable/)

- 这个题和303一脉相承，但是超时，表明每次O(N)的更新时间复杂度无法接受。
- 最后用了一个非常巧妙地思路，作者对于更新采用了O(1)的复杂度，但是sum的时候，将时间复杂度降到了O(sqrt(n))，这也是非常显著的提升。
- 作者思路的来源也在于预处理的巧妙，利用了hash分桶的思路，缓存了每个桶的sum。
    - 更新的时候，肯定只更新某一个桶，显著的降低了更新的复杂度。如果是dp的思路，更新的复杂度在O(N).
    - 查询的时候，中间区间桶的sum可以直接累加，这里的时间复杂度从O(N)降低到了O(sqrt(N))，首位的两个桶需要单独sum

### Hash

#### [36.Valid Sudoku](https://leetcode.com/problems/valid-sudoku/)

- 存储对应的数独有效位状态即可。
- grandyang的方法压缩了状态，对于每一个位置，进行编码存储。从而节省了空间开销
    - 编码技巧需要注意，下标是数组，首先下标就需要做区分，不能放到同一个位置，否则row/col无法区分。
    - 其次，字符和下标需要区分，因为都是数字，不区分无法区别。比如把数字9放在第9行和第9列，如果不区分数字和下标，都是99，看不出来。

## 搜索

### DFS

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

#### [40.Combination Sum II](https://leetcode.com/problems/combination-sum-ii/)

- 这个题对应47题，重复元素的组合生成。上面是重复元素的排列生成。
- 方式一样，避免相同元素在同一个层次的试探。
- 注意，vis的用法和排列不同，前者两种剪枝用到了这个结构，但是组合只用到了一种。使用过的元素不会重复使用，这个在排列中用startIndex保证。

#### [216.Combination Sum III](https://leetcode.com/problems/combination-sum-iii/)

- 这个题和前两个兄弟题的区别是，前两个题也是产生组合，但是对于组合的长度没有限制。这个题给了限制。
- 此时，可以用acwing的模板。
- 注意，由于组合的长度有限制，注意宽度的剪枝。

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

#### [131.Palindrome Partitioning](https://leetcode.com/problems/palindrome-partitioning/)

- 这个题第一次没做出来，主要是没有找到解题钥匙，即没有转化问题。随想录的思考过程非常好
- 切割问题，可以抽象为组合问题。但不完全一样，因为组合只要递归到最后的层次，肯定是解，但切割不是。
    - 上面说的组合，也只是说，起点的下界需要递增，这个是切割的特性，切割只能向后切，不能向前切
    - 类似组合的原因在于，切割点，可以类似从组合中选一个元素出来。
    - 第k刀在第k-1刀的基础上进行枚举所有可能的切割点。这个是核心思路。
    - 同时，第k刀的枚举点是否合理，取决于这个串是否为回文。如果不是，直接剪枝。尝试第k刀的下一个切割点。
- 如何模拟切割线。这个转化为切割起点即可。
- 切割问题如何终止。知道存在非法的切割起点，即切割起点枚举到了序列终点。即起点的下界越界了，这个是第一次使用起点下界的条件作为递归边界。

#### [93.Restore IP Addresses](https://leetcode.com/problems/restore-ip-addresses/)

- 这个题在131的基础上就容易理解多了。前者求解划分，划分需要是回文，本题一样，划分需要满足ip地址的需求。
- 边界条件有不同，本题划分只要求是4个。
- 子串转数字，随想录的解法提供了更好的办法，就地转。不要用stringstream
- 有一个特别需要注意的点，回溯的时候，对于chosen集合，不要给出额外的修改，否则会影响回溯。

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

## 动态规划

#### [118. Pascal's Triangle](https://leetcode.com/problems/pascals-triangle/)

- 基础题，非常直接的递推结构。

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