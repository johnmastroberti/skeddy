release: CMakeLists.txt Makefile
	mkdir -p build-release
	cmake -S . -B build-release -G "Ninja" -D CMAKE_BUILD_TYPE=Release -D CMAKE_CXX_COMPILER=g++-13 -D CMAKE_PREFIX_PATH=./deps/or-tools
	cmake --build build-release

debug: CMakeLists.txt Makefile
	mkdir -p build-debug
	cmake -S . -B build-debug -G "Ninja" -D CMAKE_BUILD_TYPE=Debug -D CMAKE_CXX_COMPILER=g++-13 -D CMAKE_PREFIX_PATH=./deps/or-tools
	cmake --build build-debug

clean:
	rm -rf build-release
	rm -rf build-debug

run: release
	./build-release/skeddy

drun: debug
	gdb ./build-debug/skeddy

test: build
	./build-release/tests

.PHONY: release debug clean run drun test
