## 0x00 基本算法

### 0x04 二分

直接给出作者建议的二分实现

```cpp
int lower_bound(const std::vector<int>& nums, int low, int high, int x) {
  while (low < high) {
    int mid = low + (high - low) / 2;
    if (nums[mid] >= x) high = mid;
    else low = mid + 1;
  }
  return low;
}

lower_bound(nums, 0, nums.size() - 1, x);  // 正常情形
lower_bound(nums, 0, nums.size(), x);  // 非法情形
```

```cpp
int higher_bound(const std::vector<int>& nums, int low, int high, int x) {
  while (low < high) {
    int mid = low + (high - low) / 2 + 1;
    if (nums[mid] <= x) low = mid;
    else high = mid - 1;
  }
  return low;
}

higher_bound(nums, 0,);
```

这里我稍作改动，传入了下标，主要是方便控制处理越界的情形. 这个算法的核心点如下

- 假设合法下标区间为[low, high], 则二分区间为[low, high)
- 缩小范围有两种形式
  - r = mid;l = mid + 1; 这种形式, mid = (low + high) >> 1;
  - l = mid;r = mid - 1; 这种形式，mid = (low + high + 1) >> 1;
- 处理非法区间时
  - lower_bound拓展为[low, high]
  - upper_bound拓展为[low - 1, high)

这个算法的优点如下

- 二分结束的条件即low==high
- 结束点始终位于二分区间内
- 不会漏解，任何一种其余形式的二分都不能解决这个问题

使用时的注意点
- lower_bound/upper_bound的本质语义是寻找插入点
  - 对于查找不存在的元素，找到的是插入点，这个一定要判断
  - 如果越界，本质也是插入点，但此时无法直接获取数组元素判断
