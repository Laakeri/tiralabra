#include <iostream>
#include <cstdlib>
#include "SAPdijkstra.hpp"
#include "SAPSPFA.hpp"
#include "scalingcirculation.hpp"

void nie() {
	std::cout<<"Invalid input"<<std::endl;
	abort();
}

int main(int argc, char** argv){
	if (argc != 2) {
		std::cout<<"Give algorithm to use as an argument. Use:"<<std::endl;
		std::cout<<"SAPSPFA"<<std::endl;
		std::cout<<"SAPDijkstra"<<std::endl;
		std::cout<<"ScalingCirculation"<<std::endl;
		abort();
	}
	std::string algo(argv[1]);
	if (algo != "SAPSPFA" && algo != "SAPDijkstra" && algo != "ScalingCirculation") {
		std::cout<<"Give algorithm to use as an argument. Use:"<<std::endl;
		std::cout<<"SAPSPFA"<<std::endl;
		std::cout<<"SAPDijkstra"<<std::endl;
		std::cout<<"ScalingCirculation"<<std::endl;
		abort();
	}
	int n,m;
	if (!(std::cin>>n>>m)) nie();
	if (n<0 || m<0) nie();
	if (algo == "SAPSPFA") {
		int64_t source, sink, k;
		if (!(std::cin>>source>>sink>>k)) nie();
		if (k<0 || source <= 0 || source > n || sink <= 0 || sink > n || source == sink) nie();
		MinCostFlowSAPSPFA mcmf(n, source, sink);
		for (int i = 0; i < m; i++) {
			int a, b;
			int64_t capacity, cost;
			if (!(std::cin>>a>>b>>capacity>>cost)) nie();
			mcmf.addEdge(a, b, capacity, cost, i);
		}
		auto ans = mcmf.pushFlow(k);
		std::cout<<"Flow: "<<ans.first<<std::endl;
		std::cout<<"Cost: "<<ans.second<<std::endl;
		nstd::Vector<int64_t> sol = mcmf.getSolution();
		for (int i = 0; i < m; i++) {
			std::cout<<sol[i]<<std::endl;
		}
	}
	if (algo == "SAPDijkstra") {
		int64_t source, sink, k;
		if (!(std::cin>>source>>sink>>k)) nie();
		if (k<0 || source <= 0 || source > n || sink <= 0 || sink > n || source == sink) nie();
		MinCostFlowSAPDijkstra mcmf(n, source, sink);
		for (int i = 0; i < m; i++) {
			int a, b;
			int64_t capacity, cost;
			if (!(std::cin>>a>>b>>capacity>>cost)) nie();
			mcmf.addEdge(a, b, capacity, cost, i);
		}
		auto ans = mcmf.pushFlow(k);
		std::cout<<"Flow: "<<ans.first<<std::endl;
		std::cout<<"Cost: "<<ans.second<<std::endl;
		nstd::Vector<int64_t> sol = mcmf.getSolution();
		for (int i = 0; i < m; i++) {
			std::cout<<sol[i]<<std::endl;
		}
	}
	if (algo == "ScalingCirculation") {
		ScalingCirculation circu(n);
		for (int i = 0; i < m; i++) {
			int a, b;
			int64_t capacity, cost;
			if (!(std::cin>>a>>b>>capacity>>cost)) nie();
			circu.addEdge(a, b, capacity, cost, i);
		}
		int64_t ans = circu.findMinCostFlow();
		std::cout<<"Cost: "<<ans<<std::endl;
		nstd::Vector<int64_t> sol = circu.getSolution();
		for (int i = 0; i < m; i++) {
			std::cout<<sol[i]<<std::endl;
		}
	}
}