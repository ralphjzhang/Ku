#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

void print_timing(bool reader)
{
  using namespace std::chrono;
  static time_point<system_clock> write, read;
  if (reader) {
    read = system_clock::now();
    std::cout << "Write/Read pair takes: " << duration_cast<microseconds>(read - write).count()
      << " microseconds." << std::endl;
  } else {
    write = system_clock::now();
  }
}

int count;

struct Reader
{
  std::mutex& mutex;
  std::condition_variable& cond;
  uint64_t value;

  Reader(std::mutex& mutex, std::condition_variable& cond)
    : mutex(mutex), cond(cond), value(0) { }

  bool operator()()
  {
    while (true) {
      {
        std::unique_lock<std::mutex> lock(mutex);
        cond.wait(lock);
        value = count;
        cond.notify_one();
      }
      print_timing(true);
      if (value == 42) {
        std::cout << "Writer quits, exiting reader." << std::endl;
        break;
      }
      std::cout << "Count read: " << value << std::endl;
    }
    return true;
  }
};

struct Writer
{
  std::mutex& mutex;
  std::condition_variable& cond;
  uint64_t value;

  Writer(std::mutex& mutex, std::condition_variable& cond)
    : mutex(mutex), cond(cond), value(1) { }

  bool operator()()
  {
    while (true) {
      if (value < 10) {
        std::cout << "Writing count: " << value << std::endl;
        print_timing(false);
        {
          std::unique_lock<std::mutex> lock(mutex);
          count = value++;
          cond.notify_one();
          cond.wait(lock);
        }
      } else {
        std::unique_lock<std::mutex> lock(mutex);
        count = 42;
        cond.notify_one();
        break;
      }
    }
    return true;
  }
};

int main()
{
  std::mutex mutex;
  std::condition_variable cond;
  Reader reader(mutex, cond);
  Writer writer(mutex, cond);
  std::thread t1(std::ref(reader)), t2(std::ref(writer));
  t1.join();
  t2.join();
  std::cout << "Loop exited." << std::endl;
}

