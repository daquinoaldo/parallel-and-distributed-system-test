.PHONY: build debug profile valgrind infer clean

build:
	cmake -B build
	make -C build
	cp build/skyline skyline

debug:
	cmake -B build -DCMAKE_BUILD_TYPE=Debug
	make -C build
	cp build/skyline skyline

profile:
	# http://sam.zoy.org/writings/programming/gprof.html
	gcc -shared -fPIC gprof-helper.c -o gprof-helper.so -lpthread -ldl
	cmake -B build -DCMAKE_BUILD_TYPE=Profile
	make -C build
	cp build/skyline skyline

valgrind:
	valgrind --leak-check=full \
					 --show-leak-kinds=all \
					 --track-origins=yes \
					 --verbose \
					 --log-file=valgrind-out.txt \
					 ./skyline ${ARGS}

infer:
	infer run --compilation-database build/compile_commands.json --keep-going

gprof:
	# http://sam.zoy.org/writings/programming/gprof.html
	LD_PRELOAD=./gprof-helper.so ./skyline ${ARGS}
	gprof skyline > gprof-out.txt

clean:
	rm -rf build
	rm -f skyline
	rm -f gmon.out
	rm -f gprof-helper.so
	rm -f gprof-out.txt
	rm -f valgrind-out.txt
	rm -rf infer-out