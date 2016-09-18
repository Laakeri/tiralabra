CC = g++
CFLAGS = -std=c++11 -O2 -Wall -Wextra -Wshadow -g

all: SAPdijkstra.o staticheap.o

SAPdijkstra.o: SAPdijkstra.cpp SAPdijkstra.hpp vector.hpp
	$(CC) $(CFLAGS) SAPdijkstra.cpp -c

staticheap.o: staticheap.cpp staticheap.hpp vector.hpp
	$(CC) $(CFLAGS) staticheap.cpp -c

clean:
	rm *.o