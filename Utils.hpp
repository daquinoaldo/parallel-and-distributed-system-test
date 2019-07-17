#ifndef SKYLINE_UTILS_HPP
#define SKYLINE_UTILS_HPP


#include <string>
#include "Stream.hpp"

class Utils {
private:
  static std::string serializeTuple(Tuple tuple);

  /**
   * Compare two tuples to determine if one will delete the other.
   * Check if each component of the first tuple is less or greater than the corresponding component of the other tuple.
   * @param ti The first tuple.
   * @param tj The second tuple.
   * @return -1 if the first deletes the second (each component strictly less than);
   *          1 if the second deletes the first (each component strictly greater than);
   *          0 if both survives (some components less and other greater than)
   */
  static short compareTuple(const Tuple &ti, const Tuple &tj);

public:
  static std::string serializeWindow(Window window);
  static std::string serializeStream(Stream *stream);

  /**
   * Reduce a window to a skyline.
   * @param window The window.
   * @return The skyline of the window.
   */
  static std::vector<std::vector<int>> processWindow(Window window);
};


#endif //SKYLINE_UTILS_HPP
