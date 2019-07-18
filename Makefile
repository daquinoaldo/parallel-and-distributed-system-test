.PHONY: build debug infer infer-debug clean clean-debug clean-all auto

build: clean
	cmake -B cmake-build
	make -C cmake-build
	cp cmake-build/skyline skyline

debug: clean-debug
	cmake -B cmake-build-debug -DCMAKE_BUILD_TYPE=Debug
	make -C cmake-build-debug
	cp cmake-build-debug/skyline skyline-debug

infer: clean
	infer -- cmake -B cmake-build

infer-debug: clean
	infer -- cmake -B cmake-build-debug -DCMAKE_BUILD_TYPE=Debug

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