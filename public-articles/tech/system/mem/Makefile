CXX= g++ -std=c++17
CXXFLAGS= -g -Wall -Wextra $(MYCXXFLAGS)
LDFLAGS= $(MYLDFLAGS)
LIBS= $(MYLIBS)

MYLDFLAGS=
MYLIBS= -pthread -lbenchmark

RM= rm -f

TEST_T= false_sharing
TEST_O= false_sharing.o

# Targets start here

t: $(TEST_T)

$(TEST_T): $(TEST_O)
	$(CXX) -o $@ $(LDFLAGS) $(TEST_O) $(LIBS)

echo:
	@echo "CXX= $(CXX)"
	@echo "CXXFLAGS= $(CFLAGS)"
	@echo "LDFLAGS= $(LDFLAGS)"
	@echo "LIBS= $(LIBS)"
	@echo "RM= $(RM)"

clean:
	$(RM) $(TEST_T) $(TEST_O)


.PHONY: test clean

false_sharing.o: false_sharing.cc
