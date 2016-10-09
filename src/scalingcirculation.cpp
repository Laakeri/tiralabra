#include "scalingcirculation.hpp"
#include "vector.hpp"
#include "staticheap.hpp"
#include <limits>
#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <iostream>

void ScalingCirculation::addEdge(int from, int to, int64_t capacity, int64_t cost, int id) {
	for (int64_t b = 0; ((int64_t)1<<b) <= capacity; b++) {
		if ((int64_t)toAugment.size() <= b) toAugment.push_back(nstd::Vector<int>());
		if (capacity & ((int64_t)1<<b)) {
			toAugment[b].push_back((int)edges.size());
		}
	}
	edges.push_back({from, to, 0, cost, id});
	edges.push_back({to, from, 0, -cost, id});
	g[from].push_back((int)edges.size() - 2);
	g[to].push_back((int)edges.size() - 1);
}

void ScalingCirculation::augmentEdge(int edge) {
	edges[edge].capacity++;
	if (edges[edge].capacity > 1) return; // Case 1
	int64_t pViolate = potentials[edges[edge].from] + edges[edge].cost - potentials[edges[edge].to];
	if (pViolate >= 0) return;
	nstd::StaticHeap dijkstra(vertices + 1);
	nstd::Vector<int> used(vertices + 1);
	nstd::Vector<int> from(vertices + 1);
	nstd::Vector<int64_t> dist(vertices + 1);
	for (int i = 1; i <= vertices; i++) {
		dist[i] = INFINITE;
	}
	dist[edges[edge].to] = 0;
	dijkstra.putValue(0, edges[edge].to);
	int64_t negCycle = 0;
	while (1) {
		std::pair<int64_t, int> vertexD = dijkstra.pop();
		if (vertexD.first == nstd::StaticHeap::INFINITE) break;
		int vertex = vertexD.second;
		if (used[vertex]) continue;
		used[vertex] = true;
		for (int edgeId : g[vertex]) {
			Edge e = edges[edgeId];
			int64_t newDistance = dist[vertex] + e.cost + potentials[vertex] - potentials[e.to];
			if (e.capacity > 0 && newDistance < dist[e.to]) {
				if (e.to == edges[edge].to) {
					negCycle = newDistance;
					continue;
				}
				dist[e.to] = newDistance;
				from[e.to] = edgeId;
				dijkstra.putValue(newDistance, e.to);
			}
		}
	}
	if (negCycle == 0) { // Case 2/3
		for (int i = 1; i <= vertices; i++) {
			if (used[i]) potentials[i] += pViolate + dist[i];
		}
	}
	else { // Case 4
		for (int i = 1; i <= vertices; i++) {
			if (used[i]) potentials[i] += dist[i];
		}
		edges[edge].capacity--;
		edges[edge^1].capacity++;
		int vertex = edges[edge].from;
		while (vertex != edges[edge].to) {
			edges[from[vertex]].capacity--;
			edges[from[vertex]^1].capacity++;
			vertex = edges[from[vertex]].from;
		}
	}
	// Fix potentials of other vertices
	int64_t maxDist = 0;
	for (int i = 1; i <= vertices; i++) {
		if (used[i]) maxDist = std::max(maxDist, dist[i]);
	}
	nstd::Vector<int> fixQueue;
	size_t queueFront = 0;
	for (int i = 1; i <= vertices; i++) {
		if (used[i]) {
			for (int edgeId : g[i]) {
				Edge e = edges[edgeId^1];
				if (e.capacity > 0 && potentials[e.to] > potentials[e.from] + e.cost) {
					fixQueue.push_back(e.from);
				}
			}
		}
	}
	while (queueFront < fixQueue.size()) {
		int v = fixQueue[queueFront++];
		if (used[v]) continue;
		used[v] = 1;
		potentials[v] += maxDist;
		for (int edgeId : g[v]) {
			Edge e = edges[edgeId];
			if (e.capacity > 0 && potentials[e.to] > potentials[e.from] + e.cost) {
				if (used[e.from]) abort();
				fixQueue.push_back(e.from);
			}
			e = edges[edgeId^1];
			if (e.capacity > 0 && potentials[e.to] > potentials[e.from] + e.cost) {
				if (used[e.from]) abort();
				fixQueue.push_back(e.from);
			}
		}
	}
	for (int i = 1; i <= vertices; i++) {
		std::cout<<i<<" "<<potentials[i]<<std::endl;
		for (int edgeId : g[i]) {
			Edge e = edges[edgeId];
			if (e.capacity > 0 && potentials[e.to] > potentials[e.from] + e.cost) {
				std::cout<<"fail"<<std::endl;
				abort();
			}
		}
	}
}

int64_t ScalingCirculation::findMinCostFlow() {
	for (int b = (int)toAugment.size() - 1; b >= 0; b--){
		for (int i = 0; i < (int)edges.size(); i++) {
			edges[i].capacity *= 2;
		}
		for (int edge : toAugment[b]) {
			augmentEdge(edge);
		}
	}
	int64_t cost = 0;
	for (int i = 1; i < (int)edges.size(); i += 2) {
		cost -= edges[i].cost * edges[i].capacity;
	}
	return cost;
}

ScalingCirculation::ScalingCirculation(int vertices_) :
vertices(vertices_), g(vertices + 1), potentials(vertices + 1) {
}
/*
int main(){
	ScalingCirculation lol(8);
	lol.addEdge(5, 3, 1, 1);
	lol.addEdge(1, 5, 2, 3);
	lol.addEdge(3, 8, 1, 2);
	lol.addEdge(5, 7, 2, 2);
	lol.addEdge(5, 8, 2, 5);
	lol.addEdge(7, 2, 2, 3);
	lol.addEdge(2, 8, 2, 2);
	lol.addEdge(1, 2, 1, 4);
	lol.addEdge(8, 1, 8, -50);
	std::cout<<lol.findMinCostFlow()<<std::endl;
}*/
