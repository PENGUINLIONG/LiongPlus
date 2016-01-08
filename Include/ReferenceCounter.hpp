// File: ReferenceCounter.hpp
// Author: Rendong Liang (Liong)

#ifndef _L_ReferenceCounter
#define _L_ReferenceCounter
#include "Fundamental.hpp"

namespace LiongPlus
{
	struct ReferenceCounter
	{
	public:
		volatile int _RefCount;

		ReferenceCounter()
			: _RefCount(0)
		{
		}

		/// <summary>
		/// Increase reference count.
		/// </summary>
		__forceinline void Inc()
		{
			_RefCount++;
		}

		/// <summary>
		/// Decrease reference count.
		/// </summary>
		/// <returns>False if reference count becomes 0. Otherwise, true will be returned.</returns>
		__forceinline bool Dec()
		{
			_RefCount--;
			return _RefCount == 0 ? false : true;
		}
	};
}
#endif