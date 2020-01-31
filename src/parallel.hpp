#include <sstream>
#include <thread>
#include "Timer.hpp"
#include "Utils.hpp"
#include "Queue.hpp"

long parallel(Stream* inputStream, bool v, unsigned nw);


long parallel(Stream* inputStream, bool v, unsigned nw) {
  Timer timer("Parallel");

  Queue<Window*> inputQueue;
  Queue<Window*> outputQueue;
  auto EOQ = new Window(0, -1);
  auto isEOQ = [](Window* window) { return window->id() == -1; };

  // emitter
  auto emitter = new std::thread([&v, &inputStream, &inputQueue, &EOQ, &nw]() {
    for (ulong i = 0; i < inputStream->w_no(); i++) {
      auto window = inputStream->pickWindow(i);
      if (v) {
        std::stringstream message;
        message << "[Emitter] Window " << i << ": " << Utils::serializeWindow(window) << std::endl;
        std::cout << message.str();
      }
      inputQueue.push(window);
    }
    for (ulong i = 0; i < nw; i++)
      inputQueue.push(EOQ);
  });

  // workers
  Queue<std::thread *> workers;
  for (ulong i = 0; i < nw; i++) {
    auto worker = new std::thread([&v, &inputQueue, &outputQueue, &EOQ, &isEOQ, &i]() {
      auto window = inputQueue.pop();
      while (!isEOQ(window)) {
        if (v) {
          std::stringstream message;
          message << "[Worker" << i << "] Window " << window->id() << ": " << Utils::serializeWindow(window) << std::endl;
          std::cout << message.str();
        }

        auto skyline = Utils::processWindow(window);

        if (v) {
          std::stringstream message;
          message << "[Worker" << i << "] Skyline " << skyline->id() << ": " << Utils::serializeWindow(skyline) << std::endl;
          std::cout << message.str();
        }

        outputQueue.push(skyline);
        window = inputQueue.pop();
      }
      outputQueue.push(EOQ);
    });
    workers.push(worker);
  }

  // collector
  auto collector = new std::thread([&v, &outputQueue, &EOQ, &isEOQ, &nw]() {
    auto EOQ_count = 0;
    auto skyline = outputQueue.pop();
    while (EOQ_count < nw) {
      if (!isEOQ(skyline)) {
        EOQ_count++;
        continue;
      }
      if (v) {
        std::stringstream message;
        message << "[Collector] Skyline " << skyline->id() << ": " << Utils::serializeWindow(skyline) << std::endl;
        std::cout << message.str();
      }
      delete skyline;
      skyline = outputQueue.pop();
    }
  });


  // join threads
  emitter->join();
  for (unsigned i = 0; i < nw; i++) {
    auto worker = workers.pop();
    worker->join();
    delete worker;
  }
  collector->join();

  return timer.getTime();
}