// File: IEnumerator.hpp
// Author: Rendong Liang (Liong)

#ifndef _L_IEnumerator
#define _L_IEnumerator
#include "Fundamental.hpp"

namespace LiongPlus
{
	namespace Collections
	{
		/// <summary>
		/// Supports a simple iteration over a collection.
		/// </summary>
		/// <typeparam name="T">The type of objects to enumerate.</typeparam>
		template<typename T>
		class IEnumerator
			: public virtual Interface
			, public IDisposable
		{
		public:
			/// <summary>
			/// Gets the element in the collection at the current position of the enumerator.
			/// </summary>
			/// <returns>The element in the collection at the current position of the enumerator.</returns>
			virtual T& Current() = 0;
			/// <summary>
			/// This method checks if $value and the instance are equal.
			/// </summary>
			// <note>It is strongly recommented to overload operator==()for better experience.</note>
			virtual bool Equals(IEnumerator<T>* value) = 0;
			/// <summary>
			/// Advances the enumerator to the next element of the collection.
			/// </summary>
			/// <returns>[true] if the enumerator was successfully advanced to the next element; [false] if the enumerator has passed the end of the collection.</returns>
			virtual bool MoveNext() = 0;
			/// <summary>
			/// Sets the enumerator to its initial position, which is before the first element in the collection.
			/// </summary>
			virtual void Reset() = 0;
		};

		/// <summary>
		/// To take the advantage of range-based loop.
		/// </summary>
		template<typename T>
		class EnumeratorParser
		{
		private:
			IEnumerator<T>* _Enumerator;
		public:
			EnumeratorParser(IEnumerator<T>* enumerator)
				: _Enumerator(enumerator)
			{
			}
			~EnumeratorParser()
			{
				delete _Enumerator;
			}

			bool operator!=(const EnumeratorParser& value)
			{
				return !value._Enumerator->Equals(_Enumerator);
			}
			EnumeratorParser<T>& operator++()
			{
				_Enumerator->MoveNext();
				return *this;
			}
			T& operator*()
			{
				return _Enumerator->Current();
			}
		};
	}
}
#endif