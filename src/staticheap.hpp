#ifndef NSTD_STATICHEAP_HPP
#define NSTD_STATICHEAP_HPP
#include <cstdint>
#include <limits>
#include <algorithm>

namespace nstd {

class StaticHeap {
private:
	int64_t* data;
	int size;
public:
	static const int64_t INFINITE = std::numeric_limits<int64_t>::max();
	std::pair<int64_t, int> pop();
	void putValue(int64_t value, int pos);
	StaticHeap(int size_);
	~StaticHeap();
};

}
#endif