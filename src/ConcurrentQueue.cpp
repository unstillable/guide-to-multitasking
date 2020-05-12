#include <iostream>
#include "ConcurrentQueue.h"
#include <cassert>
#include <thread>
#include <string>
#include "TransactionalObject.h"
#include "ThreadedQueue.h"
#include "ThreadedQueueWrapper.h"
#include "intThreadedTaskQueue.h"


template <typename T>
Node<T>::Node(T* data)
	: data(std::move(*data)), next{ pointer<T>{} }
{
}


//template<typename T>
//ConcurrentQueue<T>::pointer::pointer(Node node, unsigned int initCount): count(initCount)
//	, ptr(node)
//{
//}

//template <class T>
//pointer<T>::~pointer()
//{
//	delete ptr;
////	this->ptr = nullptr;
//	this->count = 0;
//}

template <typename T>
bool pointer<T>::operator==(const pointer& rhs)
{
	return this->ptr == rhs.ptr && this->count == rhs.count;
}

template <typename T>
Node<T>::Node()
	:next(pointer<T>{})
{
}

template <typename T>
ConcurrentQueue<T>::ConcurrentQueue(Node<T>* placeholder)
	: head(pointer<T>{ placeholder, 0u }), Tail(head.load())
{
	Tail = head.load();
}

template <class T>
ConcurrentQueue<T>::ConcurrentQueue(T placeholder)
	:ConcurrentQueue<T>(new Node<T>(&placeholder))
{

}

template <typename T>
ConcurrentQueue<T>::~ConcurrentQueue()
{

};

template <typename T>
T* ConcurrentQueue<T>::enqueue(T* data)
{
	Node<T>* node = new Node<T>(data);
	pointer<T> tail;
	while (true)
	{
		tail = this->Tail.load();
		pointer<T> next = tail.ptr->next.load();
		if (tail == Tail.load())
		{
			if (!next.ptr)
			{
				if (tail.ptr->next.compare_exchange_weak(next, pointer<T>{ node, next.count + 1u }))
					break;

			}
			else
			{
				Tail.compare_exchange_weak(tail, pointer<T>{ next.ptr, tail.count + 1u });
			}
		}
	}
	Tail.compare_exchange_strong(tail, pointer<T>{ node, tail.count + 1u });
	return reinterpret_cast<T*>(reinterpret_cast<unsigned>(Tail.load(std::memory_order::memory_order_consume).ptr));
}

template <typename T>
T* ConcurrentQueue<T>::dequeue()
{
	T* dataOut = nullptr;
	pointer<T> hd;
	while (true)
	{
		hd = head.load();
		pointer<T> tl = Tail.load();
		pointer<T> next = hd.ptr->next.load();
		if (hd == head.load()) {
			if (hd.ptr == tl.ptr) {
				if (!next.ptr)
					return nullptr; //break? may cause double delete.
				Tail.compare_exchange_strong(tl, pointer<T>{ next.ptr, tl.count + 1 });
			}
			else
			{
				dataOut = &next.ptr->data;
				if (head.compare_exchange_weak(hd, pointer<T>{ next.ptr, hd.count + 1 }))
				{
					break;
				}
			}
		}
	}
	delete hd.ptr;
	return dataOut;
}




class ArbitraryStruct
{
public:
	int four0;
	int four1;
	int four2;
	int four3;
	bool equals(const int a, const int b, const int c, const int d) const
	{
		return four0 == a
			&& four1 == b
			&& four2 == c
			&& four3 == d;
	}
};
std::atomic_bool flag = new std::atomic_bool();

enum oper
{
	queue,
	dequeue
};

template<class T>
class thread_test_runner
{
public:
	thread_test_runner<T>(ConcurrentQueue<T>* Q)//, T in, T out)
		: Q(Q)
		//:tIn(in), tOut(out), Q(Q)
	{
	}
	void startqueueing(const int i)
	{
		//std::atomic_uint i = 0;
		//while(flag.load())
		//{
		//	printf("enqueue");
		//	Q->enqueue(&tIn);
		//	//system("REM enqueued");
		//}

		for (int j = i; j < i + 100000; j++)
		{
			Q->enqueue(new int(j));
		}

	}

	void startdequeing()
	{
		while (flag.load())
		{
			printf("dequeue");
			//system("PAUSE");
			//Q->dequeue(&tOut);

		}
	}

	void operator()(const oper& o, int x)//, std::function<void> fun)
	{
		//fun();
		switch (o)
		{
		case queue:
			startqueueing(x);
			break;
		case dequeue:
			startdequeing();
			break;
		default:;
		}
	}

private:
	//T tIn;
	//T tOut;
	ConcurrentQueue<T>* Q;
};

template<class T>
class thread_test//_wrapper
{
public:
	thread_test(ConcurrentQueue<T>* Q, oper o, int in)//, std::function<void> fun)
	{
		test_runner = new thread_test_runner<T>(Q);// , in, in);
		_t = std::thread(std::ref(*test_runner), o, in);// , fun);
	}
	~thread_test()
	{
		if (_t.joinable()) _t.join();
		delete test_runner;
	}
private:
	thread_test_runner<T> * test_runner;
	std::thread _t;

};
//int main(int argc, char* args[])
//{
//	//const std::string welcomeMessage = "Welcome to uLisp\n";
//	//uLisp u{};
//	//u.RELP(welcomeMessage);
//	//return 0;
//	{
//		int x = 0;
//		TransactionalObject<int> to{ x };
//		assert(to.GetObject() == x);
//	}
//	{
//		int x = 0;
//		int y = 1;
//		int z = 2;
//		int a = 3;
//		int b = 5;
//		TransactionalObject<int> to{ x };
//		to.SetObject(++x);
//		to.SetObject(y);
//		to.SetObject(a);
//
//		assert(to.GetObject() == 3);
//		
//		to.SetObject(b);
//		assert(to.GetObject() == 5);
//		assert(to.GetObject() == 5);
//		
//		to.SetObject(y);
//		assert(to.GetObject() == 1);
//		assert(to.GetObject() == 1);
//		assert(to.GetObject() == 1);
//	}
//}
void inttasktest()
{

	for (int repeat = 0; repeat < 10000000; ++repeat)
	{
		intThreadedQueueWrapper taskq;
		{
			std::function<int(int*)> mylambda = [](const int* x) {return *x; };
			auto first = std::async(std::launch::async, [&]()//->std::vector<std::shared_future<int>> {
			{
				//ConcurrentQueue<std::shared_future<int>>* checker = new ConcurrentQueue<std::shared_future<int>>();
	//			checker.reserve(1000);
				for (int i = 0; i < 10000; ++i)
				{
					//std::shared_future<int>* tmp = 
#if randomize
					taskq.enqueue(std::rand(), mylambda);
#else
					taskq.enqueue(i, mylambda);
#endif
					//checker->enqueue(tmp->get());

				}
				//return checker;
			});
			auto second = std::async(std::launch::async, [&] {// [&]()->std::vector<std::shared_future<int>> {


				for (int i = 10000; i < 20000; ++i)
				{
					std::shared_future<int> tmp = taskq.enqueue(i, mylambda);
				}
			});
			auto third = std::async(std::launch::async, [&] {//()->std::vector<std::shared_future<int>> {


				for (int i = 20000; i < 30000; ++i)
				{
					std::shared_future<int> tmp = taskq.enqueue(i, mylambda);
				}
			});

			first.get();
			//auto checker = 
			auto check = taskq.enqueue(200, mylambda);
			//auto iCheck = checker->dequeue();
			//while(iCheck != nullptr)
			//{
			//	assert(iCheck->valid());
			//	auto o = iCheck->get();
			//	std::cout << o << "\n";
			//	iCheck = checker->dequeue();
			//}
			/*for (auto iter = checker.begin(); iter < checker.end(); ++iter)
			{
				std::cout << iter->get() << "\n";
			}*/

			second.get();
			/*for (auto & iter : checker)
			{
				std::cout << iter.get() << "\n";
			}
			*/
			third.get();
			/*for (auto & iter : checker)
			{
				std::cout << iter.get() << "\n";
			}*/
			auto sum = 0;
			std::vector<int> vect;

			auto popper = taskq.dequeue();
			while (popper != nullptr)
			{
				//std::cout << *popper << "\n";
				sum += *popper;
				vect.emplace_back(*popper);
				delete popper;
				popper = taskq.dequeue();

			}
			std::cout << check.get();
			std::sort(vect.begin(), vect.end());

			//std::for_each(vect.begin(),vect.end(), [=](int x){ std::cout << x << '\n';});
			auto prev = 0;//-1;

			for (auto iter = vect.begin(); iter < vect.end(); ++iter)
			{
				//	std::cout<< *iter << '\n';
				//	assert(*iter == 200 || *iter > prev);
				//	prev = *iter;
				prev += *iter;
			}

			//auto checkthatnextis200 = std::find(vect.begin(), vect.end(), check.get());
			//std::cout << *checkthatnextis200 << '\n';
			std::cout << prev << '\n';
			std::cout << sum << '\n';
			std::cout << (sum == prev) << "\nend run" << repeat << ".\n";

			vect.clear();
		}
		taskq.kill();
	}
}
#define thismain
#define ThreadTest 0
#define intTaskTest 0 
#define randomize 1
#define Queuetest 0
#define memtest 0
#define taskMemtest 1

#ifdef  thismain

int main()
{

#if intTaskTest
	std::vector<std::thread> threads;
	for(int i = 0; i < 50; i++)
	{
		threads.emplace_back(inttasktest);
	}
	for(int i = 0; i < 100; i++)
	{
		threads.back().join();
		threads.pop_back();
	}
#endif
#if memtest
{
	ConcurrentQueue<int> q{};
	for(int i = 0; i < 10000000;  ++i)
	{
		q.enqueue(&i);
	}
	for(int i = 0; i < 10000000;  ++i)
	{
		q.dequeue();
	}

}
#endif
#if taskMemtest
{
	intThreadedQueueWrapper q{};

	std::function<int(int*)> mylambda = [](const int* x) {return *x; };
	for(int i = 0; i < 1000000;  ++i)
	{
		q.enqueue(i,mylambda);
	}
	for(int i = 0; i < 1000000;  ++i)
	{
		auto t = q.dequeue();
		std::cout << *t; 
	}

}
#endif
#if Queuetest 

	ConcurrentQueue<int> q{};

	//enque all ints in range range from given number to given + 100
	thread_test<int>* t0 = new thread_test<int>(&q, oper::queue, 1);
	thread_test<int>* t1 = new thread_test<int>(&q, oper::queue, 100001);
	thread_test<int>* t2 = new thread_test<int>(&q, oper::queue, 200001);
	thread_test<int>* t3 = new thread_test<int>(&q, oper::queue, 300001);
	thread_test<int>* t4 = new thread_test<int>(&q, oper::queue, 400001);
	thread_test<int>* t5 = new thread_test<int>(&q, oper::queue, 500001);

	delete t0;
	delete t1;
	delete t2;
	delete t3;
	delete t4;
	delete t5;

	std::vector<int> vect;
	std::vector<int> vect0;
	std::vector<int> vect1;
	std::vector<int> vect2;
	std::vector<int> vect3;
	std::vector<int> vect4;
	std::vector<int> vect5;
	auto deq = q.dequeue();
	while (deq != nullptr)
	{
		vect.push_back(*deq);
		deq = q.dequeue();
	}



	for (auto iter = vect.begin(); iter != vect.end(); ++iter)
	{
		if (*iter <= 100000)
		{
			vect0.push_back(*iter);
		}
		else if (*iter <= 200000)
		{
			vect1.push_back(*iter);
		}
		else if (*iter <= 300000)
		{
			vect2.push_back(*iter);
		}
		else if (*iter <= 400000)
		{
			vect3.push_back(*iter);
		}
		else if (*iter <= 500000)
		{
			vect4.push_back(*iter);
		}
		else
		{
			vect5.push_back(*iter);
		}
	}


	int prev = 0;

	for (auto iter = vect0.begin(); iter != vect0.end(); ++iter)
	{
		if (*iter == prev || *iter != prev + 1)
		{
			assert(false);
		}

		prev = *iter;
	}
	assert(prev == 100000);
	for (auto iter = vect1.begin(); iter != vect1.end(); ++iter)
	{
		if (*iter == prev || *iter != prev + 1)
		{
			assert(false);
		}

		prev = *iter;
	}
	for (auto iter = vect2.begin(); iter != vect2.end(); ++iter)
	{
		if (*iter == prev || *iter != prev + 1)
		{
			assert(false);
		}

		prev = *iter;
	}
	for (auto iter = vect3.begin(); iter != vect3.end(); ++iter)
	{
		if (*iter == prev || *iter != prev + 1)
		{
			assert(false);
		}

		prev = *iter;
	}
	for (auto iter = vect4.begin(); iter != vect4.end(); ++iter)
	{
		if (*iter == prev || *iter != prev + 1)
		{
			assert(false);
		}

		prev = *iter;
	}
	for (auto iter = vect5.begin(); iter != vect5.end(); ++iter)
	{
		if (*iter == prev || *iter != prev + 1)
		{
			assert(false);
		}

		prev = *iter;
	}
#endif
#if ThreadTest
	ThreadedQueueWrapper<int, int> queue;
	ConcurrentQueue<conc_future<int, int>> checker;
	//std::function<int(int)> func2 = [](int i) { return i + 4; };
	const std::function<int(int*)> mylambda = [](const int* x) {return *x * *x; };
	int x = 'g';
	while (x != 'q')
	{
		std::cin >> x;
		queue.enqueue(&x, mylambda);

	}
	queue.kill();
	auto y = checker.dequeue();
	while (y)
	{
		printf("%d", *y->get());
		y = checker.dequeue();
	}

#endif


}
#endif