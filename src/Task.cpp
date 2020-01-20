#include "Task.hpp"
#include "Timer.hpp"

unsigned long Task::emitter(Stream *stream, bool verbose) {
  Timer timer("Emitter");
  auto l = stream->getL();
  auto t = stream->getT();
  for (long i = 0; i < l; i++) {
    auto tuple = Utils::newTuple(t);
    stream->push(tuple);
    if (verbose)
      std::cout << "[Emitter]\tTuple " + std::to_string(i) + ":\t" + Utils::serializeTuple(tuple) + "\n";
  }
  return timer.getTime();
}

unsigned long Task::secureEmitter(SecureStream *stream, bool verbose) {
  emitter(stream, verbose);
  stream->setEOS();
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
  Timer timer("Worker");
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

unsigned long Task::collector(Queue<std::pair<int, Skyline>> *outputStream, bool verbose) {
  Timer timer("Collector");
  int sIndex;
  Skyline skyline;
  std::tie(sIndex, skyline) = outputStream->pop();
  while (!outputStream->empty()) {
    if (verbose)
      std::cout << "[Collector]\tSkyline " + std::to_string(sIndex) + ":\t" + Utils::serializeWindow(skyline) + "\n";
    std::tie(sIndex, skyline) = outputStream->pop();
  }
  return timer.getTime();
}

unsigned long Task::secureCollector(SecureQueue<std::pair<int, Skyline>> *outputStream, bool verbose) {
  Timer timer("Collector");
  int sIndex;
  Skyline skyline;
  std::tie(sIndex, skyline) = outputStream->pop();
  while (!outputStream->getEOQ()) {
    if (verbose)
      std::cout << "[Collector]\tSkyline " + std::to_string(sIndex) + ":\t" + Utils::serializeWindow(skyline) + "\n";
    std::tie(sIndex, skyline) = outputStream->pop();
  }
  return timer.getTime();
}

