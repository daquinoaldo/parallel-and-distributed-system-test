#ifndef SKYLINE_QUEUE_HPP
#define SKYLINE_QUEUE_HPP

#include <mutex>
#include <condition_variable>
#include <deque>


template <typename T>
class Queue {

private:
  std::mutex mutex;
  std::condition_variable condition;
    std::deque<T> deque;

public:

  Queue() = default;

  void push(T const &item) {
    // unique lock will be released when leave this block
    {
        std::unique_lock<std::mutex> lock(mutex);
        deque.push_front(item);
    }
    condition.notify_one();
  }

  T pop() {
    std::unique_lock<std::mutex> lock(mutex);
    condition.wait(lock, [=] { return !deque.empty(); });
    T item(std::move(deque.back()));
    deque.pop_back();
    return item;
  }

  T get(int i) {
    std::unique_lock<std::mutex> lock(mutex);
    condition.wait(lock, [=] { return !deque.empty(); });
    T item(std::move(deque.at(i)));
    deque.pop_back();
    return item;
  }

};


#endif //SKYLINE_QUEUE_HPP
