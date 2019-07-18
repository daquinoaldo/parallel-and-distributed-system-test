#ifndef SKYLINE_QUEUE_HPP
#define SKYLINE_QUEUE_HPP


#include <deque>

template <typename T>
class Queue {

private:
  std::deque<T> deque;

public:

  Queue() = default;

  void push(T const &item) {
    deque.push_front(std::move(item));
  }

  T pop() {
    T item(std::move(deque.back()));
    deque.pop_back();
    return item;
  }

  T get(unsigned long i) {
    T item(std::move(deque[size() - 1 - i]));
    return item;
  }

  unsigned long size() {
    return deque.size();
  }

};


#endif //SKYLINE_QUEUE_HPP