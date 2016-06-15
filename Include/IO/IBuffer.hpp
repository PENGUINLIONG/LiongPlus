// File: MemoryStream.hpp
// Author: Rendong Liang (Liong)
#include "Fundamental.hpp"

#ifndef IBuffer_hpp
#define IBuffer_hpp
namespace LiongPlus
{
	namespace IO
	{
		class IBuffer
			: public virtual Interface
		{
			/// <summary>
			/// Release the control to buffer and then reallocate new memory of the same size.
			/// </summary>
			virtual Byte* AbandonBuffer() = 0;
			virtual const Byte* AccessBuffer() = 0;
			virtual bool IsBufferAccessable() = 0;
		};
	}
}
#endif