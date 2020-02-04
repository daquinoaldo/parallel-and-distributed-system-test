#include <cstring>
#include <iostream>
#include <thread>
#include "Utils.hpp"
#include "Timer.hpp"
#include "sequential.hpp"
#include "parallel.hpp"
#include "fastflow.hpp"

void help();
void autopilot(Stream* inputStream, bool v, unsigned max_nw);


void autopilot(Stream* inputStream, bool v, unsigned max_nw) {
  unsigned long seq;
  unsigned limit = (unsigned) floor(log2(max_nw));

  // RUN

  // run sequential
  std::cout << std::endl << "Sequential."<< std::endl;
  seq = sequential(inputStream, v);

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
    emitterCollectorTimes[i] = 0;//emitterCollector(inputStream, v, nw);
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
  std::cout << "Usage: skyline mode s w t k l [nw]" << std::endl;
  std::cout << "mode = auto, sequential, parallel, emitter-collector, fastflow" << std::endl;
  std::cout << "s = seed for random numbers" << std::endl;
  std::cout << "w = window size" << std::endl;
  std::cout << "t = tuple size" << std::endl;
  std::cout << "k = shift factor" << std::endl;
  std::cout << "l = stream length" << std::endl;
  std::cout << "v = verbose (0 to suppress print, 1 to show)" << std::endl;
  std::cout << "nw = number of worker (default: hardware concurrency)" << std::endl;
  std::cout << std::endl;
}


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
  auto t = argc >= 5 ? (unsigned) atoi(argv[4]) : 10;                         // tuple size
  auto k = argc >= 6 ? (unsigned) atoi(argv[5]) : 1;                          // sliding factor
  auto l = argc >= 7 ? (unsigned long) atol(argv[6]) : 100000;                // stream length
  auto v = argc >= 8 ? (bool) atoi(argv[7]) : false;                          // verbose
  auto nw = argc >= 9 ? (unsigned) atoi(argv[8]) : concurentThreadsSupported; // number of workers

  // init rand with a seed
  srand(s);

  // generate input stream
  auto inputStream = new Stream(l, w, t, k);
  std::cout <<"[Main] Expected windows: " << inputStream->w_no() << std::endl << std::endl;
  if (v) std::cout << "Stream: " << Utils::serializeWindow(inputStream) << std::endl;


  // run the chosen mode
  if (strcmp(mode, "auto") == 0) {
    std::cout << "[Main] Running in autopilot mode." << std::endl;
    autopilot(inputStream, v, nw);
  }
  else if (strcmp(mode, "sequential") == 0) {
    std::cout << "[Main] Running in sequential mode." << std::endl;
    sequential(inputStream, v);
    sequentialTimes(inputStream);
  }
  else if (strcmp(mode, "parallel") == 0) {
    std::cout << "[Main] Running in parallel mode." << std::endl;
    parallel(inputStream, v, nw);
  }
  else if (strcmp(mode, "emitter-collector") == 0) {
    std::cout << "[Main] Running in emitter-collector mode." << std::endl;
    //emitterCollector(inputStream, v, nw);
  }
  else if (strcmp(mode, "fastflow") == 0) {
    std::cout << "[Main] Running in fastflow mode." << std::endl;
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

// TODO: It scales only with large windows, i.e. when the task for each worker is very high.
// - try to use a profiler
// - try parallel picking more than one window per time

// TIMES ./skyline auto 42 1000 10 1 100000 0 256
// Sequential:             308702677
// Parallel 1 threads:     312465737
// Parallel 2 threads:     162823516
// Parallel 4 threads:      81443014
// Parallel 8 threads:      41193086
// Parallel 16 threads:     22381938
// Parallel 32 threads:     11705852
// Parallel 64 threads:      6181175
// Parallel 128 threads:     5501195
// Parallel 256 threads:     5219642
// Fastflow 1 threads:     324519100
// Fastflow 2 threads:     162298761
// Fastflow 4 threads:      81268418
// Fastflow 8 threads:      40669155
// Fastflow 16 threads:     20387590
// Fastflow 32 threads:     10261423
// Fastflow 64 threads:      9854903
// Fastflow 128 threads:     8298919
// Fastflow 256 threads:     7288061