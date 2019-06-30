#ifndef SKYLINE_STREAM_HPP
#define SKYLINE_STREAM_HPP


#include <cstdlib>
#include <vector>
#include <string>
#include "Queue.hpp"

class Stream {

public:
  explicit Stream(unsigned int windowSize = 10, unsigned int windowShift = 2, unsigned int tupleSize = 2,
                  unsigned int streamLength = 1000, unsigned int seed = 42);
  void generateTuples();
  std::vector<std::vector<int>> getWindow();
  static std::string serializeTuple(std::vector<int> tuple);
  static std::string serializeWindow(std::vector<std::vector<int>> window);

private:
  int w;
  int k;
  int t;
  int l;
  Queue<std::vector<int>>* tuples = new Queue<std::vector<int>>();
  std::vector<int> newTuple();

};


#endif //SKYLINE_STREAM_HPP
