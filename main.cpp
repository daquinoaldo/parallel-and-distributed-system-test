#include <iostream>
#include "Utils.hpp"

void sequential(unsigned int seed, int w, int t, int k, long l) {
  // data structures
  auto inputStream = new Stream(t, w, k, l, seed);
  auto outputStream = new Queue<std::pair<int, Skyline>>;

  // generate input stream
  inputStream->generateTuples();
  std::cout << "Stream: " << Utils::serializeStream(inputStream) << std::endl << std::endl << std::endl;

  // pick a window, calculate skyline, put it in output stream
  int wIndex;
  Window window;
  std::tie(wIndex, window) = inputStream->getWindow();
  while (!window.empty()) {
    auto skyline = Utils::processWindow(window);
    std::cout << "Window " + std::to_string(wIndex) + ":\t" << Utils::serializeWindow(window) << std::endl;
    std::cout << "Skyline " + std::to_string(wIndex) + ":\t" << Utils::serializeWindow(skyline) << std::endl
              << std::endl;
    outputStream->push(std::pair(wIndex, skyline));
    std::tie(wIndex, window) = inputStream->getWindow();
  }

  // print the output stream
  int sIndex;
  Skyline skyline;
  std::cout << std::endl << "Skylines:" << std::endl;
  std::tie(sIndex, skyline) = outputStream->pop();
  while (!skyline.empty()) {
    std::cout << std::to_string(sIndex) + ":\t" + Utils::serializeWindow(skyline) << std::endl;
    std::tie(sIndex, skyline) = outputStream->pop();
  }
}

int main(int argc, char *argv[]) {

  // Process argv
  if (argc < 6 || argc > 7) {
    std::cout << "Usage: skyline seed w t k l [nw]" << std::endl;
    return 1;
  }

  auto seed = (unsigned int) atoi(argv[1]); // NOLINT(cert-err34-c)
  auto w = atoi(argv[2]); // NOLINT(cert-err34-c)
  auto t = atoi(argv[3]); // NOLINT(cert-err34-c)
  auto k = atoi(argv[4]); // NOLINT(cert-err34-c)
  auto l = atol(argv[5]); // NOLINT(cert-err34-c)
  auto nw = argc == 7 ? atoi(argv[6]) : 1; // NOLINT(cert-err34-c)


  // Sequential version
  sequential(seed, w, t, k, l);

}