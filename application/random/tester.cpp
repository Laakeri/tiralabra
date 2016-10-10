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

std::pair<std::pair<int, int>, std::vector<std::pair<std::pair<int, int>, std::pair<int64_t, int64_t> > > > generatePathologicalGraph(int k, int& ns) {
	assert(k>=1);
	if (k==1){
		int s=ns++;
		int v1=ns++;
		int v2=ns++;
		int t=ns++;
		std::vector<std::pair<std::pair<int, int>, std::pair<int64_t, int64_t> > > es;
		es.push_back({{v1, v2}, {1, -1}});
		es.push_back({{s, v1}, {1, 1}});
		es.push_back({{s, v2}, {1, 1}});
		es.push_back({{v1, t}, {1, 1}});
		es.push_back({{v2, t}, {1, 1}});
		return {{s, t}, es};
	}
	else{
		int s=ns++;
		auto G = generatePathologicalGraph(k-1, ns);
		int t=ns++;
		int64_t c = ((int64_t)1 << (int64_t)k);
		G.S.push_back({{s, G.F.F}, {c, -c}});
		G.S.push_back({{s, G.F.S}, {c, -c}});
		G.S.push_back({{G.F.F, t}, {c, -c}});
		G.S.push_back({{G.F.S, t}, {c, -c}});
		return {{s, t}, G.S};
	}
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
	const int64_t BIG1 = 1e10;
	const int64_t BIG2 = 1e8;
	mf.addEdge(n, 1, BIG1, -BIG2, 1);
	int64_t cost = mf.findMinCostFlow();
	int64_t flow = 0;
	for (int i = 0; i < (int)mf.edges.size(); i++) {
		if (mf.edges[i].id == 1 && mf.edges[i].to == n) {
			flow = mf.edges[i].capacity;
			cost += flow * BIG2;
		}
	}
	std::pair<int64_t, int64_t> sol = {flow, cost};
	timer.stop();
	return {timer.getTime().count(), sol};
}

double tt1,tt2,tt3;

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

void testDense(int n) {
	auto G = generateGraph(n, getRand(n*n, 2*n*n), 1000, 1000);
	for (int i = 0; i < n; i++) {
		G.push_back({{1, getRand(2, n)}, {getRand(1, 1000), getRand(1, 1000)}});
	}
	for (int i = 0; i < n; i++) {
		G.push_back({{getRand(1, n-1), n}, {getRand(1, 1000), getRand(1, 1000)}});
	}
	auto t1 = testSAPSPFA(n, G);
	auto t2 = testSAPdijkstra(n, G);
	auto t3 = testScalingCirculation(n, G);
	if (t1.S != t2.S || t2.S != t3.S) {
		std::cout<<"fail "<<t1.S.F<<" "<<t1.S.S<<" "<<t3.S.F<<" "<<t3.S.S<<std::endl;
		abort();
	}
	std::cout<<t1.F<<" "<<t2.F<<" "<<t3.F<<" "<<t1.S.F<<" "<<t1.S.S<<std::endl;
	tt1+=t1.F;
	tt2+=t2.F;
	tt3+=t3.F;
}

void testSparse(int n) {
	auto G = generateGraph(n, getRand(2*n, 5*n), 1000, 1000);
	for (int i = 0; i < n; i++) {
		G.push_back({{1, getRand(2, n)}, {getRand(1, 1000), getRand(-5, 1000)}});
	}
	for (int i = 0; i < n; i++) {
		G.push_back({{getRand(1, n-1), n}, {getRand(1, 1000), getRand(-5, 1000)}});
	}
	auto t1 = testSAPSPFA(n, G);
	auto t2 = testSAPdijkstra(n, G);
	auto t3 = testScalingCirculation(n, G);
	if (t1.S != t2.S || t2.S != t3.S) {
		std::cout<<"fail "<<t1.S.F<<" "<<t1.S.S<<" "<<t3.S.F<<" "<<t3.S.S<<std::endl;
		abort();
	}
	std::cout<<t1.F<<" "<<t2.F<<" "<<t3.F<<" "<<t1.S.F<<" "<<t1.S.S<<std::endl;
	tt1+=t1.F;
	tt2+=t2.F;
	tt3+=t3.F;
}

void testPathological(int k) {
	int n=1;
	auto G = generatePathologicalGraph(k, n);
	n--;
	auto t1 = testSAPSPFA(n, G.S);
	auto t2 = testSAPdijkstra(n, G.S);
	auto t3 = testScalingCirculation(n, G.S);
	if (t1.S != t2.S || t2.S != t3.S) {
		std::cout<<"fail "<<t1.S.F<<" "<<t1.S.S<<" "<<t3.S.F<<" "<<t3.S.S<<std::endl;
		abort();
	}
	std::cout<<t1.F<<" "<<t2.F<<" "<<t3.F<<" "<<t1.S.F<<" "<<t1.S.S<<std::endl;
}

int main() {
	std::ofstream out("Densetest.csv");
	out<<"n, SAPSPFA, SAPdijkstra, ScalingCirculation"<<std::endl;
	out<<std::setprecision(2)<<std::fixed;
	int tcs=10;
	for (int i=10;i<=250;i+=10){
		tt1=0;
		tt2=0;
		tt3=0;
		for (int t=0;t<tcs;t++){
			testDense(i);
		}
		tt1/=(double)tcs;
		tt2/=(double)tcs;
		tt3/=(double)tcs;
		out<<i<<","<<tt1<<","<<tt2<<","<<tt3<<std::endl;
	}
}