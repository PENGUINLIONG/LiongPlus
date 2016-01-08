// File: IStandardizable.hpp
// Author: Rendong Liang (Liong)

#ifndef _L_IStandardizable
#define _L_IStandardizable
#include "Fundamental.hpp"

namespace LiongPlus
{
#ifdef _L_STL_COMPATIBILITY
	/// <summary>
	/// The IStandardizable interface is implemented by classes that are alternatives to C++ STL containers to achieve compability to standard C++.
	/// </summary>
	/// <remarks>This interface and all sorts of Standardize operation will be not available when the option #_L_STL_COMPATIBILITY in Configuration Center is disabled.</remarks>
	template<typename T>
	class IStandardizable
		: public virtual Interface
	{
	public:
		virtual T Standardize() = 0;
	};
#endif
}
#endif