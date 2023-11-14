#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace dyn_poly {

class Filter {
 public:
  virtual ~Filter() = default;

  virtual std::string Name() const = 0;
};

class CatFilter : public Filter {
 public:
  std::string Name() const override { return "CatFilter"; }
};

class TagFilter : public Filter {
 public:
  std::string Name() const override { return "TagFilter"; };
};

void Test() {
  std::vector<std::unique_ptr<Filter>> filters;
  filters.emplace_back(std::make_unique<CatFilter>());
  filters.emplace_back(std::make_unique<TagFilter>());

  for (const auto& filter : filters) {
    std::cout << filter->Name() << std::endl;
  }
}

}  // namespace dyn_poly

namespace static_poly {

template<typename T>
class Filter {
 public:
  std::string Name() {
    return static_cast<T*>(this)->GetName();
  }
};

class CatFilter : public Filter<CatFilter> {
 public:
   std::string GetName() const { return "CatFilter"; }
};

class TagFilter : public Filter<TagFilter> {
 public:
   std::string GetName() const { return "TagFilter"; }
};

template<typename T>
void PolyCall(T& filter) {
  std::cout << filter.Name() << std::endl;
}

void Test() {
  CatFilter cat_filter;
  TagFilter tag_filter;

  PolyCall(cat_filter);
  PolyCall(tag_filter);
}

}  // namespace static_poly

int main(void) {
  dyn_poly::Test();
  static_poly::Test();
  return 0;
}