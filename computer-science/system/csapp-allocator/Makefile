CXX= g++ -std=c++17 -Wall -Wextra
CXXFLAGS= -g -O3
LDFLAGS=
LIBS= -pthread 

ALL_T= main
ALL_O= main.o simple_allocator.o

RM= rm -rf

# Targets start here

t: $(ALL_T)

$(ALL_T): $(ALL_O)
	$(CXX) -o $@ $(LDFLAGS) $(ALL_O) $(LIBS)

clean:
	$(RM) $(ALL_T) $(ALL_O)

.PHONY: t clean

main.o: main.cc simple_allocator.h
simple_allocator.o: simple_allocator.cc simple_allocator.h
