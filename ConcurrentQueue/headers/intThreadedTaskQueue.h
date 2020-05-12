#ifndef INTTHREADEDTASKQUEUE_H
#define INTTHREADEDTASKQUEUE_H
#include "ConcurrentQueue.h"
#include <cassert>

class intThreadedTaskQueue
{

public:
	class task_tuple
	{
	public:
		task_tuple() {}

		task_tuple(std::function<int(int*)>& func, int* data, std::promise<int> &&prom)
		{
			this->func = func;
			this->data = data;
			this->prom = std::move(prom);
			this->fu = new std::future<int>(this->prom.get_future());
			share();
		}

		//task_tuple(const task_tuple&& t)
		//{
		//	
		//}

		std::promise<int> prom;
		std::future<int>* fu;
		std::function<int(int*)> func;
		int* data;
		std::shared_future<int> shared;

		std::shared_future<int> share()
		{
			if (!this->shared.valid())
				this->shared = this->fu->share();
			return std::ref(shared);
		}
	};


	intThreadedTaskQueue()
	{

	};

	~intThreadedTaskQueue() = default;

	int* dotask()
	{
		//assert(!intOut);
		const auto didit = thingstodid.dequeue();
		return didit ? new int(didit->get()) : nullptr;
	}

	void operator()()
	{
		while (run.load())
		{
			auto x = thingsTodo.dequeue();
			if (x != nullptr)
			{
				std::async([&]()->void
				{
					x->prom.set_value(x->func(x->data));
					thingstodid.enqueue(&x->shared);
					delete x->data;
					delete x->fu;
				}
				);
			}
		}
	}

	void kill()
	{
		run.store(false);
	}

	std::atomic_bool run = true;
	ConcurrentQueue<task_tuple> thingsTodo;
	ConcurrentQueue<std::shared_future<int>> thingstodid;
};


class intThreadedQueueWrapper//_wrapper
{


public:
	intThreadedQueueWrapper()
	{
		_t = std::thread(std::ref(cq));
	}
	~intThreadedQueueWrapper()
	{
		cq.kill();
		//cq->stop();
		if (_t.joinable()) _t.join();
	}

	//return a future for now. 

	std::shared_future<int> enqueue(int data, std::function<int(int*)> v)
	{
		std::promise<int> promise;

		intThreadedTaskQueue::task_tuple tmp(v, new int(data), std::move(promise));
		return cq.thingsTodo.enqueue(&tmp)->share();
	}

	int* dequeue()
	{
		return cq.dotask();
	}

	void kill()
	{
		cq.run.store(false);
	}

	intThreadedTaskQueue cq{};
	std::thread _t;
};









#endif // INTTHREADEDTASKQUEUE_H





