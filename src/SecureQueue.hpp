#ifndef SKYLINE_SECUREQUEUE_HPP
#define SKYLINE_SECUREQUEUE_HPP


#include <mutex>
#include <condition_variable>
#include "Queue.hpp"

template<typename T>
class SecureQueue {

private:
  std::mutex mutex;
  std::condition_variable condition;
  Queue<T> queue;
  bool EOQ = false;

public:

  SecureQueue() = default;

  void push(T const &item) {
    // unique lock will be released when leave this block
    {
      std::unique_lock<std::mutex> lock(mutex);
      queue.push(item);
    }
    condition.notify_one();
  }

  T pop() {
    std::unique_lock<std::mutex> lock(mutex);
    condition.wait(lock, [=] { return queue.size() > 0 || EOQ; });
    return queue.pop();
  }

  bool getEOQ() {
    // TODO: use another mutex and differentiate read/write lock
    std::unique_lock<std::mutex> lock(mutex);
    return EOQ;
  }

  void setEOQ() {
    // TODO: use another mutex
    std::unique_lock<std::mutex> lock(mutex);
    EOQ = true;
  }

  unsigned long size() {
    std::unique_lock<std::mutex> lock(mutex);
    return queue.size();
  }

};


#endif //SKYLINE_SECUREQUEUE_HPP