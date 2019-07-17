#include "SecureStream.hpp"

SecureStream::SecureStream(int t, int w, int k, long l, unsigned int seed) : Stream(t, w, k, l, seed) {}

std::pair<int, Window> SecureStream::getWindow() {
  std::unique_lock<std::mutex> lock(mutex);
  condition.wait(lock, [=] { return Stream::size() > (unsigned long) getW() || EOS; });
  return Stream::getWindow();
}

void SecureStream::push(const Tuple &item) {
  // unique lock will be released when leave this block
  {
    std::unique_lock<std::mutex> lock(mutex);
    Stream::push(item);
  }
  condition.notify_one();
}

unsigned long SecureStream::size() {
  std::unique_lock<std::mutex> lock(mutex);
  return Stream::size();
}

void SecureStream::setEOS() {
  EOS = true;
}

void SecureStream::awakeAll() {
  condition.notify_all();
}
