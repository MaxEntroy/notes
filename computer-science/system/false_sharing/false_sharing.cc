#include <atomic>
#include <iostream>
#include <mutex>
#include <numeric>
#include <thread>

#include "benchmark/benchmark.h"

static constexpr int kIterNum{1 << 20};
static constexpr int kThreadsNum{1 << 3};

namespace mutex_version {

void Incr() {
  int counter = 0;
  std::mutex mtx;

  std::thread threads[kThreadsNum];
  for (auto& thread : threads) {
    thread = std::thread([&counter, &mtx]() noexcept {
      for (int i = 0; i < kIterNum; ++i) {
        std::scoped_lock _(mtx);
        ++counter;
      }
    });
  }

  for (auto& thread : threads) {
    thread.join();
  }
  //std::cout << "mutex version, counter = " << counter << std::endl;
}

}  // namespace mutex_version

namespace atomic_version {

void Incr() {
  std::atomic<int> counter{0};
  std::thread threads[kThreadsNum];
  for (auto& thread : threads) {
    thread = std::thread([&counter]() noexcept {
      for (int i = 0; i < kIterNum; ++i) {
        counter.fetch_add(1, std::memory_order_relaxed);
      }
    });
  }

  for (auto& thread : threads) {
    thread.join();
  }
  //std::cout << "atomic version, counter = " << counter << std::endl;
}

}  // namespace atomic_version

namespace local_copy_with_false_sharing {

void Incr() {
  std::vector<int> sub_counters(kThreadsNum, 0);
  std::thread threads[kThreadsNum];
  for (int i = 0; i < kThreadsNum; ++i) {
    threads[i] = std::thread([i, &sub_counters]() noexcept {
      for (int k = 0; k < kIterNum; ++k) {
        ++sub_counters[i];
      }
    });
  }
  for (auto& thread : threads) {
    thread.join();
  }
  int sum = std::accumulate(sub_counters.begin(), sub_counters.end(), 0);
  //std::cout << "false sharing, counter = " << sum << std::endl;
}

}  // namespace local_copy_with_false_sharing

namespace local_copy_without_false_sharing {

void Incr() {
  struct alignas(64) AlignedInt {
    AlignedInt(int val_in) : val(val_in){};
    AlignedInt& operator++() {
      ++val;
      return *this;
    }
    int val = 0;
  };
  std::vector<AlignedInt> sub_counters(kThreadsNum, 0);
  std::thread threads[kThreadsNum];
  for (int i = 0; i < kThreadsNum; ++i) {
    threads[i] = std::thread([i, &sub_counters]() noexcept {
      for (int k = 0; k < kIterNum; ++k) {
        ++sub_counters[i];
      }
    });
  }
  for (auto& thread : threads) {
    thread.join();
  }
  int sum = std::accumulate(sub_counters.begin(), sub_counters.end(), 0,
                            [](const AlignedInt& a, const AlignedInt& b) { return a.val + b.val; });
  //std::cout << "without false sharing, counter = " << sum << std::endl;
}

}  // namespace local_copy_without_false_sharing

// Register the function as a benchmark
static void BM_MutexIncr(benchmark::State& state) {
  for (auto _ : state) {
    mutex_version::Incr();
  }
}

static void BM_AtomicIncr(benchmark::State& state) {
  for (auto _ : state) {
    atomic_version::Incr();
  }
}

static void BM_LocalCopyWithFalseSharingIncr(benchmark::State& state) {
  for (auto _ : state) {
    local_copy_with_false_sharing::Incr();
  }
}

static void BM_LocalCopyWithoutFalseSharingIncr(benchmark::State& state) {
  for (auto _ : state) {
    local_copy_without_false_sharing::Incr();
  }
}

// Register the function as a benchmark
BENCHMARK(BM_MutexIncr);
BENCHMARK(BM_AtomicIncr);
BENCHMARK(BM_LocalCopyWithFalseSharingIncr);
BENCHMARK(BM_LocalCopyWithoutFalseSharingIncr);

// Run the benchmark
BENCHMARK_MAIN();
