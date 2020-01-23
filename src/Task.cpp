#include "Task.hpp"
#include "Timer.hpp"

void Task::_generator(Stream *stream, bool verbose) {
  auto l = stream->getL();
  auto t = stream->getT();
  for (long i = 0; i < l; i++) {
    auto tuple = Utils::newTuple(t);
    stream->push(tuple);
    if (verbose)
      std::cout << "[Generator]\tTuple " + std::to_string(i) + ":\t" + Utils::serializeTuple(tuple) + "\n";
  }
}

unsigned long Task::generator(Stream *stream, bool verbose) {
  Timer timer("Generator");
  _generator(stream, verbose);
  return timer.getTime();
}

unsigned long Task::secureGenerator(SecureStream *stream, bool verbose) {
  Timer timer("SecureGenerator");
  _generator(stream, verbose);
  stream->setEOS();
  return timer.getTime();
}

unsigned long Task::secureEmitter(Stream *inputStream, std::vector<WorkerQueue*> *inputQueues, bool verbose) {
  Timer timer("SecureEmitter");
  unsigned long nextWorker = -1;
  int wIndex;
  Window window;
  // pick first window
  std::tie(wIndex, window) = inputStream->getWindow();
  while (!window.empty()) {
    // pick next worker
    nextWorker = (nextWorker + 1) % inputQueues->size();
    WorkerQueue *workerQueue = inputQueues->at(nextWorker);
    // push the window
    workerQueue->push(std::pair(wIndex, window));
    // log
    if (verbose)
      std::cout << "[SecureEmitter]\tAssigned window " + std::to_string(wIndex) + " to worker :" +
        std::to_string(nextWorker) + "\n";
    // pick next window
    std::tie(wIndex, window) = inputStream->getWindow();
  }
  // set EOQ in all the queues
  for (unsigned long i = 0; i < inputQueues->size(); i++)
    inputQueues->at(i)->setEOQ();
  return timer.getTime();
}

unsigned long Task::worker(Stream *inputStream, Queue<std::pair<int, Skyline>> *outputStream, bool verbose) {
  Timer timer("Worker");
  int wIndex;
  Window window;
  std::tie(wIndex, window) = inputStream->getWindow();
  while (!window.empty()) {
    auto skyline = Utils::processWindow(window);
    if (verbose) {
      std::cout << "[Worker]\tWindow " + std::to_string(wIndex) + ":\t" + Utils::serializeWindow(window) + "\n";
      std::cout << "[Worker]\tSkyline " + std::to_string(wIndex) + ":\t" + Utils::serializeWindow(skyline) + "\n";
    }
    outputStream->push(std::pair(wIndex, skyline));
    std::tie(wIndex, window) = inputStream->getWindow();
  }
  return timer.getTime();
}

unsigned long Task::secureWorker(SecureStream *inputStream, SecureQueue<std::pair<int, Skyline>> *outputStream,
    int wid, bool verbose) {
  Timer timer("SecureWorker" + std::to_string(wid));
  int wIndex;
  Window window;
  std::tie(wIndex, window) = inputStream->getWindow();
  while (!window.empty()) {
    if (verbose)
      std::cout << "[Worker " + std::to_string(wid) + "]\tWindow " + std::to_string(wIndex) + ":\t" +
                 Utils::serializeWindow(window) + "\n";
    auto skyline = Utils::processWindow(window);
    outputStream->push(std::pair(wIndex, skyline));
    if (verbose)
      std::cout << "[Worker " + std::to_string(wid) + "]\tSkyline " + std::to_string(wIndex) + ":\t" +
        Utils::serializeWindow(skyline) + "\n";
    std::tie(wIndex, window) = inputStream->getWindow();
  }
  outputStream->setEOQ();
  return timer.getTime();
}

unsigned long Task::secureWorkerWithQueue(WorkerQueue *inputQueue, WorkerQueue *outputQueue, int wid, bool verbose) {
  Timer timer("SecureWorkerWithQueue" + std::to_string(wid));
  int wIndex;
  Window window;
  std::tie(wIndex, window) = inputQueue->pop();
  //TODO: maybe inputQueue->empty() is enough and must be done before the pop. Must be change also in other places.
  while (!inputQueue->empty() || !window.empty()) {
    if (verbose)
      std::cout << "[Worker " + std::to_string(wid) + "]\tWindow " + std::to_string(wIndex) + ":\t" +
        Utils::serializeWindow(window) + "\n";
    auto skyline = Utils::processWindow(window);
    outputQueue->push(std::pair(wIndex, skyline));
    if (verbose)
      std::cout << "[Worker " + std::to_string(wid) + "]\tSkyline " + std::to_string(wIndex) + ":\t" +
        Utils::serializeWindow(skyline) + "\n";
    std::tie(wIndex, window) = inputQueue->pop();
  }
  outputQueue->setEOQ();
  return timer.getTime();
}

unsigned long Task::secureCollector(std::vector<WorkerQueue*> *outputQueues, SecureQueue<std::pair<int, Skyline>> *outputStream, bool verbose) {
  Timer timer("SecureCollector");
  unsigned long nextWorker = -1;
  unsigned long finishedWorkersCount = 0;
  int sIndex;
  Skyline skyline;
  while (finishedWorkersCount < outputQueues->size()) {
    // pick next worker
    nextWorker = (nextWorker + 1) % outputQueues->size();
    WorkerQueue *workerQueue = outputQueues->at(nextWorker);
    if (workerQueue == NULL) continue; // finished queue, skip
    if (!workerQueue->empty() || !skyline.empty()) {
      // pop a skyline from the worker window
      std::tie(sIndex, skyline) = workerQueue->pop();
      // push the window in the output stream
      outputStream->push(std::pair(sIndex, skyline));
      // log
      if (verbose)
        std::cout << "[SecureCollector]\tPicked skyline " + std::to_string(sIndex) + " from worker :" +
          std::to_string(nextWorker) + "\n";
    } else {
      outputQueues->at(nextWorker) = NULL;
      finishedWorkersCount++;
    }
  }
  outputStream->setEOQ();
  return timer.getTime();
}


unsigned long Task::printer(Queue<std::pair<int, Skyline>> *outputStream, bool verbose) {
  Timer timer("Printer");
  int sIndex;
  Skyline skyline;
  std::tie(sIndex, skyline) = outputStream->pop();
  while (!outputStream->empty() || !skyline.empty()) {
    if (verbose)
      std::cout << "[Printer]\tSkyline " + std::to_string(sIndex) + ":\t" + Utils::serializeWindow(skyline) + "\n";
    std::tie(sIndex, skyline) = outputStream->pop();
  }
  return timer.getTime();
}

unsigned long Task::securePrinter(SecureQueue<std::pair<int, Skyline>> *outputStream, bool verbose) {
  Timer timer("SecurePrinter");
  int sIndex;
  Skyline skyline;
  std::tie(sIndex, skyline) = outputStream->pop();
  while (!outputStream->empty() || !skyline.empty()) {
    if (verbose)
      std::cout << "[Printer]\tSkyline " + std::to_string(sIndex) + ":\t" + Utils::serializeWindow(skyline) + "\n";
    std::tie(sIndex, skyline) = outputStream->pop();
  }
  return timer.getTime();
}

