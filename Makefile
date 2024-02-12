CXXFLAGS    +=  -W -Wextra -Wall -Werror -ansi -pedantic
CXXFLAGS    +=  -pthread --std=c++11 -Iincludes -Ilibs/c++ -I/usr/local/opt/ncurses/include
LDFLAGS     +=  -lncurses -L/usr/local/opt/ncurses/lib

# existing Makefile targets go here

.PHONY: all
all: format build

.PHONY: format
format:
	clang-format src/* include/* -i

.PHONY: build
build:
	mkdir -p build
	cd build && \
	cmake .. && \
	make

.PHONY: debug
debug:
	mkdir -p build
	cd build && \
	cmake -DCMAKE_BUILD_TYPE=debug .. && \
	make

.PHONY: clean
clean:
	rm -rf build

