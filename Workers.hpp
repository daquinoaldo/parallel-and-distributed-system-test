#ifndef SKYLINE_WORKERS_HPP
#define SKYLINE_WORKERS_HPP


#include <iostream>
#include "Utils.hpp"
#include "SecureStream.hpp"
#include "SecureQueue.hpp"

class Workers {

public:
  static void generate(Stream *stream);

  static void secureGenerate(SecureStream *stream);

  static void work(Stream *inputStream, Queue<std::pair<int, Skyline>> *outputStream);

  static void secureWork(SecureStream *inputStream, SecureQueue<std::pair<int, Skyline>> *outputStream, int wid);

  static void print(Queue<std::pair<int, Skyline>> *outputStream);

  static void securePrint(SecureQueue<std::pair<int, Skyline>> *outputStream);

};


#endif //SKYLINE_WORKERS_HPP
