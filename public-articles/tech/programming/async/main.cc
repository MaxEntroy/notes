#include <chrono>
#include <future>
#include <iostream>
#include <latch>
#include <string>
#include <thread>

std::string fetchDataFromDB(const std::string& recv_data) {
  std::this_thread::sleep_for(std::chrono::seconds(5));
  return "DB " + recv_data;
}

std::string fetchDataFromFile(const std::string& recv_data) {
  std::this_thread::sleep_for(std::chrono::seconds(5));
  return "File " + recv_data;
}

void SerialFetchData() {
  const std::string data{"hello,world"};
  auto start = std::chrono::system_clock::now();
  auto db_data = fetchDataFromDB(data);
  auto file_data = fetchDataFromFile(data);
  auto stop = std::chrono::system_clock::now();

  std::cout << std::chrono::duration_cast<std::chrono::seconds>(stop - start).count() << " seconds elapsed." << std::endl;
}


void ParallelFetchDataInThreadManner() {
  const std::string data{"hello,world"};
  auto start = std::chrono::system_clock::now();

  std::string db_data;
  std::thread t([&data, &db_data] () noexcept {
    db_data = fetchDataFromDB(data);
  });

  auto file_data = fetchDataFromFile(data);

  t.join();

  auto stop = std::chrono::system_clock::now();
  std::cout << std::chrono::duration_cast<std::chrono::seconds>(stop - start).count() << " seconds elapsed." << std::endl;
}

void ParallelFetchDataInAsyncManner() {
  const std::string data{"hello,world"};
  auto start = std::chrono::system_clock::now();

  auto resultFromDB = std::async(std::launch::async, fetchDataFromDB, data);

  auto file_data = fetchDataFromFile(data);

  auto db_data = resultFromDB.get();

  auto stop = std::chrono::system_clock::now();
  std::cout << std::chrono::duration_cast<std::chrono::seconds>(stop - start).count() << " seconds elapsed." << std::endl;
  std::cout << "db data:" << db_data << ", file data:" << file_data << std::endl;
}

int main(void) {
  //SerialFetchData();
  ParallelFetchDataInAsyncManner();
  return 0;
}
