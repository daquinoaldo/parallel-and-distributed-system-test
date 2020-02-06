#cmake_minimum_required(VERSION 3.14)
project(skyline)

# Global flags
set(CMAKE_CXX_STANDARD 17)                               # New feature
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")       # Multithread
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive")   # FastFlow
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)                    # Infer

# Compiler warning flags in debug mode
set(GCC_W_ALL_LINUX "-Wall -Wextra -pedantic -Wshadow -Walloc-zero -Wduplicated-branches -Wduplicated-cond -Wconversion -Wmissing-declarations -Wmissing-include-dirs -Wnull-dereference -Wsuggest-final-methods -Wsuggest-final-types")
set(GCC_W_ALL_DARWIN "-Wall -Wextra -pedantic -Wshadow -Wconversion -Wmissing-declarations -Wmissing-include-dirs -Wnull-dereference")
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${GCC_W_ALL_DARWIN}")
else ()
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${GCC_W_ALL_LINUX}")
endif ()

# Debug flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

# Release
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# Profiler
set(CMAKE_CXX_FLAGS_PROFILE "${CMAKE_CXX_FLAGS_RELEASE} -pg")

# Defaults to Release build type
if(NOT CMAKE_BUILD_TYPE)	
  set(CMAKE_BUILD_TYPE Release)	
endif()	

add_executable(skyline
  src/main.cpp
  src/sequential.hpp
  src/parallel.hpp
  src/fastflow.hpp
  src/types.hpp
  src/Utils.hpp
  src/Utils.cpp
  src/Timer.cpp
  src/Timer.hpp
  src/Queue.hpp
)