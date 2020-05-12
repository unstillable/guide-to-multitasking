
#ifndef CONCURRENTQUEUE_H
#define CONCURRENTQUEUE_H
#define _ENABLE_ATOMIC_ALIGNMENT_FIX 1
#include <atomic>
#include <thread>
#include <condition_variable>
#include <future>
#include "Node.h"
#include "Pointer.h"

template<class T>
struct QTask
{
	std::promise<bool>* prom;
	std::function<bool(T*)>* func;
	std::atomic_bool done = false;
	T* obj;
};


template<class T>
class ConcurrentQueue
{
public:
	explicit ConcurrentQueue(Node<T>* placeholder = new Node<T>());
	explicit ConcurrentQueue(T placeholder);
	virtual ~ConcurrentQueue();
	
	virtual T* enqueue(T* data);
	virtual T* dequeue();

	
	//---------------------------------
	// Data --- head and tail pointers
	//---------------------------------
	std::atomic<pointer<T>> head;
	std::atomic<pointer<T>> Tail;
	//this is not complete
	//void operator ()()
	//{
	//	while (run.load())
	//	{
	//		QTask<T>* task = todo.load();
	//		if (!task) continue;

	//		if(task->done.load())
	//		{
	//			std::function<bool(T*)> current = task->func;
	//			T* obj = task->obj;
	//			std::promise<bool>* p = task->prom;
	//			if (Concurrency::details::atomic_compare_exchange(todo, new QTask<T>{nullptr,nullptr, false, nullptr}, task))
	//			{
	//				p->set_value(current(obj));
	//			}
	//		}
	//	}
	//}

	//void stop();
	//void doThis(QTask<T>* dothis)
	//{
	//	todo = dothis;
	//};
	//std::atomic<QTask<T>*> todo;

	virtual void operator()()
	{

	}

	std::atomic_bool run = true;
	//std::promise<bool>
};



template<class T>
class ConcurrentQueueWrapper//_wrapper
{
public:
	ConcurrentQueueWrapper()
	{
		cq = new ConcurrentQueueWrapper<T>();
		_t = std::thread(std::ref(*cq));
	}
	~ConcurrentQueueWrapper()
	{
		//cq->stop();
		if (_t.joinable()) _t.join();
		delete cq;
	}

	void enqueue(T* data)
	{
		
	}

	bool dequeue(T* dataOut){};
private:
	ConcurrentQueueWrapper<T> * cq;
	std::thread _t;
};

#endif // CONCURRENTQUEUE_H
