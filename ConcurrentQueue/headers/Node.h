#ifndef NODE_H
#define NODE_H
#include "Pointer.h"

//-------------------------------------------------------------
// Node struct
//	Based off of the Micheal and Scott Lock-free queue
//      
//-------------------------------------------------------------

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
