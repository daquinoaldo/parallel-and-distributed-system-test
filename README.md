# Skyline query
C++ Skyline implementation for the Parallel and Distributed Systems course @ UniPi

## Compile and run
```
cmake -B cmake-build
make -C cmake-build
cmake-build/skyline
```

**Debug:** a more verbose and readable version, less significant in terms of performance,
to check if things work as expected.
```
cmake -B cmake-build-debug -DCMAKE_BUILD_TYPE=Debug
make -C cmake-build-debug
cmake-build-debug/skyline
```

**Recompile:** before recompiling remove the build folder.
```
rm -rf cmake-build
rm -rf cmake-build-debug
```

**Static code analysis:** run Facebook Infer on the code looking for memory issues.
```
infer -- cmake -B cmake-build
```