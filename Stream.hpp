#ifndef SKYLINE_STREAM_HPP
#define SKYLINE_STREAM_HPP


#include <vector>
#include "Queue.hpp"

class Stream {

private:
  int t, w, k;
  long l;
  Queue<std::vector<int>> queue;

  void push(const std::vector<int> &item);

  std::vector<int> pop();

  std::vector<int> newTuple();

public:
  Stream(int t, int w, int k, long l, unsigned int seed);

  std::vector<int> get(unsigned long i);

  std::vector<std::vector<int>> getWindow();
  void generateTuples();

  unsigned long size();
};


#endif //SKYLINE_STREAM_HPP
