release: CMakeLists.txt Makefile
	mkdir -p build-release
	cmake -S . -B build -G "Ninja" -D CMAKE_BUILD_TYPE=Release -D CMAKE_CXX_COMPILER=g++-13 -D CMAKE_PREFIX_PATH=./deps/or-tools
	cmake --build build

debug: CMakeLists.txt Makefile
	mkdir -p build-debug
	cmake -S . -B build -G "Ninja" -D CMAKE_BUILD_TYPE=Debug -D CMAKE_CXX_COMPILER=g++-13 -D CMAKE_PREFIX_PATH=./deps/or-tools
	cmake --build build

clean:
	rm -rf build-release
	rm -rf build-debug

run: build
	./build/skeddy

drun: build
	gdb ./build/skeddy

test: build
	./build/tests

.PHONY: release debug clean run drun test
