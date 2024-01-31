## Introduction

下午说shouyang讨论了缓存的用法，主要是对于缓存item到底使用哪一种smart pointer的问题。

## weak_ptr for caching

起因在于下面的这段代码

```cpp
shared_ptr<const Widget> fastLoadWidget(WidgetId id) {
  static std::unordered_map<WidgetId, 
                            std::weak_ptr<const Widget>> cache;

  auto objPtr = cache[id].lock();
  if (!objPtr) {
    objPtr = loadWidget(id);
    cache[id] = objPtr
  }
  return objPtr
}
```

看完这段代码后，我得到了一些错误的认知。
- 首先，认为性能有收益。
  - 原始的代码需要查询，如果没有，再load.
  - 现在代码只有load，性能好。
  - 这个理解错误，因为```cache[id]```这里也进行了查询，同时还进行了lock，性能不会更好。
- 其次，认为这是通用写法。
  - 这也是错误认知，因为混淆了一个重要的点
    - That’s because std::weak_ptr isn’t a standalone smart pointer. 
    - It’s an augmentation of std::shared_ptr.
  - ```weak_ptr```并不能单独使用，依附于```shared_ptr```使用。
  - 所以，它其实不能hold资源。

上面这段代码的问题在于，cache其实没有hold资源。有别于基于```shared_ptr```实现的cache.

那么，问题来了？既然cache不hold资源，那谁hold资源呢？
- 答案就是caller
- 如果caller1来了，新创建了```shared_ptr```，同时没有执行结束。
- 此时caller2来了，从cache里可以拿到资源，因为caller1还没有释放。
- 之后caller1先结束，自减引用计数。没问题，此时caller2 hold.
- 但下一个时刻，caller2结束，然后caller3来了，又要重新创建。
- 本质是cache 没有hold任何资源。

## Conclusion

- 基于```shared_ptr```实现的cache，是通用的cache，cache自己hold资源。
- 基于```weak_ptr```实现的cache，不是通用的cache，不hold资源，caller hold资源。