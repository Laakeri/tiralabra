#include <cstdint>
#include <algorithm>
#include "staticheap.hpp"

namespace nstd {

void StaticHeap::putValue(int64_t value, int index) {
	data[index + size] = value;
	for (index = (index + size)/2; index; index /= 2) {
		data[index] = std::min(data[index*2], data[index*2 + 1]);
	}
}

std::pair<int64_t, int> StaticHeap::pop() {
	int index = 1;
	while (index < size) {
		if (data[index*2] <= data[index*2 + 1]) index = index*2;
		else index = index*2 + 1;
	}
	std::pair<int64_t, int> ret = {data[index], index - size};
	putValue(INFINITE, index - size);
	return ret;
}

StaticHeap::StaticHeap(int size_) : size(size_){
	int rSize = 1;
	while (rSize < size) rSize *= 2;
	size = rSize;
	data = new int64_t[size * 2];
	for (int i = 0; i < size * 2; i++) data[i] = INFINITE;
}

StaticHeap::~StaticHeap() {
	delete[] data;
}

}