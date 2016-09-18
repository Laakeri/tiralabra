#ifndef NSTD_VECTOR_HPP
#define NSTD_VECTOR_HPP
// Implementation of STL style Vector
#include <stdlib.h>
#include <stdexcept>

namespace nstd {

template<class T>
class VectorIterator;

template<class T>
class Vector {
private:
	T* container;
	size_t vectorSize;
	size_t containerSize;
	void destructElements();
	void destructBack();
public:
	T& operator[](size_t pos);
	const T& operator[](size_t pos) const;
	void reserve(size_t newSize);
	void push_back(const T& value);
	size_t size();
	void clear();
	Vector(size_t newSize);
	Vector();
	~Vector();
	VectorIterator<T> begin() const;
	VectorIterator<T> end() const;
};

template<class T>
class VectorIterator {
private:
	const Vector<T>* vec;
	size_t pos;
public:
	VectorIterator (const Vector<T>* vec_, size_t pos_);
	bool operator!= (const VectorIterator& other) const;
	T operator* () const;
	const VectorIterator& operator++ ();
};

template<class T>
Vector<T>::Vector() {
	container = nullptr;
	vectorSize = 0;
	containerSize = 0;
}

template<class T>
Vector<T>::Vector(size_t newSize) : Vector() {
	reserve(newSize);
	vectorSize = newSize;
	new (container) T[newSize]();
}

template<class T>
void Vector<T>::destructBack() {
	container[--vectorSize].~T();
}

template<class T>
void Vector<T>::destructElements() {
	while (vectorSize) destructBack();
}

template<class T>
Vector<T>::~Vector() {
	clear();
}

template<class T>
T& Vector<T>::operator[](size_t pos) {
	return container[pos];
}

template<class T>
const T& Vector<T>::operator[](size_t pos) const {
	return container[pos];
}

template<class T>
void Vector<T>::reserve(size_t newSize) {
	if (newSize <= containerSize) return;
	newSize = std::max(newSize, containerSize * 2 + 1);
	container = (T*)realloc(container, newSize * sizeof(T));
	if (container == nullptr) throw std::runtime_error("Bad alloc");
	containerSize = newSize;
}

template<class T>
void Vector<T>::push_back(const T& elem) {
	reserve(vectorSize + 1);
	container[vectorSize++] = elem;
}

template<class T>
void Vector<T>::clear() {
	destructElements();
	free(container);
	container = nullptr;
	vectorSize = 0;
	containerSize = 0;
}

template<class T>
size_t Vector<T>::size() {
	return vectorSize;
}

template<class T>
VectorIterator<T> Vector<T>::begin() const {
	return VectorIterator<T>(this, 0);
}

template<class T>
VectorIterator<T> Vector<T>::end() const {
	return VectorIterator<T>(this, vectorSize);
}

template<class T>
VectorIterator<T>::VectorIterator (const Vector<T>* vec_, size_t pos_) : vec(vec_), pos(pos_) {}

template<class T>
bool VectorIterator<T>::operator!= (const VectorIterator& other) const {
	return pos != other.pos;
}

template<class T>
T VectorIterator<T>::operator* () const {
	return vec->operator[](pos);
}

template<class T>
const VectorIterator<T>& VectorIterator<T>::operator++() {
	pos++;
	return *this;
}

}
#endif