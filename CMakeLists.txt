#cmake_minimum_required(VERSION 3.14)
project(skyline)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS -pthread)

# Compiler warning flags
set(GCC_W_ALL_LINUX "-Wall -Wextra -pedantic -Wshadow -Walloc-zero -Wduplicated-branches -Wduplicated-cond -Wconversion -Wmissing-declarations -Wmissing-include-dirs -Wnull-dereference -Wsuggest-final-methods -Wsuggest-final-types")
set(GCC_W_ALL_DARWIN "-Wall -Wextra -pedantic -Wshadow -Wconversion -Wmissing-declarations -Wmissing-include-dirs -Wnull-dereference")
set(GCC_W_MIN "-Wall -Wextra -Wshadow -pedantic")
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${GCC_W_ALL_DARWIN}")
else ()
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${GCC_W_ALL_LINUX}")
endif ()

# Debug flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

add_executable(skyline src/main.cpp src/Utils.cpp src/Utils.hpp src/Stream.cpp src/Stream.hpp src/SecureQueue.hpp src/types.hpp src/SecureStream.cpp src/SecureStream.hpp src/Task.cpp src/Task.hpp src/Timer.cpp src/Timer.hpp)