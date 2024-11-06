CXX = g++
CXXFLAGS = -g -std=c++20 -Wall -pedantic
LD = g++
LDFLAGS =

SRC=$(wildcard *.cpp)
HDR=$(wildcard *.hpp)
OBJ=$(patsubst %.cpp, %.o, $(SRC))

all: fitExcel run

# $@ = target (self)
# $^ = target dependencies
fitExcel: $(OBJ)
	$(LD) $(LDFLAGS) $^ -o $@ -L./x86_64-linux-gnu -lexpression_parser

run: fitExcel
	./fitExcel

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include Makefile.d

Makefile.d: $(SRC) $(HDR)
	$(CXX) -MM $(SRC) > $@

clean:
	rm -f fitExcel *.o *.d
