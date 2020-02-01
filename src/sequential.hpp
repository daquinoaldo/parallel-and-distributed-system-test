#include "Timer.hpp"

long sequential(Stream* inputStream, bool v);

long sequential(Stream* inputStream, bool v) {
  Timer timer("Sequential");

  for (ulong i = 0; i < inputStream->w_no(); i++) {
    auto window = inputStream->pickWindow(i);
    if (v) std::cout << "Window " << i << ": " << Utils::serializeWindow(window) << std::endl;
    auto skyline = Utils::processWindow(window);
    if (v) std::cout << "[Skyline " << skyline->id() << ": " << Utils::serializeWindow(skyline) << std::endl;
    delete skyline;
  }

  return timer.getTime();
}