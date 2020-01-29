#include <cstring>
#include <iostream>
#include <thread>
#include <cmath>
#include "Utils.hpp"
#include "SecureStream.hpp"
#include "SecureQueue.hpp"
#include "Timer.hpp"
#include "sequential.hpp"
#include "parallel.hpp"

void help();
void autopilot(unsigned s, unsigned w, unsigned t, unsigned k, unsigned long l, bool v, unsigned max_nw, unsigned nt);


void autopilot(unsigned s, unsigned w, unsigned t, unsigned k, unsigned long l, bool v, unsigned max_nw, unsigned nt) {
  unsigned long seq;
  std::vector<unsigned long>* parallelTimes;
  std::vector<unsigned long>* semiparallelTimes;
  std::vector<unsigned long>* emitterCollectorTimes;
  unsigned limit = (unsigned) floor(log2(max_nw));

  // RUN

  // run sequential
  std::cout << std::endl << "Sequential."<< std::endl;
  seq = sequential(s, w, t, k, l, v);

  // run parallel
  parallelTimes = new std::vector<unsigned long>(limit + 1);
  for (unsigned i = 0; i <= limit; i++) {
    unsigned nw = (unsigned) pow(2, i);
    std::cout << std::endl << "Parallel with " << nw << " threads."<< std::endl;
    parallelTimes->at(i) = parallel(s, w, t, k, l, v, nw);
  }

  // run semiparallel
  semiparallelTimes = new std::vector<unsigned long>(limit + 1);
  for (unsigned i = 0; i <= limit; i++) {
    unsigned nw = (unsigned) pow(2, i);
    std::cout << std::endl << "Semi-parallel with " << nw << " threads."<< std::endl;
    semiparallelTimes->at(i) = semiparallel(s, w, t, k, l, v, nw);
  }

  // run emitter-collector
  emitterCollectorTimes = new std::vector<unsigned long>(limit + 1);
  for (unsigned i = 0; i <= limit; i++) {
    unsigned nw = (unsigned) pow(2, i);
    std::cout << std::endl << "Emitter-collector with " << nw << " threads."<< std::endl;
    emitterCollectorTimes->at(i) = emitterCollector(s, w, t, k, l, v, nw, nt);
  }


  // REPORT
  std::cout << std::endl;
  std::cout << "AUTOPILOT REPORT" << std::endl;

  // sequential
  std::cout << "Sequential:\t\t" << seq << std::endl;

  // parallel
  for (unsigned i = 0; i <= limit; i++) {
    unsigned nw = (unsigned) pow (2, i);
    std::cout << "Parallel " << nw << " threads:\t" << parallelTimes->at(i) << std::endl;
  }

  // semi-parallel
  for (unsigned i = 0; i <= limit; i++) {
    unsigned nw = (unsigned) pow (2, i);
    std::cout << "Semi-parallel " << nw << " threads:\t" << semiparallelTimes->at(i) << std::endl;
  }

  // emitter-collector
  for (unsigned i = 0; i <= limit; i++) {
    unsigned nw = (unsigned) pow (2, i);
    std::cout << "Emitter-collector " << nw << " threads:\t" << emitterCollectorTimes->at(i) << std::endl;
  }

  // release memory
  delete parallelTimes;
  delete semiparallelTimes;
  delete emitterCollectorTimes;

  std::cout << std::endl;
}


void help() {
  std::cout << std::endl;
  std::cout << "Usage: skyline mode s w t k l [nw [nt]]" << std::endl;
  std::cout << "mode = auto, sequential, parallel, semi-parallel, emitter-collector, ff" << std::endl;
  std::cout << "s = seed for random numbers" << std::endl;
  std::cout << "w = window size" << std::endl;
  std::cout << "t = tuple size" << std::endl;
  std::cout << "k = shift factor" << std::endl;
  std::cout << "l = stream length" << std::endl;
  std::cout << "v = verbose (0 to suppress print, 1 to show)" << std::endl;
  std::cout << "nw = number of worker (default: hardware concurrency)" << std::endl;
  std::cout << "nt = number of task at a time assigned to the worker (default: 1)" << std::endl;
  std::cout << std::endl;
}


// TODO: pointer vs references: a references produced a deep copy
// TODO: all pointers must be deallocated explicitly with delete
int main(int argc, char *argv[]) {
  // help message
  if (argc == 2 && (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)) {
    help();    // show the help message
    return 0;  // stop execution of standard workflow
  }

  // compute number of threads
  unsigned concurentThreadsSupported = std::thread::hardware_concurrency();
  if (concurentThreadsSupported <= 0) // not able to detect
    concurentThreadsSupported = 128;
  std::cout << "[Main] Your machine supports " << concurentThreadsSupported << " threads." << std::endl;

  // auto mode arguments
  if (argc >= 2 && strcmp(argv[1], "auto") == 0) {
    
  }

  // standard arguments
  auto mode = argc >= 2 ? argv[1] : "auto";                                   // mode
  auto s = argc >= 3 ? (unsigned) atoi(argv[2]) : 42;                         // seed for random
  auto w = argc >= 4 ? (unsigned) atoi(argv[3]) : 100;                        // window size
  auto t = argc >= 5 ? (unsigned) atoi(argv[4]) : 50;                         // tuple size
  auto k = argc >= 6 ? (unsigned) atoi(argv[5]) : 1;                          // sliding factor
  auto l = argc >= 7 ? (unsigned long) atol(argv[6]) : 1000;                 // stream length
  auto v = argc >= 8 ? (bool) atoi(argv[7]) : false;                          // verbose
  auto nw = argc >= 9 ? (unsigned) atoi(argv[8]) : concurentThreadsSupported; // number of workers
  auto nt = argc == 10 ? (unsigned) atoi(argv[9]) : 1;                        // task at a time per worker

  std::cout <<"[Main]\tExpected windows: " << ceil(((double) (l - w)) / k) + 1 << std::endl << std::endl;

  // run the chosen mode
  if (strcmp(mode, "auto") == 0) {
    std::cout << "[Main]\tRunning in autopilot mode." << std::endl;
    autopilot(s, w, t, k, l, v, nw, nt);
  }
  else if (strcmp(mode, "sequential") == 0) {
    std::cout << "[Main]\tRunning in sequential mode." << std::endl;
    sequential(s, w, t, k, l, v);
  }
  else if (strcmp(mode, "parallel") == 0) {
    std::cout << "[Main]\tRunning in parallel mode." << std::endl;
    parallel(s, w, t, k, l, v, nw);
  }
  else if (strcmp(mode, "semi-parallel") == 0) {
    std::cout << "[Main]\tRunning in semi-parallel mode." << std::endl;
    semiparallel(s, w, t, k, l, v, nw);
  }
  else if (strcmp(mode, "emitter-collector") == 0) {
    std::cout << "[Main]\tRunning in emitter-collector mode." << std::endl;
    emitterCollector(s, w, t, k, l, v, nw, nt);
  }
  else {  // invalid mode
    help();    // show the help message
    return 1;  // return error code
  }
}

// TODO: Still not scale. Try to figure out why with a profiler.
// Try also parallel picking more than one window per time.
// Try also using pointers instead of copy.