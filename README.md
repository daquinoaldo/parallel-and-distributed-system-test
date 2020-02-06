# Skyline query
_C++ Skyline implementation for the Parallel and Distributed Systems course @ Pisa University, a.y 2018/19_

## Compile
```bash
make           # compile with -o3
make debug     # activate all warnings and compile with -g
make profile   # compile with -o3 and -pg
make clean     # remove all build files for a totally cleaned build
```

## Run
```bash
./skyline      # runs in automatic mode
./skyline -h   # print the help message with parameters and explanation
```

## Helpers
```bash
make valgrind  # run valgrind on the last compiled executable (for memory leak)
make gprof     # run gprof on the last compiled executable (to reason about where the most time is spent)
make infer     # run infer on the last compiled executable (static analysis)

# Tip: you can pass arguments to the helpers
make ARGS="parallel 42 100 ..." valgrind # you obtain valgrind ./skyline parallel 42 100 ...
```

## Troubleshooting
Requires CMake at least version 3.14 and CC and C++ at least version 7.3.  

With CMake < 3.14 this project can be compiled manually as follows.
```bash
cmake .
make
```

If CMake picks the wrong compilers, them can be forced as follows.
```bash
cmake -DCMAKE_C_COMPILER=<cc/path> -DCMAKE_CXX_COMPILER=<c++/path> .
make
```

## Compile on the Intel Xeon Phi
```bash
source /home/daquino-spm19/.bashrc
cmake -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ .
make
```