// File: IEnumerable.hpp
// Author: Rendong Liang (Liong)

#ifndef _L_IEnumerable
#define _L_IEnumerable
#include "Fundamental.hpp"
#include "IEnumerator.hpp"
#include "Ptr.hpp"

namespace LiongPlus
{
	namespace Collections
	{
		/// <summary>
		/// Exposes the enumerator, which supports a simple iteration over a collection of a specified type.
		/// </summary>
		/// <typeparam name="T">The type of objects to enumerate.</typeparam>
		template<typename T>
		class IEnumerable
			: public virtual Interface
		{
		public:
			/// <summary>
			/// Returns an enumerator that iterates through a collection.
			/// </summary>
			/// <returns>An [LiongPlus::Collections::IEnumerator] object that can be used to iterate through the collection.</returns>
			virtual Ptr<IEnumerator<T>> GetEnumerator() = 0;
		};
	}
}
#endif