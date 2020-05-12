#ifndef THREADEDQUEUEWRAPPER_H
#define THREADEDQUEUEWRAPPER_H

#include "ThreadedQueue.h"

template<class T, class fout>
class ThreadedQueueWrapper//_wrapper
{
public:
	struct task_tuple : Base<task_tuple, T>
	{
		task_tuple()
		{

		}
		task_tuple(T* (*func)(T*), T* data, std::promise<T*> &&prom)
		{
			this->func = func;
			this->data = data;
			this->prom = std::move(prom);
		}
		T* myTask()
		{
			return func(data);
		}
		std::function<T*(T*)> func;
		T* data;
		std::promise<T*> prom;
	};
	
public:
	ThreadedQueueWrapper()
	{
		cq = new ThreadedQueue<T,fout>(new Node<T>());
		_t = std::thread(std::ref(*cq));
	}
	~ThreadedQueueWrapper()
	{
		//cq->stop();
		if (_t.joinable()) _t.join();
		delete cq;
	}

	//return a future for now. 

	conc_future<T,fout>& enqueue(T* data, std::function<fout(T*)> v)
	{
		std::promise<T*> promise;
		std::shared_future<T*> fu = promise.get_future().share();

		conc_future<T, fout> tmp{v,data,std::move(promise)};
		cq->thingsTodo.enqueue(&tmp);
		return *new conc_future<T,fout>(std::move(fu), data);
	}

	//T* dequeue(T* dataOut)
	//{
	//	return cq->;
	//}

	void kill()
	{
		cq->run.store(false);
	}

	void foreach(void(*lambda)(T x))
	{
		cq->foreach(lambda);

	};
	ThreadedQueue<T, fout> * cq;
	std::thread _t;
private:
};
#endif // THREADEDQUEUEWRAPPER_H
