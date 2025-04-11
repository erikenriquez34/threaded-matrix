CXX := g++
CXXFLAGS := -std=c++17 -pthread -g -MMD

DEPEND := transpose.cpp
TARGETS := main multithreaded-assign singlethread-oblivious singlethread-sequential

all: $(TARGETS)

main: main.cpp $(SOURCES)
	$(CXX) $(CXXFLAGS) $^ -o $@

multithreaded-assign: multithreaded-assign.cpp $(DEPEND)
	$(CXX) $(CXXFLAGS) $^ -o $@

singlethread-oblivious: singlethread-oblivious.cpp $(DEPEND)
	$(CXX) $(CXXFLAGS) $^ -o $@

singlethread-sequential: singlethread-sequential.cpp $(DEPEND)
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -f $(TARGETS) *.o *.d

-include *.d
