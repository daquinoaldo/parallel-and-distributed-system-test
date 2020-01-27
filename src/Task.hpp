#ifndef SKYLINE_WORKERS_HPP
#define SKYLINE_WORKERS_HPP


#include <iostream>
#include "types.hpp"
#include "Utils.hpp"
#include "SecureStream.hpp"
#include "SecureQueue.hpp"

class Task {

private :
  static void _generator(Stream *stream, bool verbose);

public:
  static unsigned long generator(Stream *stream, bool verbose);

  static unsigned long secureGenerator(SecureStream *stream, bool verbose);

  static unsigned long secureEmitter(Stream *inputStream, std::vector<WorkerQueue*> *inputQueues, bool verbose, unsigned nt);

  static unsigned long worker(Stream *inputStream, Queue<std::pair<int, Skyline>> *outputStream, bool verbose);

  static unsigned long secureWorker(SecureStream *inputStream, SecureQueue<std::pair<int, Skyline>> *outputStream, int wid, bool verbose);

  static unsigned long secureWorkerWithQueue(WorkerQueue *inputQueue, WorkerQueue *outputQueue, int wid, bool verbose);

  static unsigned long secureCollector(std::vector<WorkerQueue*> *outputQueues, SecureQueue<std::pair<int, Skyline>> *outputStream, bool verbose);

  static unsigned long printer(Queue<std::pair<int, Skyline>> *outputStream, bool verbose);

  static unsigned long securePrinter(SecureQueue<std::pair<int, Skyline>> *outputStream, bool verbose);

};


#endif //SKYLINE_WORKERS_HPP
