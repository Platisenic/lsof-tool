CXX = g++
CFLAGS = -g -std=c++11
EXEC = hw1

.PHONY: all run clean

all: $(EXEC)

$(EXEC): $(EXEC).o lsof.o 
	$(CXX) $(CFLAGS) -o $@ $^

lsof.o: lsof.cpp lsof.hpp
	$(CXX) $(CFLAGS) -c -o $@ $<

$(EXEC).o: $(EXEC).cpp lsof.hpp
	$(CXX) $(CFLAGS) -c -o $@ $<

run: all
	./$(EXEC)

clean: 
	rm -f $(EXEC) *.o

