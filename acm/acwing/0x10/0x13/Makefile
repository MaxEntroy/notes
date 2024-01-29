CXX= g++ -std=c++17
CXXFLAGS= -g -O2 -Wall -Wextra
LDFLAGS=
LIBS= 

CORE_O= main.o

ALL_T= main
ALL_O= $(CORE_O)

# Targets start here.

t: $(ALL_T)

o: $(ALL_O)

$(ALL_T): $(ALL_O)
	$(CXX) -o $@ $(LDFLAGS) $(ALL_O) $(LIBS)

clean:
	rm -rf $(ALL_O) $(ALL_T)

.PHONY: clean o t

main.o: main.cc 
