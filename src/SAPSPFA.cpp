#include "SAPSPFA.hpp"
#include "vector.hpp"
#include "staticheap.hpp"
#include <limits>
#include <algorithm>
#include <cstdint>
#include <iostream>

void MinCostFlowSAPSPFA::addEdge(int from, int to, int64_t capacity, int64_t cost, int id) {
	edges.push_back({from, to, capacity, cost, id});
	edges.push_back({to, from, 0, -cost, id});
	g[from].push_back((int)edges.size() - 2);
	g[to].push_back((int)edges.size() - 1);
}

// Augment flow along the found augmenting path
int64_t MinCostFlowSAPSPFA::augment(int vertex, int64_t flow, const nstd::Vector<int>& from) {
	if (vertex == source) return flow;
	int64_t augmentFlow = augment(edges[from[vertex]].from, std::min(flow, edges[from[vertex]].capacity), from);
	edges[from[vertex]].capacity -= augmentFlow;
	edges[from[vertex]^1].capacity += augmentFlow;
	return augmentFlow;
}

// Find a single augmenting path
std::pair<int64_t, int64_t> MinCostFlowSAPSPFA::findAugmentingPath(int64_t maxAugment) {
	nstd::Vector<int> from(vertices + 1);
	nstd::Vector<int64_t> dist(vertices + 1);
	nstd::Vector<int> inQueue(vertices + 1);
	for (int i = 1; i <= vertices; i++) {
		dist[i] = INFINITE;
	}
	dist[source] = 0;
	nstd::Vector<int> queue;
	size_t queueFront = 0;
	queue.push_back(source);
	// Use spfa
	while (queueFront < queue.size()) {
		int vertex = queue[queueFront++];
		inQueue[vertex] = false;
		for (int edgeId : g[vertex]) {
			Edge e = edges[edgeId];
			if (e.capacity > 0 && dist[vertex] + e.cost < dist[e.to]) {
				dist[e.to] = dist[vertex] + e.cost;
				from[e.to] = edgeId;
				if (!inQueue[e.to]) {
					inQueue[e.to] = true;
					queue.push_back(e.to);
				}
			}
		}
	}
	if (dist[sink] < INFINITE) {
		int64_t flow = augment(sink, maxAugment, from);
		return {flow, flow * dist[sink]};
	}
	else return {0, 0};
}

std::pair<int64_t, int64_t> MinCostFlowSAPSPFA::pushFlow(int64_t flowAmount) {
	int64_t flow = 0;
	int64_t cost = 0;
	// Increase flow until flowAmount or maxflow is found
	while (flowAmount > 0) {
		std::pair<int64_t, int64_t> fl = findAugmentingPath(flowAmount);
		flow += fl.first;
		flowAmount -= fl.first;
		cost += fl.second;
		if (fl.first == 0) break;
	}
	return {flow, cost};
}


MinCostFlowSAPSPFA::MinCostFlowSAPSPFA(int vertices_, int source_, int sink_) :
vertices(vertices_), source(source_), sink(sink_), g(vertices + 1) {
}