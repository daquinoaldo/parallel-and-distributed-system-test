#include "Stream.hpp"

Stream::Stream(unsigned _t, unsigned _w, unsigned _k, unsigned long _l, unsigned seed) : t(_t), w(_w), k(_k), l(_l) {
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

std::pair<int, Window> Stream::getWindow() {
  // last window can be smaller than w (window size)
  bool lastWindow = queue.size() <= (unsigned long) w;
  auto size = lastWindow ? queue.size() : (unsigned long) w;
  Window window(size);
  for (unsigned long i = 0; i < size; i++)
    // pop first k, get the others leaving in the queue
    // if size < w (last window) pop all the item, since there won't be another window
    window[i] = i < (unsigned long) k || lastWindow ? pop() : get(i - (unsigned long) k);
  return std::pair(index++, window);
}

unsigned long Stream::size() {
  return queue.size();
}

int Stream::getT() {
  return t;
}

int Stream::getW() {
  return w;
}

long Stream::getL() {
  return l;
}