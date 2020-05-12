
#ifndef CRTP_H
#define CRTP_H

template<class derived, class T>
class Base
{
	T* doTask()
	{
		return static_cast<derived*>(this)->myTask();
	}
};



template<class T = int>
T* process(T* x)
{
	return x * x;
}
#endif // CRTP_H
