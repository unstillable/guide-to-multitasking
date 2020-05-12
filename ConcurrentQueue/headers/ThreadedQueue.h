
#ifndef ThreadedQueue_H
#define ThreadedQueue_H
#define _ENABLE_ATOMIC_ALIGNMENT_FIX 1
#include <atomic>
#include <thread>
#include <condition_variable>
#include <future>
#include "ConcurrentQueue.h"
#include "crtp.h"

//#include "ThreadedQueueWrapper.h"

//#define task_tuple(T) taskTuple<T>
//#define process(x) ;

template<class T, class fout>
class conc_future
{
public:
	conc_future(std::function<fout*(T)> fu, T* toProc)
		:fut(std::move(fu)), toProc(toProc)
	{

	}
	~conc_future()
	{
		delete toProc;
	}
	fout* get()
	{
		return fut.get();
	}



private:
	std::function<fout*(T)> fut;
	T* toProc;


};


//template<class T>
//typedef std::tuple<std::function<bool(T*)>, T*, std::promise<T*>> task_tuple;

template<class T, class fout>
class ThreadedQueue : public ConcurrentQueue<T>
{
public:


	explicit ThreadedQueue(Node<T>* placeholder = new Node<T>) : ConcurrentQueue<T>(placeholder)
	{

	};
	explicit ThreadedQueue(T placeholder);
	~ThreadedQueue() = default;

	void operator()() override
	{
		while (run.load())
		{
			auto x = thingsTodo.dequeue();
			if (x != nullptr) x->prom.set_value(x->func(x->data));

			/*if(data)
				promise->set_value(func(data));*/
				//do work
		}
	}

	std::atomic_bool run = true;

	ConcurrentQueue<Base<conc_future<T, fout>, fout>> thingsTodo;

	void foreach(void(*lambda)(T x))
	{
		pointer<T>* t = &this->ConcurrentQueue<T>::head.load();
		while (t->ptr->next.load().ptr != nullptr)
		{
			(*lambda)(t->ptr->data);
			t = &t->ptr->next.load();
		}
	}
};



//int main()
//{
//	ThreadedQueueWrapper<int> q{};
//	int a = 0;
//	int b = 1;
//	int c = 2;
//	int d = 3;
//	int e = 4;
//	int f = 5;
//	int g = 6;
//	q.enqueue(&a);
//	q.enqueue(&b);
//	q.enqueue(&c);
//	q.enqueue(&d);
//	q.enqueue(&e);
//	q.enqueue(&f);
//	q.enqueue(&g);
//	
//	q.foreach([](int x) -> void { printf("%d /n", x); });
//	system("PAUSE");
//
		//int a0{ 0,0,0,0 };
		//thread_test<int> t1(&q, oper::queue, a0);
		//thread_test<int> t2(&q, oper::dequeue, a0);
		//std::string quit;
		//while (!quit._Equal("q"))
		//	std::cin >> quit;
		//flag.store(false);

		//int a2{ 2,2,2,2 };
		//int a3{ 3,3,3,3 };
		//int a4{ 4,4,4,4 };
		//int a5{ 5,5,5,5 };

		//q.enqueue(&a0);
		//q.enqueue(&a1);
		//q.enqueue(&a2);
		//q.enqueue(&a3);
		//q.enqueue(&a4);
		//q.enqueue(&a5);

		//int outData{};

		//if (!q.dequeue(&outData)) assert(false);

		//assert(outData.equals(0, 0, 0, 0));
		//if (!q.dequeue(&outData)) assert(false);

		//assert(outData.equals(1, 1, 1, 1));
		//if (!q.dequeue(&outData)) assert(false);

		//assert(outData.equals(2, 2, 2, 2));
		//if (!q.dequeue(&outData)) assert(false);

		//assert(outData.equals(3, 3, 3, 3));
		//if (!q.dequeue(&outData)) assert(false);

		//assert(outData.equals(4, 4, 4, 4));
		//if (!q.dequeue(&outData)) assert(false);

		//assert(outData.equals(5, 5, 5, 5));
		//if (!q.dequeue(&outData)) assert(true);

		//assert(outData.equals(5, 5, 5, 5));


//}
#endif // ThreadedQueue_H
