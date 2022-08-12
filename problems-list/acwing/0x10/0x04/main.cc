#include <assert.h>
#include <iostream>
#include <vector>

namespace bisearch {

int lower_bound(const std::vector<int>& nums, int low, int high, int x) {
  while (low < high) {
    int mid = low + (high - low) / 2;
    if (nums[mid] >= x) high = mid;
    else low = mid + 1;
  }
  return low;
}

int higher_bound(const std::vector<int>& nums, int low, int high, int x) {
  while (low < high) {
    int mid = low + (high - low) / 2 + 1;
    if (nums[mid] <= x) low = mid;
    else high = mid - 1;
  }
  return low;
}

void test_case1() {
  std::vector<int> nums{5, 7, 7, 8, 8, 10};
  int target = 7;
  int idx = lower_bound(nums, 0, nums.size() - 1, target);
  assert(idx == 1);
}

void test_case2() {
  std::vector<int> nums{5, 7, 7, 8, 8, 10};
  int target = 11;
  int idx = lower_bound(nums, 0, nums.size(), target);
  assert(idx == nums.size());
}

void test_case3() {
  std::vector<int> nums{5, 7, 7, 8, 8, 10};
  int target = 9;
  int idx = higher_bound(nums, 0, nums.size() - 1, target);
  assert(idx == 4);
}

void test_case4() {
  std::vector<int> nums{5, 7, 7, 8, 8, 10};
  int target = 4;
  int idx = higher_bound(nums, -1, nums.size() - 1, target);
  assert(idx == -1);
}

}

int main (void) {
  bisearch::test_case1();
  bisearch::test_case2();
  bisearch::test_case3();
  bisearch::test_case4();
  return 0;
}
