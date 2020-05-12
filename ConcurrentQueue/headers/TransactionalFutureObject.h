#pragma once
#include "ConcurrentQueue.h"
#include <future>

template<class T>
class TransactionalFutureObject
{
public:
	TransactionalFutureObject();
	~TransactionalFutureObject();

private:
	ConcurrentQueue<std::shared_future<t>> queue;
};

