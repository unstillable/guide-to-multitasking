//
//#ifndef LOCKFREEQUEUE_H
//#define LOCKFREEQUEUE_H
//#include <atomic>
//
//
//class pointer
//{
//public:
//	pointer(class Node* ptr, unsigned count);
//
//	std::atomic<Node*> ptr;
//	std::atomic_uint count;
//};
//
//
//class Node
//{
//public:
//	Node(void* data, pointer* next);
//
//	void* value;
//	std::atomic<pointer*> next;
//};
//
//class LockFreeQueue
//{
//public:
//
//	std::atomic<pointer*> Head;
//	std::atomic<pointer*> Tail;
//
//};
//
//
//
//#endif // LOCKFREEQUEUE_H
