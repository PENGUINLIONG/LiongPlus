// File: IObservable.hpp
// Author: Rendong Liang (Liong)

#ifndef _L_IObservable
#define _L_IObservable
#include "Fundamental.hpp"
#include "IObserver.hpp"

namespace LiongPlus
{
	template<typename T>
	class IObservable
		: public virtual Interface
	{
	public:
		virtual unique_ptr<Object> Subscribe(IObserver<T> observer) = 0;
	};
}

#endif
