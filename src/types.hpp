#ifndef SKYLINE_TYPES_HPP
#define SKYLINE_TYPES_HPP


#include <vector>
#include <cmath>
#include <iostream>

class Tuple: public std::vector<int> {
public:
  Tuple(const ulong &__a): std::vector<int>(__a) {
    for (ulong i = 0; i < size(); i++)
      at(i) = rand() % 100;
  }
};


class Window: public std::vector<Tuple*> {
private:
  const long i;
public:
  Window(const ulong &__a, const long &id): std::vector<Tuple*>(__a), i(id) {}
  long id() { return i; }
};

typedef Window Skyline;


class Stream: public std::vector<Tuple*> {

private:
  const ulong w;
  const ulong k;
  ulong w_num;

public:
  Stream(const ulong &__a, const ulong &w_size, const ulong &t_size, const ulong &sliding_f):
        std::vector<Tuple*>(__a), w(w_size), k(sliding_f), w_num(floor((size() - w) / k) + 1) {
    // fill the stream with tuple
    for (ulong i = 0; i < size(); i++)
      at(i) = new Tuple(t_size);
  }

  ~Stream() {
    for (ulong i = 0; i < size(); i++)
      delete at(i);
  }

  ulong w_size() { return w; }
  ulong sliding_f() { return k; }
  ulong w_no() { return w_num; }

  Window* pickWindow(ulong w_i) {
    auto window = new Window(w, w_i);
    auto start = w_i * k;
    for (ulong i = 0; i < w; i++) {
      auto tuple = at(start + i);
      window->at(i) = tuple;
    }
    return window;
  }
};


#endif //SKYLINE_TYPES_HPP
