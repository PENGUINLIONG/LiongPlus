#pragma once
// File: IList.hpp
// Author: Rendong Liang (Liong)

#ifndef _L_IList
#define _L_IList
#include "Fundamental.hpp"
#include "ICollection.hpp"
#include "IEnumerable.hpp"

namespace LiongPlus
{
	namespace Collections
	{
		template<typename T>
		class IList
			: public virtual Interface
			, public ICollection<T>
			, public IEnumerable<T>
		{
		public:
			/// <summary>
			/// Determines the index of a specific item in [this].
			/// </summary>
			/// <param name="item">The object to locate in the [this}.</param>
			/// <returns>The index of item if found in the list; otherwise, -1.</returns>
			virtual int IndexOf(T& item) = 0;
			/// <summary>
			/// Inserts an item to the [this] at the specified index.
			/// </summary>
			/// <param name="index">The zero-based index at which $item should be inserted.</param>
			/// <param name="item">The object to insert into [this].</param>
			virtual void Insert(int index, T& item) = 0;
			/// <summary>
			/// Remove item at the specified index.
			/// </summary>
			/// <param name="index">The zero-based index of the item to remove.</param>
			virtual void RemoveAt(int index) = 0;
			// Implement operator[].
		};
	}
}
#endif