#ifndef SKYLINE_SECURESTREAM_HPP
#define SKYLINE_SECURESTREAM_HPP


#include <mutex>
#include <condition_variable>
#include "Stream.hpp"

class SecureStream : public Stream {

private:
  std::mutex mutex;
  std::condition_variable condition;
  bool EOS = false;

public:
  SecureStream(int t, int w, int k, long l, unsigned int seed);

  void push(const Tuple &item) override;

  std::pair<int, Window> getWindow() override;

  unsigned long size() override;

  void setEOS();

  void awakeAll();
};


#endif //SKYLINE_SECURESTREAM_HPP
