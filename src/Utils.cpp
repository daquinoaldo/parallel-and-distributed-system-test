#include "Utils.hpp"


Tuple Utils::newTuple(int t) {
  Tuple tuple((unsigned long) t);
  for (int i = 0; i < t; i++) {
    auto r = rand(); // NOLINT(cert-msc30-c,cert-msc50-cpp)
#ifdef DEBUG
    r = r % 100;
#endif
    tuple[(unsigned long) i] = r;
  }
  return tuple;
}


std::string Utils::serializeTuple(Tuple tuple) {
  std::string string = "<";
  for (unsigned long i = 0; i < tuple.size(); i++)
    string += std::to_string(tuple[i]) + (i != tuple.size() - 1 ? "," : ">");
  return string;
}


std::string Utils::serializeWindow(Window window) {
  std::string string = "[";
  for (unsigned long i = 0; i < window.size(); i++)
    string += serializeTuple(window[i]) + (i != window.size() - 1 ? ", " : "]");
  return string;
}


/*std::string Utils::serializeStream(Stream *stream) {
  std::string string = "[";
  for (unsigned long i = 0; i < stream->size(); i++)
    string += serializeTuple(stream->get(i)) + (i != stream->size() - 1 ? ", " : "]");
  return string;
}*/


short Utils::compareTuple(const Tuple &ti, const Tuple &tj) {
  unsigned long k = 0;
  bool allLT = true;
  bool allGT = true;
  // Scan each component, stop if them don't eliminate themselves:
  while (k < ti.size() && (allLT || allGT)) {
    // if is bigger, this tuple has not each component less than the other tuple;
    if (ti[k] > tj[k]) {
      allLT = false;
      k++;
    }
      // if is smaller, this tuple has not each component greater than the other tuple;
    else if (ti[k] < tj[k]) {
      allGT = false;
      k++;
    }
      // else them are equals, so is neither GT nor LT.
    else {
      allLT = false;
      allGT = false;
    }
  }
  if (allLT) return -1;
  else if (allGT) return 1;
  return 0;
}


Skyline Utils::processWindow(Window window) {
  unsigned long i = 0;
  // For each tuple in window
  while (i < window.size()) {
    bool alive = true;
    unsigned long j = i + 1;
    // compares it with the subsequent elements
    while (alive && j < window.size()) {
      // checking if each tuple component of the first tuple is less or greater than
      // the corresponding component of the other tuple.
      short cmp = compareTuple(window[i], window[j]);
      // If the first tuple has each component strictly less than the corresponding component of the second one,
      // the first tuple eliminates the second one.
      if (cmp < 0) window.erase(window.begin() + (long) j);
        // Do not increment j, since window[j] is already the next item.
        // Vice-versa, if each component of the first one is strictly greater than the corresponding one
        // of the second tuple, the second tuple eliminates the first one.
      else if (cmp > 0) {
        alive = false;
        window.erase(window.begin() + (long) i);
      }
        // Else, them remains both on the window, since them are still candidates for the skyline.
        // Check the next item
      else j++;
    }
    if (alive) i++;  // else window[i] is already a new item.
  }
  return window;
}
