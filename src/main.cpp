#include <cstring>
#include <iostream>
#include <thread>
#include <cmath>
#include "Utils.hpp"
#include "SecureStream.hpp"
#include "SecureQueue.hpp"
#include "Workers.hpp"
#include "Timer.hpp"

long sequential(unsigned int seed, int w, int t, int k, long l) {
  // timer
  Timer timer("Sequential");

  // data structures
  auto inputStream = new Stream(t, w, k, l, seed);
  auto outputStream = new Queue<std::pair<int, Skyline>>;

  // generate input stream
  Workers::generate(inputStream);

  // pick a window, calculate skyline, put it in output stream
  Workers::work(inputStream, outputStream);

  // print the output stream
  Workers::print(outputStream);

  // return time spent for autopilot
  return timer.getTime();
}

long parallel(unsigned int seed, int w, int t, int k, long l, int nw) {
  // timer
  Timer timer("Parallel");

  // data structures
  auto inputStream = new SecureStream(t, w, k, l, seed);
  auto outputStream = new SecureQueue<std::pair<int, Skyline>>;

  // generate input stream
  std::thread streamGenerator(Workers::secureGenerate, inputStream);

  // workers
  Queue<std::thread *> threads;
  for (int i = 0; i < nw; i++) {
    auto worker = new std::thread(Workers::secureWork, inputStream, outputStream, i);
    threads.push(worker);
  }

  // print the output stream
  std::thread outputPrinter(Workers::securePrint, outputStream);

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
  int w = 30;
  int t = 3;
  int k = 1;
  long l = 10000;
  int nw = 4;

  // run and save times
  long seq = sequential(seed, w, t, k, l);
  long par = parallel(seed, w, t, k, l, nw);

  // report
  std::cout << std::endl;
  std::cout << "AUTOPILOT REPORT" << std::endl;
  std::cout << "Sequential:\t" << seq << std::endl;
  std::cout << "Parallel:\t" << par << std::endl;
  std::cout << std::endl;
}


// TODO: pointer vs references: a references produced a deep copy
// TODO: all pointers must be deallocated explicitly with delete
int main(int argc, char *argv[]) {

  // auto mode arguments
  if (argc == 2 && strcmp(argv[1], "auto") == 0) {
    autopilot(); // run the autopilot
    return 0;   // stop execution of standard workflow
  }

  // help message
  if (argc < 6 || argc > 7) {
    std::cout << std::endl;
    std::cout << "Usage: skyline seed w t k l [nw]" << std::endl; // TODO: max tuple value?
    std::cout << "seed = seed for random numbers" << std::endl;
    std::cout << "w = window size" << std::endl;
    std::cout << "t = tuple size" << std::endl;
    std::cout << "k = shift factor" << std::endl;
    std::cout << "l = stream length" << std::endl;
    std::cout << "nw = number of worker, optional, empty or 0 for sequential execution" << std::endl;
    std::cout << std::endl;
    std::cout << "Auto usage: skyline auto" << std::endl;
    std::cout << "Will run parallel and sequential tests with timers and print some statistics." << std::endl;
    std::cout << std::endl;
    return 1;
  }

  // standard arguments
  auto seed = (unsigned int) atoi(argv[1]); // NOLINT(cert-err34-c)
  auto w = atoi(argv[2]); // NOLINT(cert-err34-c)
  auto t = atoi(argv[3]); // NOLINT(cert-err34-c)
  auto k = atoi(argv[4]); // NOLINT(cert-err34-c)
  auto l = atol(argv[5]); // NOLINT(cert-err34-c)
  auto nw = argc == 7 ? atoi(argv[6]) : 0; // NOLINT(cert-err34-c)

  std::cout << "[Main]\tExpected windows: " << ceil(((double) (l - w)) / k) + 1 << std::endl;

  // run with nw for parallel, without nw for sequential
  if (nw > 0) parallel(seed, w, t, k, l, nw);
  else sequential(seed, w, t, k, l);

}

// TODO: doesn't scale
// TODO: segmentation fault
// seq    44171
// par1   34368
// par2   30226
// par3   34774
// par4   36809
// par5   34631
// par64  36585
// par128 46547