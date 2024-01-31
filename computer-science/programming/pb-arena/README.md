#### ArenaConstructable

```cpp
TEST(ArenaTest, ArenaConstructable) {
  EXPECT_TRUE(Arena::is_arena_constructable<TestAllTypes>::type::value);
  EXPECT_TRUE(Arena::is_arena_constructable<const TestAllTypes>::type::value);
  EXPECT_FALSE(Arena::is_arena_constructable<Arena>::type::value);
}
```

- is_arena_constructable<T>::value is true if the message type T has arena support enabled, and false otherwise.

#### BasicCreate

An arena provides two allocation interfaces: 
- CreateMessage<T>, which works for arena-enabled proto2 message types as well as other types that satisfy the appropriate protocol (described below), and Create<T>, which works for any arbitrary type T.
- CreateMessage<T> is better when the type T supports it, because this interface (i) passes the arena pointer to the created object so that its sub-objects and internal allocations can use the arena too, and (ii) elides the object's destructor call when possible. 
- Create<T> does not place any special requirements on the type T, and will invoke the object's destructor when the arena is destroyed.

如果是pb2-message，直接用CreateMessage.

- template<typename T> void Own(T* object): Adds object to the arena’s list of owned heap objects. 
- When the arena is destroyed, it traverses this list and frees each object using operator delete, i.e., the system memory allocator. 
- This method is useful in cases when an object’s lifetime should be tied to the arena but, for whatever reason, the object itself cannot be or was not already allocated on the arena.

Own的语义为接管动态对象的生命周期，raii object。最佳实践是，如果该动态对象的生命周期和arena一致，那么交给它接管。这里要小心double free.

```
TEST(ArenaTest, CreateAndMove) {
  Arena arena;
  std::string s("foo");
  const std::string* s_move = Arena::Create<std::string>(&arena, std::move(s));
  EXPECT_TRUE(s_move != nullptr);
  EXPECT_TRUE(s.empty());  // NOLINT
  EXPECT_EQ("foo", *s_move);
}
```

这个testcaes可以看出来Create用rvalue-reference构造时的动作，会将原来串置成empty. 当然，move semantics的要求是被窃取串也不能在用。说这个case主要是想看create的动作。