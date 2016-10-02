#include "SAPdijkstra.hpp"
#include "vector.hpp"
#include "staticheap.hpp"
#include <limits>
#include <algorithm>
#include <cstdint>

void MinCostFlowSAPDijkstra::addEdge(int from, int to, int64_t capacity, int64_t cost, int id) {
	normalized = false;
	edges.push_back({from, to, capacity, cost, id});
	edges.push_back({to, from, 0, -cost, id});
	g[from].push_back((int)edges.size() - 2);
	g[to].push_back((int)edges.size() - 1);
}

void MinCostFlowSAPDijkstra::normalize() {
	if (normalized) return;
	normalized = true;
	nstd::Vector<bool> inQueue(vertices + 1);
	for (int i = 1; i <= vertices; i++) {
		potentials[i] = INFINITE;
	}
	potentials[source] = 0;
	nstd::Vector<int> queue;
	size_t queueFront = 0;
	queue.push_back(source);
	while (queueFront < queue.size()) {
		int vertex = queue[queueFront++];
		inQueue[vertex] = false;
		for (int edgeId : g[vertex]) {
			Edge e = edges[edgeId];
			if (e.capacity > 0 && potentials[vertex] + e.cost < potentials[e.to]) {
				potentials[e.to] = potentials[vertex] + e.cost;
				if (!inQueue[e.to]) {
					inQueue[e.to] = true;
					queue.push_back(e.to);
				}
			}
		}
	}
}

int64_t MinCostFlowSAPDijkstra::augment(int vertex, int64_t flow, const nstd::Vector<int>& from) {
	if (vertex == source) return flow;
	int64_t augmentFlow = augment(edges[from[vertex]].from, std::min(flow, edges[from[vertex]].capacity), from);
	edges[from[vertex]].capacity -= augmentFlow;
	edges[from[vertex]^1].capacity += augmentFlow;
	return augmentFlow;
}

std::pair<int64_t, int64_t> MinCostFlowSAPDijkstra::findAugmentingPath(int64_t maxAugment) {
	nstd::StaticHeap dijkstra(vertices + 1);
	nstd::Vector<int> used(vertices + 1);
	nstd::Vector<int> from(vertices + 1);
	nstd::Vector<int64_t> dist(vertices + 1);
	for (int i = 1; i <= vertices; i++) {
		dist[i] = INFINITE;
	}
	dist[source] = 0;
	dijkstra.putValue(0, source);
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
				dist[e.to] = newDistance;
				from[e.to] = edgeId;
				dijkstra.putValue(newDistance, e.to);
			}
		}
	}
	int64_t cost = dist[sink] + potentials[sink];
	for (int i = 1; i <= vertices; i++) {
		if (used[i]) potentials[i] += dist[i];
	}
	if (used[sink]) {
		int64_t flow = augment(sink, maxAugment, from);
		return {flow, flow * cost};
	}
	else return {0, 0};
}

std::pair<int64_t, int64_t> MinCostFlowSAPDijkstra::pushFlow(int64_t flowAmount) {
	int64_t flow = 0;
	int64_t cost = 0;
	normalize();
	while (flowAmount > 0) {
		std::pair<int64_t, int64_t> fl = findAugmentingPath(flowAmount);
		flow += fl.first;
		flowAmount -= fl.first;
		cost += fl.second;
		if (fl.first == 0) break;
	}
	return {flow, cost};
}


MinCostFlowSAPDijkstra::MinCostFlowSAPDijkstra(int vertices_, int source_, int sink_) :
vertices(vertices_), source(source_), sink(sink_), g(vertices + 1), potentials(vertices + 1) {
}