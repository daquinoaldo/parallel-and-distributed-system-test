#include "Timer.hpp"

long sequential(Stream* inputStream, bool v);
void sequentialTimes(Stream* inputStream);


long sequential(Stream* inputStream, bool v) {
  Timer timer("Sequential");

  for (ulong i = 0; i < inputStream->w_no(); i++) {
    auto window = inputStream->pickWindow(i);
    if (v) std::cout << "Window " << i << ": " << Utils::serializeWindow(window) << std::endl;
    auto skyline = Utils::processWindow(window);
    if (v) std::cout << "Skyline " << skyline->id() << ": " << Utils::serializeWindow(skyline) << std::endl;
    delete skyline;
  }

  return timer.getTime();
}

void sequentialTimes(Stream* inputStream) {
  std::chrono::_V2::system_clock::time_point start;
  std::chrono::_V2::system_clock::time_point stop;
  long pickWindow = 0;
  long processWindow = 0;
  long deleteSkyline = 0;

  for (ulong i = 0; i < inputStream->w_no(); i++) {
    start = std::chrono::system_clock::now();
    auto window = inputStream->pickWindow(i);
    stop = std::chrono::system_clock::now();
    pickWindow += std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count();

    start = std::chrono::system_clock::now();
    auto skyline = Utils::processWindow(window);
    stop = std::chrono::system_clock::now();
    processWindow += std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count();

    start = std::chrono::system_clock::now();
    delete skyline;
    stop = std::chrono::system_clock::now();
    deleteSkyline += std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count();
  }

  std::cout << "pickWindow: " << pickWindow/1000 << " musec" << std::endl;
  std::cout << "processWindow: " << processWindow/1000 << " musec" << std::endl;
  std::cout << "deleteSkyline: " << deleteSkyline/1000 << " musec" << std::endl;
}