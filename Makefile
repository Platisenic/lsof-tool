CXX = g++
CFLAGS = -g -Wall -Werror -Wextra -std=c++11
EXEC = hw1
PACKNAME = 310552029_hw1

.PHONY: all run clean pack

all: $(EXEC)

$(EXEC): $(EXEC).o lsof.o inputParser.o utils.o
	$(CXX) $(CFLAGS) -o $@ $^

$(EXEC).o: $(EXEC).cpp lsof.hpp inputParser.hpp utils.hpp
	$(CXX) $(CFLAGS) -c -o $@ $<

lsof.o: lsof.cpp lsof.hpp utils.hpp
	$(CXX) $(CFLAGS) -c -o $@ $<

inputParser.o: inputParser.cpp inputParser.hpp
	$(CXX) $(CFLAGS) -c -o $@ $<

utils.o: utils.cpp utils.hpp
	$(CXX) $(CFLAGS) -c -o $@ $<

run: all
	./$(EXEC)

clean: 
	rm -rf $(EXEC) *.o $(PACKNAME) $(PACKNAME).zip

pack: clean
	mkdir $(PACKNAME)
	cp *.cpp *.hpp Makefile $(PACKNAME)
	zip -r $(PACKNAME).zip $(PACKNAME)
	rm -rf $(PACKNAME)


