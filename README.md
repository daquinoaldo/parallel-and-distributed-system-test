# Skyline query
_C++ Skyline implementation for the Parallel and Distributed Systems course @ UniPi_

## Compile and run

### Standard version
```bash
make
./skyline
```

### Debug version
Use this version to debug the behavior of the tool, not to test scalability.
- tuple components are number up to 100 instead of up to maxint
- gdb debugger can be attached (compiled with -g)
- all warnings are active at compile time
- more verbose logging, to verify the correctness of the computation
- less performance
```bash
make debug
./skyline-debug
```

### Infer analysis
```bash
make infer
# --- or ---
make infer-debug
```

### Autopilot
An _autopilot_ version is included. Automatically runs the tool with some parameters and shows timers and statistics.
```bash
make auto
# --- or separately ---
make
./skyline auto
```

### Troubleshooting
Requires CMake version 3.14 and cc and c++ version 7.3.  

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