#include "Utils.hpp"

std::string Utils::serializeStream(Stream *stream) {
  std::string string = "[";
  for (unsigned long i = 0; i < stream->size(); i++)
    string += serializeTuple(stream->get(i)) + (i != stream->size() - 1 ? ", " : "]");
  return string;
}

std::string Utils::serializeTuple(std::vector<int> tuple) {
  std::string string = "<";
  for (unsigned long i = 0; i < tuple.size(); i++)
    string += std::to_string(tuple[i]) + (i != tuple.size() - 1 ? "," : ">");
  return string;
}

std::string Utils::serializeWindow(std::vector<std::vector<int>> window) {
  std::string string = "[";
  for (unsigned long i = 0; i < window.size(); i++)
    string += serializeTuple(window[i]) + (i != window.size() - 1 ? ", " : "]");
  return string;
}