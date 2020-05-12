#include "ThreadedQueue.h"


//template <class T>
//ThreadedQueue<T>::ThreadedQueue(Node<T>* placeholder): ConcurrentQueue<T>(placeholder)
//{
//}

template <class T, class fout>
ThreadedQueue<T, fout>::ThreadedQueue(T placeholder):ConcurrentQueue<T>(placeholder)
{

}

//
////duplicate code is sloppy, but this is a hard problem. 
//template <typename T>
//void ThreadedQueue<T>::enqueue(T* data)
//{
//	Node* node = new Node(data);
//	pointer tail;
//	while (true)
//	{
//		tail = this->Tail.load();
//		pointer next = tail.ptr->next.load();
//		if (tail == Tail.load())
//		{
//			if (!next.ptr)
//			{
//				if (tail.ptr->next.compare_exchange_weak(next, pointer{ node, next.count + 1u }))
//					break;
//
//			}
//			else
//			{
//				Tail.compare_exchange_weak(tail, pointer{ next.ptr, tail.count + 1u });
//			}
//		}
//	}
//	Tail.compare_exchange_weak(tail, pointer{ node, tail.count + 1u });
//}
//
////duplicate code is sloppy but this is a hard problem.  could just inherit... 
//template <typename T>
//bool ThreadedQueue<T>::dequeue(T* dataOut)
//{
//	pointer hd;
//	while (true)
//	{
//		hd = std::atomic_load(&head);
//		pointer tl = std::atomic_load(&Tail);
//		pointer next = hd.ptr->next.load();
//		if (hd == head.load()) {
//			if (hd.ptr == tl.ptr) {
//				if (!next.ptr)
//					return false;
//				Tail.compare_exchange_weak(tl, pointer{ next.ptr, tl.count + 1 });
//			}
//			else
//			{
//				*dataOut = next.ptr->data;
//				if (head.compare_exchange_weak(hd, pointer{ next.ptr, hd.count + 1 }))
//				{
//					break;
//				}
//			}
//		}
//	}
//	delete hd.ptr;
//	return true;
//}