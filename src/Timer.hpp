#ifndef SKYLINE_TIMER_HPP
#define SKYLINE_TIMER_HPP


#include <iostream>
#include <chrono>

class Timer {

private:
  std::chrono::system_clock::time_point start;
  std::string message;

public:
  explicit Timer(std::string message);

  long getTime();

  ~Timer();

};


#endif //SKYLINE_TIMER_HPP
