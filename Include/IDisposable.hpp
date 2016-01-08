// File: IDisposable.hpp
// Author: Rendong Liang (Liong)

#ifndef _L_IDisposable
#define _L_IDisposable
#include "Fundamental.hpp"

namespace LiongPlus
{
	class IDisposable
		: public virtual Interface
	{
	protected:
		virtual void Dispose() = 0;
	};
}
#endif