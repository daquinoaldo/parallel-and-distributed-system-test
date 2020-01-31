#ifndef SKYLINE_UTILS_HPP
#define SKYLINE_UTILS_HPP


#include <string>
#include <vector>
#include "types.hpp"

class Utils {
private:
  static short compareTuple(Tuple* ti, Tuple* tj);

public:
  static std::string serializeTuple(Tuple* tuple);
  static std::string serializeWindow(Window* window);
  static Skyline* processWindow(Window* window);
};


#endif //SKYLINE_UTILS_HPP
