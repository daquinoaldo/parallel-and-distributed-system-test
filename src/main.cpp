#include <cstring>
#include <iostream>
#include <thread>
#include <cmath>
#include "Utils.hpp"
#include "SecureStream.hpp"
#include "SecureQueue.hpp"
#include "Workers.hpp"
#include "Timer.hpp"

long sequential(unsigned int seed, int w, int t, int k, long l, bool verbose) {
  // timer
  Timer timer("Sequential");

  // data structures
  auto inputStream = new Stream(t, w, k, l, seed);
  auto outputStream = new Queue<std::pair<int, Skyline>>;

  // generate input stream
  Workers::generate(inputStream, verbose);

  // pick a window, calculate skyline, put it in output stream
  Workers::work(inputStream, outputStream, verbose);

  // print the output stream
  Workers::print(outputStream, verbose);

  // return time spent for autopilot
  return timer.getTime();
}

long parallel(unsigned int seed, int w, int t, int k, long l, bool verbose, int nw) {
  // timer
  Timer timer("Parallel");

  // data structures
  auto inputStream = new SecureStream(t, w, k, l, seed);
  auto outputStream = new SecureQueue<std::pair<int, Skyline>>;

  // generate input stream
  std::thread streamGenerator(Workers::secureGenerate, inputStream, verbose);

  // workers
  Queue<std::thread *> threads;
  for (int i = 0; i < nw; i++) {
    auto worker = new std::thread(Workers::secureWork, inputStream, outputStream, i, verbose);
    threads.push(worker);
  }

  // print the output stream
  std::thread outputPrinter(Workers::securePrint, outputStream, verbose);

  // join threads
  streamGenerator.join();
  inputStream->awakeAll();  // awake threads waiting for a window  // TODO: this may be too early -> maybe not, after EOS they will terminate
  for (int i = 0; i < nw; i++)
    threads.pop()->join();
  outputPrinter.join();

  // return time spent for autopilot
  return timer.getTime();
}

void autopilot() {
  // parameters
  unsigned int seed = 42;
  int w = 50;       // window size
  int t = 10;       // tuple size
  int k = 1;        // sliding factor
  long l = 100000;  // stream length: 100.000
  bool v = false;   // verbose

    // compute number of threads
  unsigned concurentThreadsSupported = std::thread::hardware_concurrency();
  if (concurentThreadsSupported <= 0) // not able to detect
    concurentThreadsSupported = 128;
  std::cout << "AUTOPILOT: skyline " << w << " " << t << " " << k << " " << l << " " << v <<
    " " << concurentThreadsSupported << std::endl;

  unsigned limit = floor(log2(concurentThreadsSupported));
  long times[limit] = {};

  // run sequential
  long seq = sequential(seed, w, t, k, l, v);

  // run parallel
  for (int i = 0; i <= limit; i++) {
    unsigned nw = pow (2, i);
    times[i] = parallel(seed, w, t, k, l, v, nw);
  }

  // report
  std::cout << std::endl;
  std::cout << "AUTOPILOT REPORT" << std::endl;
  std::cout << "Sequential:\t\t" << seq << std::endl;
  for (int i = 0; i <= limit; i++) {
    unsigned nw = pow (2, i);
    std::cout << "Parallel " << nw << " threads:\t" << times[i] << std::endl;
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
  auto seed = (unsigned int) atoi(argv[1]);
  auto w = atoi(argv[2]);  // window size
  auto t = atoi(argv[3]);  // tuple size
  auto k = atoi(argv[4]);  // sliding factor
  auto l = atoi(argv[5]);  // stream length
  bool v = atoi(argv[6]);  // verbose
  auto nw = argc == 8 ? atoi(argv[7]) : 0;

  std::cout << "[Main]\tExpected windows: " << ceil(((double) (l - w)) / k) + 1 << std::endl << std::endl;

  // run with nw for parallel, without nw for sequential
  if (nw > 0) parallel(seed, w, t, k, l, v, nw);
  else sequential(seed, w, t, k, l, v);

}

// TODO: doesn't scale
// seq    44171
// par1   34368
// par2   30226
// par3   34774
// par4   36809
// par5   34631
// par64  36585
// par128 46547