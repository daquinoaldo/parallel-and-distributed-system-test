#ifndef SKYLINE_STREAM_HPP
#define SKYLINE_STREAM_HPP


#include "Queue.hpp"
#include "types.hpp"

class Stream {

private:
  int t, w, k;
  long l;
  Queue<Tuple> queue;

  void push(const Tuple &item);

  Tuple pop();

  Tuple newTuple();

public:
  Stream(int t, int w, int k, long l, unsigned int seed);

  Tuple get(unsigned long i);

  Window getWindow();
  void generateTuples();

  unsigned long size();
};


#endif //SKYLINE_STREAM_HPP
