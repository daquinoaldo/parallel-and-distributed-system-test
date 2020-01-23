#ifndef SKYLINE_STREAM_HPP
#define SKYLINE_STREAM_HPP


#include <cstdlib>
#include "types.hpp"
#include "Queue.hpp"

class Stream {

private:
  int t, w, k;
  long l;
  long index = 0;
  Queue<Tuple> queue;

  Tuple pop();

  Tuple get(unsigned long i);

public:
  Stream(unsigned t, unsigned w, unsigned k, unsigned long l, unsigned seed);

  virtual void push(const Tuple &item);

  virtual std::pair<int, Window> getWindow();

  int getT();

  int getW();

  long getL();

  virtual unsigned long size();
};


#endif //SKYLINE_STREAM_HPP
