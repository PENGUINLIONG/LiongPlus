// File: IComparer.hpp
// Author: Rendong Liang (Liong)

#ifndef _L_IComparer
#define _L_IComparer
#include "Fundamental.hpp"
#include "../Delegate.hpp"

namespace LiongPlus
{
	namespace Collections
	{
		/// <summary>
		/// Defines a method that a type implements to compare two objects.
		/// </summary>
		/// <typeparam name="T">The type of the items to be compared.</typeparam>
		template<typename T>
		class IComparer
			: public virtual Interface
		{
		public:
			/// <summary>
			/// Compares two objects and returns a value indicating whether one is less than, equal to, or greater than the other.
			/// </summary>
			/// <param name="x">The first object to compare.</param>
			/// <param name="y">The second object to compare.</param>
			/// <returns>A signed integer that indicates the relative items of x and y. It should be a value less than zero if x is less than y, zero if x is equal to y, or a value greater than zero if x is greater than y.</returns>
			virtual long Compare(T& x, T& y) = 0;
		};

		/// <summary>
		/// Parse a [LiongPlus::Comparison] as a [LiongPlus::Collections::IComparer] to sort a array or do something else.
		/// </summary>
		template<typename T>
		class FunctorComparer
			: public Object
			, public IComparer<T>
		{
		public:
			FunctorComparer(Comparison<T>& functor)
				: _Functor(functor)
			{
			}

			virtual long Compare(T& x, T& y)
			{
				return _Functor(x, y);
			}
		private:
			Comparison<T> _Functor;
		};
	}
}
#endif