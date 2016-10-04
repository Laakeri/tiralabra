#include "../../src/SAPSPFA.hpp"
#include "../../src/SAPdijkstra.hpp"
#include "../../src/scalingcirculation.hpp"
#include "../../src/vector.hpp"
#include <bits/stdc++.h>
#define F first
#define S second

class Timer {
public:
	bool timing;
	std::chrono::duration<double> elapsedTime;
	std::chrono::time_point<std::chrono::system_clock> startTime;

	Timer() {
		timing = false;
		elapsedTime = std::chrono::duration<double>(std::chrono::duration_values<double>::zero());
	}
	void start() {
		if (timing) return;
		timing = true;
		startTime = std::chrono::system_clock::now();
	}
	void stop() {
		if (!timing) return;
		timing = false;
		std::chrono::time_point<std::chrono::system_clock> endTime = std::chrono::system_clock::now();
		elapsedTime += (endTime - startTime);
	}
	std::chrono::duration<double> getTime() {
		if (timing) {
			stop();
			std::chrono::duration<double> ret = elapsedTime;
			start();
			return ret;
		}
		else {
			return elapsedTime;
		}
	}
};

std::mt19937 rng(1337);

int64_t getRand(int64_t lo, int64_t hi) {
	return std::uniform_int_distribution<int64_t>(lo, hi)(rng);
}

std::vector<std::pair<std::pair<int, int>, std::pair<int64_t, int64_t> > > generateGraph(int n, int m, int64_t maxCapacity, int64_t maxCost) {
	assert(n > 1);
	std::vector<std::pair<std::pair<int, int>, std::pair<int64_t, int64_t> > > ret(m);
	for (int i = 0; i < m; i++) {
		int a = getRand(1, n);
		int b = a;
		while (a == b) b = getRand(1, n);
		ret[i] = {{a, b}, {getRand(1, maxCapacity), getRand(1, maxCost)}};
	}
	return ret;
}

std::pair<double, std::pair<int64_t, int64_t> > testSAPSPFA(int n, const std::vector<std::pair<std::pair<int, int>, std::pair<int64_t, int64_t> > >& edges) {
	Timer timer;
	timer.start();
	MinCostFlowSAPSPFA mf(n, 1, n);
	int64_t capSum = 0;
	for (auto e : edges) {
		mf.addEdge(e.F.F, e.F.S, e.S.F, e.S.S);
		capSum += e.S.F;
	}
	auto sol = mf.pushFlow(capSum);
	timer.stop();
	return {timer.getTime().count(), sol};
}

std::pair<double, std::pair<int64_t, int64_t> > testSAPdijkstra(int n, const std::vector<std::pair<std::pair<int, int>, std::pair<int64_t, int64_t> > >& edges) {
	Timer timer;
	timer.start();
	MinCostFlowSAPDijkstra mf(n, 1, n);
	int64_t capSum = 0;
	for (auto e : edges) {
		mf.addEdge(e.F.F, e.F.S, e.S.F, e.S.S);
		capSum += e.S.F;
	}
	auto sol = mf.pushFlow(capSum);
	timer.stop();
	return {timer.getTime().count(), sol};
}

std::pair<double, std::pair<int64_t, int64_t> > testScalingCirculation(int n, const std::vector<std::pair<std::pair<int, int>, std::pair<int64_t, int64_t> > >& edges) {
	Timer timer;
	timer.start();
	ScalingCirculation mf(n);
	int64_t capSum = 0;
	for (auto e : edges) {
		mf.addEdge(e.F.F, e.F.S, e.S.F, e.S.S);
		capSum += e.S.F;
	}
	const int64_t BIG = 1e5;
	mf.addEdge(n, 1, BIG, -BIG);
	int64_t cost = mf.findMinCostFlow();
	int64_t flow = 0;
	while (cost < 0) {
		cost += BIG;
		flow++;
	}
	std::pair<int64_t, int64_t> sol = {flow, cost};
	timer.stop();
	return {timer.getTime().count(), sol};
}

void testSparseUnitCap(int n) {
	auto G = generateGraph(n, getRand(4*n, 10*n), 1, 100);
	for (int i = 0; i < n; i++) {
		G.push_back({{1, getRand(2, n)}, {1, getRand(1, 100)}});
	}
	for (int i = 0; i < n; i++) {
		G.push_back({{getRand(1, n-1), n}, {1, getRand(1, 100)}});
	}
	auto t1 = testSAPSPFA(n, G);
	auto t2 = testSAPdijkstra(n, G);
	if (t1.S != t2.S) {
		std::cout<<"fail"<<std::endl;
		abort();
	}
	std::cout<<t1.F<<" "<<t2.F<<" "<<t1.S.F<<" "<<t1.S.S<<std::endl;
}

void testSparse(int n) {
	auto G = generateGraph(n, getRand(2*n, 5*n), 10, 10);
	for (int i = 0; i < n; i++) {
		G.push_back({{1, getRand(2, n)}, {getRand(1, 10), getRand(1, 10)}});
	}
	for (int i = 0; i < n; i++) {
		G.push_back({{getRand(1, n-1), n}, {getRand(1, 10), getRand(1, 10)}});
	}
	auto t1 = testSAPSPFA(n, G);
	auto t2 = testSAPdijkstra(n, G);
	auto t3 = testScalingCirculation(n, G);
	if (t1.S != t2.S || t2.S != t3.S) {
		std::cout<<"fail "<<t1.S.F<<" "<<t1.S.S<<" "<<t3.S.F<<" "<<t3.S.S<<std::endl;
		abort();
	}
	std::cout<<t1.F<<" "<<t2.F<<" "<<t3.F<<" "<<t1.S.F<<" "<<t1.S.S<<std::endl;
}

int main() {
	for (int i=0;i<100000000;i++){
		testSparse(300);
	}
}