#include "scalingcirculation.hpp"
#include "vector.hpp"
#include "staticheap.hpp"
#include <limits>
#include <algorithm>
#include <cstdint>
#include <cstdlib>

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

// Fix potentials to minimal potentials with dijkstra
void ScalingCirculation::fixPotentials() {
	nstd::Vector<int64_t> newPotentials(vertices + 1);
	for (int i = 1; i <= vertices; i++) {
		used[i] = 0;
		dist[i] = INFINITE;
	}
	for (int i = 1; i <= vertices; i++) {
		if (used[i] == 0) {
			nstd::Vector<int> visited;
			dijkstra.putValue(0, i);
			dist[i] = 0;
			int64_t potAdd = 0;
			while (1) {
				std::pair<int64_t, int> vertexD = dijkstra.pop();
				if (vertexD.first == nstd::StaticHeap::INFINITE) break;
				int vertex = vertexD.second;
				if (used[vertex]) continue;
				used[vertex] = 1;
				visited.push_back(vertex);
				for (int edgeId : g[vertex]) {
					Edge e = edges[edgeId];
					int64_t newDistance = dist[vertex] + e.cost + potentials[vertex] - potentials[e.to];
					if (e.capacity > 0 && used[e.to] == 0 && newDistance < dist[e.to]) {
						dist[e.to] = newDistance;
						dijkstra.putValue(newDistance, e.to);
					}
				}
			}
			for (int v : visited) {
				if (v == i) newPotentials[v] = 0;
				else newPotentials[v] = dist[v] - potentials[i] + potentials[v];
				for (int edgeId : g[v]) {
					Edge e = edges[edgeId];
					if (used[e.to] == 2 && e.capacity > 0) {
						potAdd = std::max(potAdd, newPotentials[e.to] - e.cost - newPotentials[v]);
					}
				}
			}
			for (int v : visited) {
				newPotentials[v] += potAdd;
				used[v] = 2;
			}
		}
	}
	potentials = newPotentials;
}

// Increment capacity of edge
void ScalingCirculation::augmentEdge(int edge) {
	edges[edge].capacity++;
	if (edges[edge].capacity > 1) return; // Case 1
	int64_t pViolate = potentials[edges[edge].from] + edges[edge].cost - potentials[edges[edge].to];
	if (pViolate >= 0) return;
	for (int i = 1; i <= vertices; i++) {
		used[i] = 0;
		dist[i] = INFINITE;
		fromP[i] = 0;
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
				fromP[e.to] = edgeId;
				dijkstra.putValue(newDistance, e.to);
			}
		}
	}
	int64_t maxDist = 0;
	for (int i = 1; i <= vertices; i++) {
		if (used[i]) maxDist = std::max(maxDist, dist[i]);
	}
	if (negCycle == 0) { // Case 2/3
		for (int i = 1; i <= vertices; i++) {
			if (used[i]) potentials[i] += pViolate + dist[i];
			else potentials[i] += maxDist;
		}
	}
	else { // Case 4
		for (int i = 1; i <= vertices; i++) {
			if (used[i]) potentials[i] += dist[i];
			else potentials[i] += maxDist;
		}
		edges[edge].capacity--;
		edges[edge^1].capacity++;
		int vertex = edges[edge].from;
		while (vertex != edges[edge].to) {
			edges[fromP[vertex]].capacity--;
			edges[fromP[vertex]^1].capacity++;
			vertex = edges[fromP[vertex]].from;
		}
	}
	// Fix potentials in order to avoid overflow
	fixPotentials();
}

int64_t ScalingCirculation::findMinCostFlow() {
	for (int b = (int)toAugment.size() - 1; b >= 0; b--){
		// Scaling
		for (int i = 0; i < (int)edges.size(); i++) {
			edges[i].capacity *= 2;
		}
		// Increment
		for (int edge : toAugment[b]) {
			augmentEdge(edge);
		}
	}
	// Compute the final cost
	int64_t cost = 0;
	for (int i = 1; i < (int)edges.size(); i += 2) {
		cost -= edges[i].cost * edges[i].capacity;
	}
	return cost;
}

nstd::Vector<int64_t> ScalingCirculation::getSolution() {
	nstd::Vector<int64_t> ret(edges.size());
	for (unsigned i = 0; i < edges.size(); i+=2) {
		ret[i/2] = edges[i^1].capacity;
	}
	return ret;
}

ScalingCirculation::ScalingCirculation(int vertices_) :
vertices(vertices_), g(vertices + 1), potentials(vertices + 1), used(vertices + 1), dist(vertices + 1), dijkstra(vertices + 1), fromP(vertices + 1) {
}