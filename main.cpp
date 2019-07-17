#include <iostream>
#include <thread>
#include <cmath>
#include "Utils.hpp"
#include "SecureStream.hpp"
#include "SecureQueue.hpp"
#include "Workers.hpp"

void sequential(unsigned int seed, int w, int t, int k, long l) {
  // data structures
  auto inputStream = new Stream(t, w, k, l, seed);
  auto outputStream = new Queue<std::pair<int, Skyline>>;

  // generate input stream
  Workers::generate(inputStream);

  // pick a window, calculate skyline, put it in output stream
  Workers::work(inputStream, outputStream);

  // print the output stream
  Workers::print(outputStream);
}


// TODO: fix cout
void parallel(unsigned int seed, int w, int t, int k, long l, int nw) {
  // data structures
  auto inputStream = new SecureStream(t, w, k, l, seed);
  auto outputStream = new SecureQueue<std::pair<int, Skyline>>;

  // generate input stream
  std::thread streamGenerator(Workers::secureGenerate, inputStream);

  // workers
  auto threads = new Queue<std::thread *>();
  for (int i = 0; i < nw; i++) {
    auto worker = new std::thread(Workers::secureWork, inputStream, outputStream, i);
    threads->push(worker);
  }

  // print the output stream
  std::thread outputPrinter(Workers::securePrint, outputStream);

  // join threads
  streamGenerator.join();
  inputStream->awakeAll();  // awake threads waiting for a window  // TODO: this may be too early -> maybe not, after EOS they will terminate
  for (int i = 0; i < nw; i++)
    threads->pop()->join();
  outputPrinter.join();
}


// TODO: pointer vs references: a references produced a deep copy
int main(int argc, char *argv[]) {

  // Process argv
  if (argc < 6 || argc > 7) {
    std::cout << "Usage: skyline seed w t k l [nw]" << std::endl;
    std::cout << "seed = seed for random numbers" << std::endl;
    std::cout << "w = window size" << std::endl;
    std::cout << "t = tuple size" << std::endl;
    std::cout << "k = shift factor" << std::endl;
    std::cout << "nw = number of worker, optional, empty or 0 for sequential execution" << std::endl;
    return 1;
  }

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