#ifndef SKYLINE_TYPES_HPP
#define SKYLINE_TYPES_HPP


#include <vector>
#include "SecureQueue.hpp"

typedef std::vector<int> Tuple;
typedef std::vector<Tuple> Window;
typedef std::vector<Tuple> Skyline;
typedef SecureQueue<std::pair<int, Window>> WorkerQueue;


#endif //SKYLINE_TYPES_HPP
