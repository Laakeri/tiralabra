CC = g++
CFLAGS = -std=c++11 -O2 -Wall -Wextra -Wshadow -g

all: tester

tester: src tester.o
	$(CC) $(CFLAGS) tester.o ../../src/SAPdijkstra.o ../../src/SAPSPFA.o ../../src/staticheap.o -o tester

tester.o: tester.cpp
	$(CC) $(CFLAGS) -c tester.cpp

src:
	$(MAKE) -C ../../src all

clean:
	rm *.o