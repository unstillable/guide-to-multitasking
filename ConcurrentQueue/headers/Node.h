#ifndef NODE_H
#define NODE_H
#include "Pointer.h"

template<class T>
struct Node
{
public:

	Node<T>();
	Node<T>(T* data);
	T data;
	std::atomic<pointer<T>> next;
};

#endif