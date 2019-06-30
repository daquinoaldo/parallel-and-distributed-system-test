#include <iostream>
#include "Stream.hpp"
#include "Skyline.hpp"

int main() {
  // init things
  auto stream = new Stream();

  // first worker: tuple stream producer
  stream->generateTuples();

  // workers: pick a window, calculate skyline, put it in output stream
  auto window = stream->getWindow();
  auto skyline = Skyline::processWindow(window);
  std::cout << "[Worker 1] Window: " << Stream::serializeWindow(window) << std::endl;
  std::cout << "[Worker 1] Skyline: " << Stream::serializeWindow(skyline) << std::endl;
}