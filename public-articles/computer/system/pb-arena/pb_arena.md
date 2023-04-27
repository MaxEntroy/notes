### Why use arena allocation?

### Arena Class API

#### Constructors

#### Allocation Methods

#### "Owned list" Methods

#### Other Methods

#### Thread Safety

### Generated Message Class

#### Message Class Methods


- ```RawSample(RawSample&& other)```:
    - If the source message is not on arena, shallow copy is done.
    - However, if the source message is on arena, it performs a deep copy of the underlying data.
    - In both cases the source message is left in a valid but unspecified state.
- ```RawSample& operator=(Message&& other)```:
    - **If both messages are not on arena or are on the same arena**, shallow copy is done.
    - However, if only one message is on arena, or the messages are on different arenas, it performs a deep copy of the underlying data.
    - n both cases the source message is left in a valid but unspecified state.
    - 这条规则比较重要，因为我们的服务中经常有pb的赋值，来回赋值的情形常见。
- ```void Swap(Message* other)```:
    - **If both messages to be swapped are not on arenas or are on the same arena**, shallow swap is done.
    - However, if only one message is on arena, or the messages are on different arenas, deep swap is done.



#### Embedded Message Fields

#### String Fields

#### Repeated Fields

#### Repeated Numeric Fields

#### Repeated Embedded Message Fields

#### Repeated String Fields

### Using Patterns and Best Practices
