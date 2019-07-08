#include "Stream.hpp"

Stream::Stream(int t, int w, int k, long l, unsigned int seed) : t(t), w(w), k(k), l(l) {
  srand(seed);
}

void Stream::push(const std::vector<int> &item) {
  queue.push(item);
}

std::vector<int> Stream::pop() {
  return queue.pop();
}

std::vector<int> Stream::get(unsigned long i) {
  auto item(queue.get(i));
  return item;
}

std::vector<std::vector<int>> Stream::getWindow() {
  auto size = std::min(w, (int) queue.size());
  std::vector<std::vector<int>> window((unsigned long) size);
  for (int i = 0; i < size; i++)
    window[(unsigned long) i] = i < k ? pop() : get((unsigned long) (i - k));
  return window;
}

std::vector<int> Stream::newTuple() {
  std::vector<int> tuple((unsigned long) t);
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
  for (int i = 0; i < l; i++)
    push(newTuple());
}

unsigned long Stream::size() {
  return queue.size();
}
