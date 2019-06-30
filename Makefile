monitor : main.o util.o
	g++ -std="c++17" -o monitor main.o util.o -lncurses

main.o : main.cpp
	cc -c main.cpp

util.o : util.cpp
	cc -c util.cpp

clean :
	rm monitor main.o util.o