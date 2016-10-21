// File: IObserver.hpp
// Author: Rendong Liang (Liong)

#ifndef _L_IObserver
#define _L_IObserver
#include "Fundamental.hpp"

namespace LiongPlus
{
	template<typename T>
	class IObserver
		: public virtual Interface
	{
	public:
		virtual void OnCompleted() = 0;
		virtual void OnError(Exception e) = 0;
		virtual void OnNext(T value) = 0;		
	}
}
#endif
