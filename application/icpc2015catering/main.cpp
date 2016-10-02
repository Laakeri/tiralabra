#include "../../src/SAPSPFA.hpp"
#include "../../src/SAPdijkstra.hpp"
#include "../../src/vector.hpp"
#include <iostream>
#include <cstdint>

int main(){
	std::ios_base::sync_with_stdio(0);
	std::cin.tie(0);
	int n,k;
	std::cin>>n>>k;
	int source = 2*n + 9;
	int sink = 2*n + 10;
	MinCostFlowSAPSPFA mf(2*n + 10, source, sink);
	const int64_t inf = 1e18 / (int64_t)n;
	for (int i = 1; i <= n; i++) {
		mf.addEdge(i*2, i*2 + 1, 1, -inf);
		mf.addEdge(i*2 + 1, sink, inf, 0);
	}
	for (int i = 0; i < n; i++) {
		for (int j = 1; i + j <= n; j++) {
			int c;
			std::cin>>c;
			if (i == 0) mf.addEdge(source, (i+j)*2, 1, c);
			else mf.addEdge(i*2 + 1, (i+j)*2, 1, c);
		}
	}
	int64_t ans = 1e18;
	int64_t cost = 0;
	for (int i = 0; i < k; i++) {
		auto flow = mf.pushFlow(1);
		cost += flow.second;
		ans = std::min(ans, cost + inf*(int64_t)n);
	}
	std::cout<<ans<<std::endl;
}