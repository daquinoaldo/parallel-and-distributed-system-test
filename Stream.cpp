#include "Stream.hpp"

Stream::Stream(int windowSize, int windowShift, int tupleSize, int streamLength, int seed)
: w(windowSize), k(windowShift), t(tupleSize), l(streamLength) {
  // init rand for newTuple()
  srand (seed);
}

std::vector<int> Stream::newTuple() {
  std::vector<int> tuple(t);
  for (int i = 0; i < t; i++)
    tuple[i] = rand(); // NOLINT(cert-msc30-c,cert-msc50-cpp)
  return tuple;
}

void Stream::generateTuples() {
  for (int i = 0; i < l; i++)
    tuples->push(newTuple());
}

std::vector<std::vector<int>> Stream::getWindow() {
  std::vector<std::vector<int>> window(w);
  for (int i = 0; i < w; i++)
    // pop first k (get and delete) and get the others leaving in the Queue
    // TODO: handle last windows wrong size
    window[i] = i < k ? tuples->pop() : tuples->get(i - k);
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