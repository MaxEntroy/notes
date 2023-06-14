CXX= g++ -std=c++17
CXXFLAGS= -g -O3 -Wall -Wextra
LDFLAGS=
LIBS= -pthread 

ALL_T= main
ALL_O= variatic_template.o

RM= rm -rf

# Targets start here

t: $(ALL_T)

$(ALL_T): $(ALL_O)
	$(CXX) -o $@ $(LDFLAGS) $(ALL_O) $(LIBS)

clean:
	$(RM) $(ALL_T) $(ALL_O)

.PHONY: t clean

variatic_template.o: variatic_template.cc
