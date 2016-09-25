CC = g++
CFLAGS = -std=c++11 -O2 -Wall -Wextra -Wshadow -g

all: main

main: src main.o
	$(CC) $(CFLAGS) main.o ../../src/SAPdijkstra.o ../../src/SAPSPFA.o ../../src/staticheap.o -o main

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

src:
	$(MAKE) -C ../../src all

clean:
	rm *.o