
#ifndef TRANSACTIONALOBJECT_H
#define TRANSACTIONALOBJECT_H
#include "ConcurrentQueue.h"

template <typename O>
class TransactionalObject
{
public:
	TransactionalObject(O obj);

	~TransactionalObject();

	void SetObject(O newVal);
	O GetObject();

	std::atomic<ConcurrentQueue<O>*> o_queue;// = new std::atomic<ConcurrentQueue<O>*>;
};

template <typename O>
TransactionalObject<O>::TransactionalObject(O obj)
{
	o_queue.store(new ConcurrentQueue<O>(obj));

}

template <typename O>
TransactionalObject<O>::~TransactionalObject()
{

}

template <typename O>
void TransactionalObject<O>::SetObject(O newVal)
{
	ConcurrentQueue<O>* obj_q = o_queue.load();
//	typename ConcurrentQueue<O>::pointer evil_dequeuer{};
	while(true)
	{
		pointer<O> tail = obj_q->Tail.load();
		pointer<O> next = tail.ptr->next.load();
		if(tail == obj_q->Tail.load())
		{
			if(!next.ptr)
			{
				obj_q->enqueue(&newVal);
				break;
			}
		}
	}
}

template <typename O>
O TransactionalObject<O>::GetObject()
{
	ConcurrentQueue<O>* obj_q = o_queue.load(std::memory_order_acquire);
	O obj{};
	O* evil_dequeuer{};
	
	pointer<O> tail = obj_q->Tail.load();
	pointer<O> next = tail.ptr->next.load();

	while((evil_dequeuer = obj_q->dequeue()) != nullptr)
	{
		obj = *evil_dequeuer;
		if (obj == obj_q->Tail.load().ptr->data) break;
	}
	obj_q->enqueue(&obj);
	o_queue.store(obj_q, std::memory_order_release);
	return obj;
}

#endif // TRANSACTIONALOBJECT_H