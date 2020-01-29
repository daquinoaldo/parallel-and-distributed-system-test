#ifndef SKYLINE_QUEUE_HPP
#define SKYLINE_QUEUE_HPP


#include <deque>

template <typename T>
class Queue {

private:
  std::deque<T> deque;

public:

  Queue() = default;
  ~Queue() = default;

  void push(T const &item) {
    deque.push_front(item);
  }

  T pop() {
    if (deque.size() == 0)
      return T();
    T item(deque.back());
    deque.pop_back();
    return item;
  }

  T get(unsigned long i) {
    T item(deque[size() - 1 - i]);
    return item;
  }

  unsigned long size() {
    return deque.size();
  }

  bool empty() {
    return deque.size() == 0;
  }

};


#endif //SKYLINE_QUEUE_HPP