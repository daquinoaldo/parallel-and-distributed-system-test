#include <iostream>
#include "Stream.hpp"

int main() {
  auto stream = new Stream();
  stream->generateTuples();
  std::cout << Stream::serializeWindow(stream->getWindow()) << std::endl;
  return 0;
}