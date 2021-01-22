## Chapter07 Templates and Generic Programming

### Item47: Use Member function templates to accept all compatible types.

It is about type conversion.

- why: Smart pointers don't do well in supporting implicit conversions, because 
there is no inherent relationship among different instantiations of the same template.
We have to program them explicitly.

- best practise
  - subitem1: we need a ctor template.(Think about why the type params are not in the class template params)
  - subitem2: The generalized copy ctor is not declared explicit.
  - subitem3: We use the member initialization list to initialize SmartPtr<T>'s data member of type T* with the type U*
  held by the SmartPtr<U>.(Think about why)
