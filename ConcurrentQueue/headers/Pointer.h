#ifndef POINTER_H
#define POINTER_H

#include "Node.h"

//-------------------------------------------------------------
// Pointer struct
//	Based off of the Micheal and Scott Lock-free queue
//      
//-------------------------------------------------------------

template<class T>
struct Node;

template<class T>
struct pointer
{
public:
	Node<T>* ptr;
	unsigned int count;
	bool operator ==(const pointer& rhs);
};
#endif // POINTER_H
