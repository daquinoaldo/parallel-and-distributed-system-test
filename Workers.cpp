#include "Workers.hpp"

void Workers::generate(Stream *stream) {
  auto l = stream->getL();
  auto t = stream->getT();
  for (long i = 0; i < l; i++) {
    auto tuple = Utils::newTuple(t);
    stream->push(tuple);
    std::cout << "[Generator]\tTuple " + std::to_string(i) + ":\t" + Utils::serializeTuple(tuple) + "\n";
  }
}

void Workers::secureGenerate(SecureStream *stream) {
  generate(stream);
  stream->setEOS();
}

void Workers::work(Stream *inputStream, Queue<std::pair<int, Skyline>> *outputStream) {
  int wIndex;
  Window window;
  std::tie(wIndex, window) = inputStream->getWindow();
  while (!window.empty()) {
    auto skyline = Utils::processWindow(window);
    std::cout << "[Worker]\tWindow " + std::to_string(wIndex) + ":\t" + Utils::serializeWindow(window) + "\n";
    std::cout << "[Worker]\tSkyline " + std::to_string(wIndex) + ":\t" + Utils::serializeWindow(skyline) + "\n";
    outputStream->push(std::pair(wIndex, skyline));
    std::tie(wIndex, window) = inputStream->getWindow();
  }
}

void Workers::secureWork(SecureStream *inputStream, SecureQueue<std::pair<int, Skyline>> *outputStream, int wid) {
  int wIndex;
  Window window;
  std::tie(wIndex, window) = inputStream->getWindow();
  while (!window.empty()) {
    std::cout << "[Worker " + std::to_string(wid) + "]\tWindow " + std::to_string(wIndex) + ":\t" +
                 Utils::serializeWindow(window) + "\n";
    auto skyline = Utils::processWindow(window);
    outputStream->push(std::pair(wIndex, skyline));
    std::cout << "[Worker " + std::to_string(wid) + "]\tSkyline " + std::to_string(wIndex) + ":\t" +
                 Utils::serializeWindow(skyline) + "\n";
    std::tie(wIndex, window) = inputStream->getWindow();
  }
  outputStream->setEOQ();

}

void Workers::print(Queue<std::pair<int, Skyline>> *outputStream) {
  int sIndex;
  Skyline skyline;
  std::tie(sIndex, skyline) = outputStream->pop();
  while (!skyline.empty()) {
    std::cout << "[Printer]\tSkyline " + std::to_string(sIndex) + ":\t" + Utils::serializeWindow(skyline) + "\n";
    std::tie(sIndex, skyline) = outputStream->pop();
  }
}

void Workers::securePrint(SecureQueue<std::pair<int, Skyline>> *outputStream) {
  int sIndex;
  Skyline skyline;
  std::tie(sIndex, skyline) = outputStream->pop();
  while (!skyline.empty()) {
    std::cout << "[Printer]\tSkyline " + std::to_string(sIndex) + ":\t" + Utils::serializeWindow(skyline) + "\n";
    std::tie(sIndex, skyline) = outputStream->pop();
  }
}

