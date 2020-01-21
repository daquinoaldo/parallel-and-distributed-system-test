#ifndef SKYLINE_WORKERS_HPP
#define SKYLINE_WORKERS_HPP


#include <iostream>
#include "Utils.hpp"
#include "SecureStream.hpp"
#include "SecureQueue.hpp"

class Task {

private :
  static void _emitter(Stream *stream, bool verbose);

public:
  static unsigned long emitter(Stream *stream, bool verbose);

  static unsigned long secureEmitter(SecureStream *stream, bool verbose);

  static unsigned long worker(Stream *inputStream, Queue<std::pair<int, Skyline>> *outputStream, bool verbose);

  static unsigned long secureWorker(SecureStream *inputStream, SecureQueue<std::pair<int, Skyline>> *outputStream,
    int wid, bool verbose);

  static unsigned long collector(Queue<std::pair<int, Skyline>> *outputStream, bool verbose);

  static unsigned long secureCollector(SecureQueue<std::pair<int, Skyline>> *outputStream, bool verbose);

};


#endif //SKYLINE_WORKERS_HPP
