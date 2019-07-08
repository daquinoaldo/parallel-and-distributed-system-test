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
  Queue<T> *queue = new Queue<T>;

public:

  SecureQueue() = default;

  void push(T const &item) {
    // unique lock will be released when leave this block
    {
      std::unique_lock<std::mutex> lock(mutex);
      queue->push(item);
    }
    condition.notify_one();
  }

  T pop() {
    std::unique_lock<std::mutex> lock(mutex);
    condition.wait(lock, [=] { return queue->size() > 0; });
    return queue->pop();
  }

  T get(unsigned long i) {
    std::unique_lock<std::mutex> lock(mutex);
    condition.wait(lock, [=] { return queue->size() > i; });
    return queue->get(i);
  }

  unsigned long size() {
    std::unique_lock<std::mutex> lock(mutex);
    return queue->size();
  }

};


#endif //SKYLINE_SECUREQUEUE_HPP