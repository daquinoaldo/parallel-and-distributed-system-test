#ifndef SKYLINE_UTILS_HPP
#define SKYLINE_UTILS_HPP


#include <string>
#include <vector>
#include "Stream.hpp"

class Utils {
private:
  static std::string serializeTuple(std::vector<int> tuple);

public:
  static std::string serializeWindow(std::vector<std::vector<int>> window);

  static std::string serializeStream(Stream *stream);
};


#endif //SKYLINE_UTILS_HPP
