CC = g++
CFLAGS = -std=c++11 -O2 -Wall -Wextra -Wshadow -g

all: SAPSPFA.o SAPdijkstra.o staticheap.o

SAPSPFA.o: SAPSPFA.cpp SAPSPFA.hpp vector.hpp
	$(CC) $(CFLAGS) SAPSPFA.cpp -c

SAPdijkstra.o: SAPdijkstra.cpp SAPdijkstra.hpp vector.hpp
	$(CC) $(CFLAGS) SAPdijkstra.cpp -c

staticheap.o: staticheap.cpp staticheap.hpp vector.hpp
	$(CC) $(CFLAGS) staticheap.cpp -c

clean:
	rm *.o