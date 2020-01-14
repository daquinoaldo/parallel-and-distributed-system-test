#include "Timer.hpp"

Timer::Timer(std::string message) : message(std::move(message)) {
  start = std::chrono::system_clock::now();
}

Timer::~Timer() {
  std::cout << "[Timer]\t\t" + message + " computed in " + std::to_string(getTime()) + " usec" + "\n";
}

long Timer::getTime() {
  auto stop = std::chrono::system_clock::now();
  return std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
}
