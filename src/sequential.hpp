#include "Timer.hpp"
#include "Task.hpp"

long sequential(unsigned s, unsigned w, unsigned t, unsigned k, unsigned long l, bool v);

long sequential(unsigned s, unsigned w, unsigned t, unsigned k, unsigned long l, bool v) {
  // timer
  Timer timer("Sequential");

  // data structures
  auto inputStream = new Stream(t, w, k, l, s);
  auto outputStream = new Queue<std::pair<int, Skyline>>;

  // generate input stream
  Task::generator(inputStream, v);

  // pick a window, calculate skyline, put it in output stream
  Task::worker(inputStream, outputStream, v);

  // print the output stream
  Task::printer(outputStream, v);

  // release the memory
  delete inputStream;
  delete outputStream;

  // return time spent for autopilot
  return timer.getTime();
}