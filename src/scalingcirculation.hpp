#ifndef SCALINGCIRCULATION_HPP
#define SCALINGCIRCULATION_HPP
#include <cstdint>
#include <algorithm>
#include "vector.hpp"
#include "staticheap.hpp"

class ScalingCirculation {
private:
	struct Edge {
		int from, to;
		int64_t capacity, cost;
		int id;
	};
	static const int64_t INFINITE = std::numeric_limits<int64_t>::max();
	int vertices;
	nstd::Vector<nstd::Vector<int> > g;
	nstd::Vector<int64_t> potentials;
	nstd::Vector<nstd::Vector<int> > toAugment;
	nstd::Vector<int> used;
	nstd::Vector<int64_t> dist;
	nstd::StaticHeap dijkstra;
	nstd::Vector<int> fromP;
	void augmentEdge(int edge);
	void fixPotentials();
public:
	nstd::Vector<Edge> edges;
	void addEdge(int from, int to, int64_t capacity, int64_t cost, int id = 0);
	int64_t findMinCostFlow();
	ScalingCirculation(int vertices_);
};
#endif