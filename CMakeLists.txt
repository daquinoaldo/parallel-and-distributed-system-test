#cmake_minimum_required(VERSION 3.14)
project(skyline)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-pthread -fpermissive")

# Compiler warning flags
set(GCC_W_ALL_LINUX "-Wall -Wextra -pedantic -Wshadow -Walloc-zero -Wduplicated-branches -Wduplicated-cond -Wconversion -Wmissing-declarations -Wmissing-include-dirs -Wnull-dereference -Wsuggest-final-methods -Wsuggest-final-types")
set(GCC_W_ALL_DARWIN "-Wall -Wextra -pedantic -Wshadow -Wconversion -Wmissing-declarations -Wmissing-include-dirs -Wnull-dereference")
set(GCC_W_MIN "-Wall -Wextra -Wshadow -pedantic")
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${GCC_W_ALL_DARWIN}")
else ()
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${GCC_W_ALL_LINUX}")
endif ()

# Debug flags for all
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG -g")
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -pg")  # profiler

if(NOT CMAKE_BUILD_TYPE)	
  set(CMAKE_BUILD_TYPE Release)	
endif()	
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/usr/local/include/ff")

add_executable(skyline
  src/main.cpp
  src/fastflow.hpp
  src/types.hpp
  src/Utils.hpp
  src/Utils.cpp
  src/Timer.cpp
  src/Timer.hpp
  src/Queue.hpp
)