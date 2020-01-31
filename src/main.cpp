#include <cstring>
#include <iostream>
#include <thread>
#include "Utils.hpp"
#include "Timer.hpp"
#include "parallel.hpp"
#include "fastflow.hpp"

void help();
void autopilot(Stream* inputStream, bool v, unsigned max_nw, unsigned nt);


void autopilot(Stream* inputStream, bool v, unsigned max_nw, unsigned nt) {
  unsigned long seq;
  unsigned limit = (unsigned) floor(log2(max_nw));

  // RUN

  // run sequential
  std::cout << std::endl << "Sequential."<< std::endl;
  seq = 0;//sequential(inputStream, v);

  // run parallel
  std::vector<unsigned long> parallelTimes(limit + 1);
  for (unsigned i = 0; i <= limit; i++) {
    unsigned nw = (unsigned) pow(2, i);
    std::cout << std::endl << "Parallel with " << nw << " threads."<< std::endl;
    parallelTimes[i] = parallel(inputStream, v, nw);
  }

  // run emitter-collector
  std::vector<unsigned long> emitterCollectorTimes(limit + 1);
  for (unsigned i = 0; i <= limit; i++) {
    unsigned nw = (unsigned) pow(2, i);
    std::cout << std::endl << "Emitter-collector with " << nw << " threads."<< std::endl;
    emitterCollectorTimes[i] = 0;//emitterCollector(inputStream, v, nw, nt);
  }

  // run fastflow
  std::vector<unsigned long> fastflowTimes(limit + 1);
  for (unsigned i = 0; i <= limit; i++) {
    unsigned nw = (unsigned) pow(2, i);
    std::cout << std::endl << "FastFlow with " << nw << " threads."<< std::endl;
    fastflowTimes[i] = fastflow(inputStream, v, nw);
  }


  // REPORT
  std::cout << std::endl;
  std::cout << "AUTOPILOT REPORT" << std::endl;

  // sequential
  std::cout << "Sequential:\t\t" << seq << std::endl;

  // parallel
  for (unsigned i = 0; i <= limit; i++) {
    unsigned nw = (unsigned) pow (2, i);
    std::cout << "Parallel " << nw << " threads:\t" << parallelTimes[i] << std::endl;
  }

  // emitter-collector
  for (unsigned i = 0; i <= limit; i++) {
    unsigned nw = (unsigned) pow (2, i);
    std::cout << "Emitter-collector " << nw << " threads:\t" << emitterCollectorTimes[i] << std::endl;
  }

  // fastflow
  for (unsigned i = 0; i <= limit; i++) {
    unsigned nw = (unsigned) pow (2, i);
    std::cout << "Fastflow " << nw << " threads:\t" << fastflowTimes[i] << std::endl;
  }

  std::cout << std::endl;
}


void help() {
  std::cout << std::endl;
  std::cout << "Usage: skyline mode s w t k l [nw [nt]]" << std::endl;
  std::cout << "mode = auto, sequential, parallel, emitter-collector, fastflow" << std::endl;
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

  // standard arguments
  auto mode = argc >= 2 ? argv[1] : "auto";                                   // mode
  auto s = argc >= 3 ? (unsigned) atoi(argv[2]) : 42;                         // seed for random
  auto w = argc >= 4 ? (unsigned) atoi(argv[3]) : 100;                        // window size
  auto t = argc >= 5 ? (unsigned) atoi(argv[4]) : 50;                         // tuple size
  auto k = argc >= 6 ? (unsigned) atoi(argv[5]) : 1;                          // sliding factor
  auto l = argc >= 7 ? (unsigned long) atol(argv[6]) : 100000;                // stream length
  auto v = argc >= 8 ? (bool) atoi(argv[7]) : false;                          // verbose
  auto nw = argc >= 9 ? (unsigned) atoi(argv[8]) : concurentThreadsSupported; // number of workers
  auto nt = argc == 10 ? (unsigned) atoi(argv[9]) : 1;                        // task at a time per worker

  // init rand with a seed
  srand(s);

  // generate input stream
  auto inputStream = new Stream(l, w, t, k);
  std::cout <<"[Main]\tExpected windows: " << inputStream->w_no() << std::endl << std::endl;


  // run the chosen mode
  if (strcmp(mode, "auto") == 0) {
    std::cout << "[Main]\tRunning in autopilot mode." << std::endl;
    autopilot(inputStream, v, nw, nt);
  }
  else if (strcmp(mode, "sequential") == 0) {
    std::cout << "[Main]\tRunning in sequential mode." << std::endl;
    //sequential(inputStream, v);
  }
  else if (strcmp(mode, "parallel") == 0) {
    std::cout << "[Main]\tRunning in parallel mode." << std::endl;
    parallel(inputStream, v, nw);
  }
  else if (strcmp(mode, "emitter-collector") == 0) {
    std::cout << "[Main]\tRunning in emitter-collector mode." << std::endl;
    //emitterCollector(inputStream, v, nw, nt);
  }
  else if (strcmp(mode, "fastflow") == 0) {
    std::cout << "[Main]\tRunning in fastflow mode." << std::endl;
    fastflow(inputStream, v, nw);
  }
  else {  // invalid mode
    delete inputStream;
    help();    // show the help message
    return 1;  // return error code
  }

  // release memory
  delete inputStream;

}

// TODO: Still not scale. Try to figure out why with a profiler.
// Try also parallel picking more than one window per time.
// Try also using pointers instead of copy.


// FF time
// 1: 404067
// 2: 220247
// 4: 129758
// 8:  80376
// 16: 46564
// 32: 64438
// 64: 80502