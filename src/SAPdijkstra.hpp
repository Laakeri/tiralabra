#ifndef SAPDIJKSTRA_HPP
#define SAPDIJKSTRA_HPP
#include <cstdint>
#include <algorithm>
#include "vector.hpp"

class MinCostFlowSAPDijkstra {
private:
	struct Edge {
		int from, to;
		int64_t capacity, cost;
		int id;
	};
	static const int64_t INFINITE = std::numeric_limits<int64_t>::max();
	int vertices, source, sink;
	bool normalized;
	nstd::Vector<Edge> edges;
	nstd::Vector<nstd::Vector<int> > g;
	nstd::Vector<int64_t> potentials;
	void normalize();
	int64_t augment(int x, int64_t flow, const nstd::Vector<int>& from);
	std::pair<int64_t, int64_t> findAugmentingPath(int64_t maxAugment);
public:
	nstd::Vector<int64_t> getSolution();
	void addEdge(int from, int to, int64_t capacity, int64_t cost, int id = 0);
	std::pair<int64_t, int64_t> pushFlow(int64_t flowAmount);
	MinCostFlowSAPDijkstra(int vertices_, int source_, int sink_);
};
#endif