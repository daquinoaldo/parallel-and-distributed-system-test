#ifndef SKYLINE_SKYLINE_HPP
#define SKYLINE_SKYLINE_HPP


#include <vector>

class Skyline {

public:
  /**
   * Reduce a window to a skyline.
   * @param window The window.
   * @return The skyline of the window.
   */
  static std::vector<std::vector<int>> processWindow(std::vector<std::vector<int>> window);

  /**
   * Compare two tuples to determine if one will delete the other.
   * Check if each component of the first tuple is less or greater than the corresponding component of the other tuple.
   * @param ti The first tuple.
   * @param tj The second tuple.
   * @return -1 if the first deletes the second (each component strictly less than);
   *          1 if the second deletes the first (each component strictly greater than);
   *          0 if both survives (some components less and other greater than)
   */
  static short compareTuple(const std::vector<int> &ti, const std::vector<int> &tj);

};


#endif //SKYLINE_SKYLINE_HPP
