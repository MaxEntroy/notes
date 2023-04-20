#include <atomic>
#include <mutex>
#include <iostream>
#include <thread>

constexpr int kThres = 10;
std::atomic<int> counter{9};

namespace non_cas {

void producer() {
  if (counter < kThres)
    ++counter;
}

void test() {
  std::thread t1(producer);
  std::thread t2(producer);

  t1.join();
  t2.join();

  std::cout << "counter=" << counter << std::endl;
}

}  // namespace non-cas

//namespace cas {
//// make counter 9, right result
//// make counter 0, wrong result
//
//void producer() {
//  auto cur = counter.load(std::memory_order_relaxed);
//  if (cur < kThres)
//    counter.compare_exchange_strong(cur, cur + 1);
//}
//
//void test() {
//  std::thread t1(producer);
//  std::thread t2(producer);
//
//  t1.join();
//  t2.join();
//
//  std::cout << "counter=" << counter << std::endl;
//}
//
//}  // namespace cas
//
//namespace cas1 {
//// make counter 9, right result
//// make counter 0, right result
//
//void producer() {
//  auto cur = counter.load(std::memory_order_relaxed);
//  while (cur < kThres and not counter.compare_exchange_strong(cur, cur + 1));
//}
//
//void test() {
//  std::thread t1(producer);
//  std::thread t2(producer);
//
//  t1.join();
//  t2.join();
//
//  std::cout << "counter=" << counter << std::endl;
//}
//
//}  // namespace cas1
//
//namespace cas2 {
//// make counter 9, right result
//// make counter 0, right result
//
//void producer() {
//  auto cur = counter.load(std::memory_order_relaxed);
//  while (cur < kThres and not counter.compare_exchange_weak(cur, cur + 1));
//}
//
//void test() {
//  std::thread t1(producer);
//  std::thread t2(producer);
//
//  t1.join();
//  t2.join();
//
//  std::cout << "counter=" << counter << std::endl;
//}
//
//}  // namespace cas2



//namespace cas2 {
//
//std::atomic<bool> is_running{true};
//void close_and_clean_up() { std::cout << "close_and_clean_up is done." << std::endl; }
//
//void worker() {
//  auto cur_state = is_running.load(std::memory_order_relaxed);
//  if (cur_state == true) {
//    is_running.compare_exchange_strong(cur_state, false);
//    close_and_clean_up();
//  }
//}
//
//void test() {
//  std::thread t1(worker);
//  std::thread t2(worker);
//  std::thread t3(worker);
//
//  t1.join();
//  t2.join();
//  t3.join();
//}
//
////void worker11() {
////  auto cur_state = is_running.load(std::memory_order_relaxed);
////  if (is_running.compare_exchange_strong(cur_state, false)) close();
////}
////
////void worker2() {
////  auto cur_state = is_running.load(std::memory_order_relaxed);
////  while (cur_state == true and not is_running.compare_exchange_weak(cur_state, false));
////  if (cur_state == true) close();
////}
//
//}  // namespace cas2

int main(void) {
  non_cas::test();
  //cas::test();
  //cas1::test();
  //cas2::test();
  return 0;
}
