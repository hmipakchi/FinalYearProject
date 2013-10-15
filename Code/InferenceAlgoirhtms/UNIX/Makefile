#
# Makefile for non-Microsoft compilers
#

## Linux  (uncomment the 2 lines below for compilation on Linux)
#CXXFLAGS += -std=c++98 -Wall -ggdb
#LDFLAGS += 

## CygWin (uncomment the 2 lines below for compilation on CygWin)
CXXFLAGS += -Wall
LDFLAGS += 

all: test_greedy test_generate_network
 
opt: CXXFLAGS += -O4
opt: LDFLAGS += -O4
opt: test_greedy test_generate_network

# COMPILE

test_greedy:	test_greedy.cpp Snap.o
	g++ $(LDFLAGS) -o test_greedy test_greedy.cpp test_cascade_inference.cpp Snap.o -I./glib -I./snap

test_generate_network:	test_generate_network.cpp Snap.o
	g++ $(LDFLAGS) -o test_generate_network test_generate_network.cpp test_cascade_inference.cpp Snap.o -I./glib -I./snap
	
Snap.o: 
	g++ -c $(CXXFLAGS) ./snap/Snap.cpp -I./glib -I./snap

clean:
	rm -f   test_greedy test_generate_network Snap.o  $(MAIN).exe test_generate_network.exe test_greedy.exe
