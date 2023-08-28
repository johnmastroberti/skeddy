build: builddir
	cmake --build build

builddir: CMakeLists.txt Makefile
	mkdir -p build
	cmake -S . -B build -G "Ninja" -D CMAKE_CXX_COMPILER=g++-13 -D CMAKE_PREFIX_PATH=./deps/or-tools

clean:
	rm -rf build

run: build
	./build/skeddy

test: build
	./build/tests

.PHONY: build builddir clean run test
