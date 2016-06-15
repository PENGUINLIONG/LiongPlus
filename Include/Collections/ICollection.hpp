// File: ICollection.hpp
// Author: Rendong Liang (Liong)

#ifndef _L_ICollection
#define _L_ICollection
#include "Fundamental.hpp"
#include "IEnumerable.hpp"

namespace LiongPlus
{
	template<typename T>
	class Array;
	namespace Collections
	{

		/// <summary>
		/// Defines methods to manipulate generic collections.
		/// </summary>
		/// <typeparam name="T">The type of the elements in the collection.</typeparam>
		template<typename T>
		class ICollection
			: public virtual Interface
			, public IEnumerable<T>
		{
		public:
			/// <summary>
			/// Adds an item.
			/// </summary>
			/// <param name="value">The object to add.</param>
			/// <returns>The position into which the new element was inserted, or -1 to indicate that the item was not inserted into the collection.</returns>
			virtual long Add(T& value) = 0;
			/// <summary>
			/// Removes all items.
			/// </summary>
			virtual void Clear() = 0;
			/// <summary>
			/// Determines whether this contains a specific value.
			/// </summary>
			/// <param name="value">The object to locate.</param>
			/// <returns>[true] if the Object is found; otherwise, [false].</returns>
			virtual bool Contains(T& value) = 0;
			/// <summary>
			/// Gets the number of elements contained in this.
			/// </summary>
			/// <returns>The number of elements.</returns>
			virtual long GetCount() = 0;
			/// <summary>
			/// Copies the elements to an [LiongPlus::Collections::Array], starting at a particular Array index.
			/// </summary>
			/// <param name="array">The [LiongPlus::Collections::Array] that is the destination of the elements copied from [this].</param>
			/// <param name="index">The zero-based index in $array at which copying begins.</param>
			virtual void CopyTo(Array<T>& array, long index) = 0;
			/// <summary>
			/// Inserts an item at the specified index.
			/// </summary>
			/// <param name="index">The zero-based index at which value should be inserted.</param>
			/// <param name="value">The object to insert.</param>
			virtual void Insert(long index, T& value) = 0;
			/// <summary>
			/// Removes the first occurrence of a specific object.
			/// </summary>
			/// <param name="value">The object to remove.</param>
			virtual void Remove(T& value) = 0;
		};
	}
}
#endif