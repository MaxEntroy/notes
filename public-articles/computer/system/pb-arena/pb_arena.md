### Why use arena allocation?

### Arena Class API

#### Constructors

#### Allocation Methods

#### "Owned list" Methods

#### Other Methods

#### Thread Safety

### Generated Message Class

#### Message Class Methods

```lua
message CatInfo {
  optional int32 cat1_id = 1;
  optional int32 cat2_id = 2;
};

message TagInfo {
  repeated int32 tag_ids = 1;
}

message Item {
  optional string id = 1;
  optional CatInfo cat_info = 2;
  optional TagInfo tag_info = 3;
  // ...
};
```


- ```Item(Item&& other)```:
    - If the source message is not on arena, shallow copy is done.
    - However, if the source message is on arena, it performs a deep copy of the underlying data.
    - In both cases the source message is left in a valid but unspecified state.
- ```Item& operator=(Item&& other)```:
    - **If both messages are not on arena or are on the same arena**, shallow copy is done.
    - However, if only one message is on arena, or the messages are on different arenas, it performs a deep copy of the underlying data.
    - n both cases the source message is left in a valid but unspecified state.
    - 这条规则比较重要，因为我们的服务中经常有pb的赋值，来回赋值的情形常见。
- ```void Swap(Item* other)```:
    - **If both messages to be swapped are not on arenas or are on the same arena**, shallow swap is done.
    - However, if only one message is on arena, or the messages are on different arenas, deep swap is done.
- ```void UnsafeArenaSwap(Item* other)```: Identical to Swap(), 
    - except it **assumes** both objects are on the same arena (or not on arenas at all) and **always** uses the efficient pointer-swapping implementation of this operation. 
    - Using this method can improve performance as, unlike Swap(), it doesn’t need to check which messages live on which arena before performing the swap. 
    - As the Unsafe prefix suggests, you should only use this method if you are sure the messages you want to swap aren’t on different arenas; otherwise this method could have unpredictable results.
    - 这个方法说一下，两点，
        - 第一，always shallow swap
        - 第二，自己确保对象的生命周期有效，否则，swap之后，原来的析构了，非法内存访问风险

#### Embedded Message Fields

- ```CatInfo* mutable_cat_info()```: Returns a mutable pointer to the submessage instance. If the parent object is on an arena then the returned object will be as well.
- ```void set_allocated_cat_info(CatInfo* bar)```: Takes a new object and adopts it as the new value for the field. Arena support adds additional copying semantics to maintain **proper ownership** when objects cross arena/arena or arena/heap boundaries:
    - If the parent object is on the heap and bar is on the heap, or if the parent and message are on the same arena, this method’s behavior is unchanged. 
    - If the parent is on an arena and bar is on the heap, the parent message adds bar to its arena’s ownership list with arena->Own().
    - If the parent is on an arena and bar is on a different arena, this method makes a copy of message and takes the copy as the new field value.
    - 总结下
        - 方式一：shallow copy，问题，会不会double free?
        - 方式二：shallow copy，同时接管对象的生命周期，我理解此时会double free
        - 方式三：deep copy，大家相安无事。
- ```Bar* release_cat_info()```: Returns the existing submessage instance of the field, if set, or a NULL pointer if not set, releasing ownership of this instance to the caller and clearing the parent message’s field. Arena support adds additional copying semantics to maintain the contract that the returned object is always heap-allocated:
    - If the parent message is on an arena, this method will make a copy of the submessage on the heap, clear the field value, and return the copy.
    - If the parent message is on the heap, the method behavior is unchanged.

#### String Fields

#### Repeated Fields

#### Repeated Numeric Fields

#### Repeated Embedded Message Fields

#### Repeated String Fields

### Using Patterns and Best Practices
