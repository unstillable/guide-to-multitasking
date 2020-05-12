//#include "LockFreeQueue.h"
//#include <pplinterface.h>
//
//
//void initialize(LockFreeQueue* Q, void* data)
//{
//	Node* node = new Node(data,nullptr);
//	Q->Head.store(node, std::memory_order_seq_cst); = Q->Tail->ptr = node;
//}
//
//void enqueue(LockFreeQueue* Q, void* data)
//{
//	auto node = new Node(data, nullptr);
//	pointer* tail = nullptr;
//	pointer* next = nullptr;
//	while(true)
//	{
//		tail = Q->Tail;
//		next = tail->ptr->next;
//		if (tail == Q->Tail)
//			if (next->ptr == nullptr)
//				
//	}
//}
//
//
//pointer::pointer(Node* ptr, unsigned count): ptr(ptr),
//                                             count(count)
//{
//}
//
//Node::Node(void* data, pointer* next): value(data)
//{
//	this->next = new pointer(nullptr, 1);
//}
