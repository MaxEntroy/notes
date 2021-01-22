## Chapter07 Templates and Generic Programming

### Item47: Use Member function templates to accept all compatible types.

It is about type conversion.

- why: Smart pointers don't do well in supporting implicit conversions, because 
there is no inherent relationship among different instantiations of the same template.
We have to program them explicitly.

- best practise
  - subitem1: we need a ctor template.
  - subitem2: The generalized copy ctor is not declared explicit.
  - subitem3: We use the member initialization list to initialize SmartPtr<T>'s data member of type T* with the type U*
  held by the SmartPtr<U>
  - subitem4: Another common role for them is in support for assignment.
  - subitem5: If you declare member templates for generalized copy construction or generalized assignment, you'll
still need to declare the normal copy constructor and copy assignment operator, too.(Member function templates are wonderful things, but they don't alter the basic rules of the language.)

```cpp
template<typename T>
class SmartPtr {
 public:
  template<typename U>
  SmartPtr(const SmartPtr<U>& other) // initialize this held ptr
      : heldPtr(other.get()) { ... } // with other's held ptr
  T* get() const { return heldPtr; }
...
 private: // built-in pointer held
  T *heldPtr; // by the SmartPtr
};
```