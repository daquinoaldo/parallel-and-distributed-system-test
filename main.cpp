#include <iostream>
#include "Stream.hpp"
#include "Skyline.hpp"

void sequential(Stream *stream) {
  // first worker: tuple stream producer
  stream->generateTuples();
  std::cout << "Stream: " << stream->serializeStream() << std::endl;

  // workers: pick a window, calculate skyline, put it in output stream
  auto window = stream->getWindow();
  while (!window.empty()) {
    auto skyline = Skyline::processWindow(window);
    std::cout << "[Worker 1] Window: " << Stream::serializeWindow(window) << std::endl;
    std::cout << "[Worker 1] Skyline: " << Stream::serializeWindow(skyline) << std::endl;
    window = stream->getWindow();
  }

}

//int main(int argc, char *argv[]) {
int main() {

  /*
  // Process argv
  if (argc < 5 || argc > 6) {
    std::cout << "Usage: skyline seed w t k l [nw]" << std::endl;
    return 1;
  }

  auto seed = (unsigned int) atoi(argv[1]); // NOLINT(cert-err34-c)
  auto w = (unsigned int) atoi(argv[2]); // NOLINT(cert-err34-c)
  auto t = (unsigned int) atoi(argv[3]); // NOLINT(cert-err34-c)
  auto k = (unsigned int) atoi(argv[4]); // NOLINT(cert-err34-c)
  auto l = (unsigned int) atoi(argv[5]); // NOLINT(cert-err34-c)
  auto nw = argc == 6 ? (unsigned int) atoi(argv[1]) : 1; // NOLINT(cert-err34-c)
   */


  // Sequential version
  auto stream = new Stream();
  sequential(stream);

}