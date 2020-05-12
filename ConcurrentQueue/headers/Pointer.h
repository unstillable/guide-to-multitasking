#ifndef POINTER_H
#define POINTER_H

template<class T>
struct Node;

template<class T>
struct pointer
{
public:
	//skip the big four/5/6,etc. because this only needs to work, nothing more
	//pointer(Node node = nullptr, unsigned int initCount = 0);
	//data

	Node<T>* ptr;
	unsigned int count;
	//~pointer<T>();

	//		unsigned char padding[8];
	bool operator ==(const pointer& rhs);
};
#endif // POINTER_H
