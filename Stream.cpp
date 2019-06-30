#include "Stream.hpp"

Stream::Stream(unsigned int windowSize, unsigned int windowShift, unsigned int tupleSize, unsigned int streamLength,
               unsigned int seed) : w((int) windowSize), k((int) windowShift), t((int) tupleSize),
                                    l((int) streamLength) {
  // init rand for newTuple()
  srand(seed);
}

std::vector<int> Stream::newTuple() {
  std::vector<int> tuple((unsigned long) t);
  for (int i = 0; i < t; i++) {
    auto r = rand();   // NOLINT(cert-msc30-c,cert-msc50-cpp)
#ifdef DEBUG
    r = r % 100;
#endif
    tuple[(unsigned long) i] = r;
  }
  return tuple;
}

void Stream::generateTuples() {
  for (int i = 0; i < l; i++)
    tuples->push(newTuple());
}

std::vector<std::vector<int>> Stream::getWindow() {
  std::vector<std::vector<int>> window((unsigned long) w);
  for (int i = 0; i < w; i++)
    // pop first k (get and delete) and get the others leaving in the Queue
    // TODO: handle last windows wrong size
    // TODO: lock not in each get but in the entire window
    window[(unsigned long) i] = i < k ? tuples->pop() : tuples->get((unsigned long) (i - k));
  return window;
}

std::string Stream::serializeTuple(std::vector<int> tuple) {
  std::string string = "<";
  for (unsigned long i = 0; i < tuple.size(); i++)
    string += std::to_string(tuple[i]) + (i != tuple.size() - 1 ? "," : ">");
  return string;
}

std::string Stream::serializeWindow(std::vector<std::vector<int>> window) {
  std::string string = "[";
  for (unsigned long i = 0; i < window.size(); i++)
    string += serializeTuple(window[i]) + (i != window.size() - 1 ? ", " : "]");
  return string;
}