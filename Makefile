CXXFLAGS = -std=c++17

objects = main.o util.o

monitor : $(objects)
	$(CXX) $(CXXFLAGS) -o monitor main.o util.o -lncurses

clean :
	rm monitor $(objects)