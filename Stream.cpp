#include "Stream.hpp"

Stream::Stream(int t, int w, int k, long l, unsigned int seed) : t(t), w(w), k(k), l(l) {
  srand(seed);
}

void Stream::push(const Tuple &item) {
  queue.push(item);
}

Tuple Stream::pop() {
  return queue.pop();
}

Tuple Stream::get(unsigned long i) {
  auto item(queue.get(i));
  return item;
}

Window Stream::getWindow() {
  // last window can be smaller than w (window size)
  bool lastWindow = queue.size() <= (unsigned long) w;
  auto size = lastWindow ? queue.size() : (unsigned long) w;
  Window window(size);
  for (unsigned long i = 0; i < size; i++)
    // pop first k, get the others leaving in the queue
    // if size < w (last window) pop all the item, since there won't be another window
    window[i] = i < (unsigned long) k || lastWindow ? pop() : get(i - (unsigned long) k);
  return window;
}

Tuple Stream::newTuple() {
  Tuple tuple((unsigned long) t);
  for (int i = 0; i < t; i++) {
    auto r = rand(); // NOLINT(cert-msc30-c,cert-msc50-cpp)
#ifdef DEBUG
    r = r % 100;
#endif
    tuple[(unsigned long) i] = r;
  }
  return tuple;
}

void Stream::generateTuples() {
  for (long i = 0; i < l; i++)
    push(newTuple());
}

unsigned long Stream::size() {
  return queue.size();
}
