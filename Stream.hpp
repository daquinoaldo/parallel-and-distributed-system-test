#ifndef SKYLINE_STREAM_HPP
#define SKYLINE_STREAM_HPP

#include <cstdlib>
#include <vector>
#include <string>
#include "Queue.hpp"


class Stream {

public:
  explicit Stream(int w = 4, int k = 1, int t = 2, int l = 100, int seed = 0);
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
