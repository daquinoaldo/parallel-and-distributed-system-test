cmake_minimum_required(VERSION 3.14)
project(skyline)

set(CMAKE_CXX_STANDARD 17)


set(GCC_W_SOME "-Wall -Wextra -pedantic -Wshadow -Walloc-zero -Wduplicated-branches -Wduplicated-cond -Wconversion -Wmissing-declarations -Wmissing-include-dirs -Wnull-dereference  -Wsuggest-final-methods -Wsuggest-final-types")
set(GCC_W_MIN "-Wall -Wextra -Wshadow -pedantic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_W_SOME}")

#file(GLOB SOURCE_FILES *.cpp)


add_executable(skyline main.cpp Queue.hpp Stream.cpp Stream.hpp)