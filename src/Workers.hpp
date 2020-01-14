#ifndef SKYLINE_WORKERS_HPP
#define SKYLINE_WORKERS_HPP


#include <iostream>
#include "Utils.hpp"
#include "SecureStream.hpp"
#include "SecureQueue.hpp"

class Workers {

public:
  static void generate(Stream *stream, bool verbose);

  static void secureGenerate(SecureStream *stream, bool verbose);

  static void work(Stream *inputStream, Queue<std::pair<int, Skyline>> *outputStream, bool verbose);

  static void secureWork(SecureStream *inputStream, SecureQueue<std::pair<int, Skyline>> *outputStream,
    int wid, bool verbose);

  static void print(Queue<std::pair<int, Skyline>> *outputStream, bool verbose);

  static void securePrint(SecureQueue<std::pair<int, Skyline>> *outputStream, bool verbose);

};


#endif //SKYLINE_WORKERS_HPP
