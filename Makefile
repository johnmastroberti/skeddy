build: build-dir
	cmake --build build

build-dir: CMakeLists.txt Makefile
	mkdir -p build
	cmake -S . -B build -G "Ninja" -D CMAKE_CXX_COMPILER=g++-13 -D CMAKE_PREFIX_PATH=./or-tools

clean:
	rm -rf build

run: build
	./build/skeddy

.PHONY: build build-dir clean run
