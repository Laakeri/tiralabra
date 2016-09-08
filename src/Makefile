CC = g++
CFLAGS = -std=c++11 -O2 -Wall -Wextra -Wshadow -g

all: SAPdijkstra

SAPdijkstra: staticheap.o
	$(CC) $(CFLAGS) SAPdijkstra.cpp -c -o SAPdijkstra.o

staticheap.o:
	$(CC) $(CFLAGS) staticheap.cpp -c -o staticheap.o

clean:
	rm *.o