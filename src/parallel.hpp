#include <thread>
#include "Timer.hpp"
#include "Task.hpp"

long parallel(unsigned s, unsigned w, unsigned t, unsigned k, unsigned long l, bool v, unsigned nw);
long semiparallel(unsigned s, unsigned w, unsigned t, unsigned k, unsigned long l, bool v, unsigned nw);
long emitterCollector(unsigned s, unsigned w, unsigned t, unsigned k, unsigned long l, bool v, unsigned nw, unsigned nt);


long parallel(unsigned s, unsigned w, unsigned t, unsigned k, unsigned long l, bool v, unsigned nw) {
  // timer
  Timer timer("Parallel");

  // data structures
  auto inputStream = new SecureStream(t, w, k, l, s);
  auto outputStream = new SecureQueue<std::pair<int, Skyline>>;

  // generate input stream
  std::thread streamGenerator(Task::secureGenerator, inputStream, v);

  // workers
  Queue<std::thread *> threads;
  for (unsigned i = 0; i < nw; i++) {
    auto worker = new std::thread(Task::secureWorker, inputStream, outputStream, i, v);
    threads.push(worker);
  }

  // print the output stream
  std::thread outputPrinter(Task::securePrinter, outputStream, v);

  // join threads
  streamGenerator.join();
  inputStream->awakeAll();
  for (unsigned i = 0; i < nw; i++)
    threads.pop()->join();
  outputPrinter.join();

  // return time spent for autopilot
  return timer.getTime();
}


long semiparallel(unsigned s, unsigned w, unsigned t, unsigned k, unsigned long l, bool v, unsigned nw) {
  // timer
  Timer timer("Parallel");

  // data structures
  auto inputStream = new SecureStream(t, w, k, l, s);
  auto outputStream = new SecureQueue<std::pair<int, Skyline>>;

  // generate input stream before starting workers
  Task::secureGenerator(inputStream, v);

  // workers
  Queue<std::thread *> threads;
  for (unsigned i = 0; i < nw; i++) {
    auto worker = new std::thread(Task::secureWorker, inputStream, outputStream, i, v);
    threads.push(worker);
  }

  // join threads
  inputStream->awakeAll();
  for (unsigned i = 0; i < nw; i++)
    threads.pop()->join();

  // print the output stream after the workers have finished
  Task::securePrinter(outputStream, v);

  // return time spent for autopilot
  return timer.getTime();
}


long emitterCollector(unsigned s, unsigned w, unsigned t, unsigned k, unsigned long l, bool v, unsigned nw, unsigned nt) {
  // timer
  Timer timer("EmitterCollector");

  // data structures
  auto inputStream = new SecureStream(t, w, k, l, s);
  auto outputStream = new SecureQueue<std::pair<int, Skyline>>;
  auto inputQueues = new std::vector<WorkerQueue*>(nw);
  for (unsigned i = 0; i < nw; i++) {
    inputQueues->at(i) = new WorkerQueue();
  }
  auto outputQueues = new std::vector<WorkerQueue*>(nw);
  for (unsigned i = 0; i < nw; i++) {
    outputQueues->at(i) = new WorkerQueue();
  }

  // generate input stream
  std::thread streamGenerator(Task::secureGenerator, inputStream, v);

  // emitter
  std::thread emitter(Task::secureEmitter, inputStream, inputQueues, v, nt);

  // workers
  Queue<std::thread *> threads;
  for (unsigned i = 0; i < nw; i++) {
    auto worker = new std::thread(Task::secureWorkerWithQueue, inputQueues->at(i), outputQueues->at(i), i, v);
    threads.push(worker);
  }

  // collector
  std::thread collector(Task::secureCollector, outputQueues, outputStream, v);

  // print the output stream
  std::thread outputPrinter(Task::securePrinter, outputStream, v);

  // join threads
  streamGenerator.join();
  emitter.join();
  inputStream->awakeAll();
  for (unsigned i = 0; i < nw; i++)
    threads.pop()->join();
  collector.join();
  outputPrinter.join();

  // return time spent for autopilot
  return timer.getTime();
}