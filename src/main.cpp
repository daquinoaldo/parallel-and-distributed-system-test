#include <cstring>
#include <iostream>
#include <thread>
#include <cmath>
#include "Utils.hpp"
#include "SecureStream.hpp"
#include "SecureQueue.hpp"
#include "Task.hpp"
#include "Timer.hpp"


long sequential(unsigned seed, unsigned w, unsigned t, unsigned k, unsigned long l, bool verbose) {
  // timer
  Timer timer("Sequential");

  // data structures
  auto inputStream = new Stream(t, w, k, l, seed);
  auto outputStream = new Queue<std::pair<int, Skyline>>;

  // generate input stream
  Task::generator(inputStream, verbose);

  // pick a window, calculate skyline, put it in output stream
  Task::worker(inputStream, outputStream, verbose);

  // print the output stream
  Task::printer(outputStream, verbose);

  // return time spent for autopilot
  return timer.getTime();
}


long parallel(unsigned seed, unsigned w, unsigned t, unsigned k, unsigned long l, bool verbose, unsigned nw) {
  // timer
  Timer timer("Parallel");

  // data structures
  auto inputStream = new SecureStream(t, w, k, l, seed);
  auto outputStream = new SecureQueue<std::pair<int, Skyline>>;

  // generate input stream
  std::thread streamGenerator(Task::secureGenerator, inputStream, verbose);

  // workers
  Queue<std::thread *> threads;
  for (unsigned i = 0; i < nw; i++) {
    auto worker = new std::thread(Task::secureWorker, inputStream, outputStream, i, verbose);
    threads.push(worker);
  }

  // print the output stream
  std::thread outputPrinter(Task::securePrinter, outputStream, verbose);

  // join threads
  streamGenerator.join();
  inputStream->awakeAll();
  for (unsigned i = 0; i < nw; i++)
    threads.pop()->join();
  outputPrinter.join();

  // return time spent for autopilot
  return timer.getTime();
}


long semiparallel(unsigned seed, unsigned w, unsigned t, unsigned k, unsigned long l, bool verbose, unsigned nw) {
  // timer
  Timer timer("Parallel");

  // data structures
  auto inputStream = new SecureStream(t, w, k, l, seed);
  auto outputStream = new SecureQueue<std::pair<int, Skyline>>;

  // generate input stream before starting workers
  Task::secureGenerator(inputStream, verbose);

  // workers
  Queue<std::thread *> threads;
  for (unsigned i = 0; i < nw; i++) {
    auto worker = new std::thread(Task::secureWorker, inputStream, outputStream, i, verbose);
    threads.push(worker);
  }

  // join threads
  inputStream->awakeAll();
  for (unsigned i = 0; i < nw; i++)
    threads.pop()->join();

  // print the output stream after the workers have finished
  Task::securePrinter(outputStream, verbose);

  // return time spent for autopilot
  return timer.getTime();
}


void autopilot() {
  // parameters
  unsigned seed = 42;
  unsigned w = 100;         // window size
  unsigned t = 50;          // tuple size
  unsigned k = 1;           // sliding factor
  unsigned long l = 100000; // stream length: 10.000
  bool v = false;           // verbose

    // compute number of threads
  unsigned concurentThreadsSupported = std::thread::hardware_concurrency();
  if (concurentThreadsSupported <= 0) // not able to detect
    concurentThreadsSupported = 128;
  std::cout << "AUTOPILOT: skyline " << seed << w << " " << t << " " << k << " " << l << " " << v <<
    " " << concurentThreadsSupported << std::endl;
  
  std::cout << "[AUTOPILOT]\tExpected windows: " << ceil(((double) (l - w)) / k) + 1 << std::endl << std::endl;


  unsigned limit = (unsigned) floor(log2(concurentThreadsSupported));

  // run sequential
  std::cout << std::endl << "Sequential."<< std::endl;
  long seq = sequential(seed, w, t, k, l, v);

  // run parallel
  std::vector<int> parallelTimes(limit);
  for (unsigned i = 0; i <= limit; i++) {
    unsigned nw = (unsigned) pow(2, i);
    std::cout << std::endl << "Parallel with  " << nw << " threads."<< std::endl;
    parallelTimes[i] = parallel(seed, w, t, k, l, v, nw);
  }

  // run semiparallel
  std::vector<int> semiparallelTimes(limit);
  for (unsigned i = 0; i <= limit; i++) {
    unsigned nw = (unsigned) pow(2, i);
    std::cout << std::endl << "Semi-parallel with  " << nw << " threads."<< std::endl;
    semiparallelTimes[i] = semiparallel(seed, w, t, k, l, v, nw);
  }

  // report
  std::cout << std::endl;
  std::cout << "AUTOPILOT REPORT" << std::endl;
  std::cout << "Sequential:\t\t" << seq << std::endl;
  for (unsigned i = 0; i <= limit; i++) {
    unsigned nw = (unsigned) pow (2, i);
    std::cout << "Parallel " << nw << " threads:\t" << parallelTimes[i] << std::endl;
  }
  for (unsigned i = 0; i <= limit; i++) {
    unsigned nw = (unsigned) pow (2, i);
    std::cout << "Semi-parallel " << nw << " threads:\t" << semiparallelTimes[i] << std::endl;
  }
  std::cout << std::endl;
}

void help() {
  std::cout << std::endl;
  std::cout << "Usage: skyline seed w t k l [nw]" << std::endl;
  std::cout << "seed = seed for random numbers" << std::endl;
  std::cout << "w = window size" << std::endl;
  std::cout << "t = tuple size" << std::endl;
  std::cout << "k = shift factor" << std::endl;
  std::cout << "l = stream length" << std::endl;
  std::cout << "v = verbose (0 to suppress print, 1 to show)" << std::endl;
  std::cout << "nw = number of worker, optional, empty or 0 for sequential execution" << std::endl;
  std::cout << std::endl;
  std::cout << "Auto usage: skyline auto" << std::endl;
  std::cout << "Will run parallel and sequential tests with timers and print some statistics." << std::endl;
  std::cout << std::endl;
}


// TODO: pointer vs references: a references produced a deep copy
// TODO: all pointers must be deallocated explicitly with delete
int main(int argc, char *argv[]) {

  // auto mode arguments
  if (argc >= 2 && strcmp(argv[1], "auto") == 0) {
    autopilot();  // run the autopilot
    return 0;     // stop execution of standard workflow
  }

  // help message
  if (argc == 2 && (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)) {
    help();    // show the help message
    return 0;  // stop execution of standard workflow
  }

  // wrong args: help message
  if (argc != 7 && argc != 8) {
    help();    // show the help message
    return 1;  // return error code
  }

  // standard arguments
  auto seed = (unsigned) atoi(argv[1]);
  auto w = (unsigned) atoi(argv[2]);        // window size
  auto t = (unsigned) atoi(argv[3]);        // tuple size
  auto k = (unsigned) atoi(argv[4]);        // sliding factor
  auto l = (unsigned long) atol(argv[5]);   // stream length
  auto v = (bool) atoi(argv[6]);            // verbose
  auto nw = argc == 8 ? (unsigned) atoi(argv[7]) : 0;

  std::cout << "[Main]\tExpected windows: " << ceil(((double) (l - w)) / k) + 1 << std::endl << std::endl;

  // run with nw for parallel, without nw for sequential
  if (nw > 0) parallel(seed, w, t, k, l, v, nw);
  else sequential(seed, w, t, k, l, v);

}

// TODO: semi-parallel doesn't scale. The problem is not the emitter but the locks.
// Try to use a queue of task for each worker and an emitter and a collector.