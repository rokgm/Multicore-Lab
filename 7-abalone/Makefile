### Compiler
### Chosen compiler has to be in path (use "module" to setup environment)

# Use this for GCC (warning: GCC < 4.2.x does not support OpenMP)
# CXX = g++

# Use this for the Intel C++ compiler ("icc" is only the C version)
CXX = icpc

# Use this for MPI
#CXX = mpiCC


### Options

# Useful options for GCC
CXXFLAGS=-O3 -g -fno-omit-frame-pointer -fopenmp
LDFLAGS=


LIB_OBJS = move.o board.o network.o search.o eval.o
SEARCH_OBJS = $(LIB_OBJS) search-abid.o search-onelevel.o\
	search-minimax.o search-negamax.o search-negamax-parallel.o search-ab-seq.o search-ab-parallel.o

all: player start referee

player: player.o $(SEARCH_OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $< $(SEARCH_OBJS)

start: start.o $(LIB_OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $< $(LIB_OBJS)

referee: referee.o $(LIB_OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $< $(LIB_OBJS)

clean:
	rm -rf *.o *~ player start referee networktest

networktest: tests/networktest.o network.o
	$(CXX) -o networktest tests/networktest.o network.o

board.o: board.h board.cpp search.cpp
move.o: move.h move.cpp
network.o: network.h network.cpp
player.o: player.cpp 
search.o: search.cpp board.cpp move.cpp
eval.o: eval.cpp board.cpp
start.o: start.cpp board.cpp move.cpp
referee.o: referee.cpp board.cpp move.cpp
search-onelevel.o: search.h board.h eval.h
search-abid.o: search.h board.h
search-minimax.o: search.h board.h eval.h
search-negamax.o: search.h board.h eval.h
search-negamax-parallel.o: search.h board.h eval.h
search-ab-seq.o: search.h board.h eval.h
search-ab-parallel.o: search.h board.h eval.h
