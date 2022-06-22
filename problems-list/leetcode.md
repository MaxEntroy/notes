## 搜索

### [704.Binary Search](https://leetcode.com/problems/binary-search/)

- 两分查找的基本实现
- 使用前提，有序，不重复，缺一不可
- 递归实现，注意两个边界条件。非递归实现也需要掌握

### [34.Find First and Last Position of Element in Sorted Array](https://leetcode.com/problems/find-first-and-last-position-of-element-in-sorted-array/)

- 两分查找的衍生版本，但是更深刻
- 关键在于边界位置查询的实现，区分左右试探
- 边界查询的终止条件是找到非法区间，则上一次为边界

### [35.Search Insert Position](https://leetcode.com/problems/search-insert-position/)

- 两分查找衍生版本，比34简单一点
- 试了几个example发现非法区间low即为最终解