#ifndef SKYLINE_UTILS_HPP
#define SKYLINE_UTILS_HPP


#include <string>
#include "types.hpp"

class Utils {
private:
  static short compareTuple(const Tuple &ti, const Tuple &tj);

public:
  static Tuple newTuple(int t);

  static std::string serializeTuple(Tuple tuple);
  static std::string serializeWindow(Window window);

  static std::vector<std::vector<int>> processWindow(Window window);
};


#endif //SKYLINE_UTILS_HPP
