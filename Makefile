.PHONY: build debug valgrind valgrind-debug infer infer-debug clean clean-debug clean-all auto

build: clean
	cmake -B cmake-build -DCMAKE_EXPORT_COMPILE_COMMANDS=1
	make -C cmake-build
	cp cmake-build/skyline skyline

debug: clean-debug
	cmake -B cmake-build-debug -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=1
	make -C cmake-build-debug
	cp cmake-build-debug/skyline skyline-debug

valgrind: build
	valgrind --leak-check=full \
					--show-leak-kinds=all \
					--track-origins=yes \
					--verbose \
					--log-file=valgrind-out.txt \
					./skyline

valgrind-debug: debug
	valgrind --leak-check=full \
					--show-leak-kinds=all \
					--track-origins=yes \
					--verbose \
					--log-file=valgrind-out.txt \
					./skyline-debug #parallel 42 3 2 1 5

infer: build
	infer run --compilation-database cmake-build/compile_commands.json --keep-going

infer-debug: debug
		infer run --compilation-database cmake-build-debug/compile_commands.json --keep-going

clean:
	rm -rf cmake-build
	rm -f skyline

clean-debug:
	rm -rf cmake-build-debug
	rm -f skyline-debug

clean-all: clean clean-debug
	rm -rf infer-out

auto: build
	./skyline auto