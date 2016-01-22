// File: ContinuousMemoryEnumerator.hpp
// Author: Rendong Liang (Liong)

#ifndef _L_ContinuousMemoryEnumerator
#define _L_ContinuousMemoryEnumerator
#include "Fundamental.hpp"
#include "../Exception.hpp"
#include "IEnumerator.hpp"

namespace LiongPlus
{
	namespace Collections
	{
		template<typename T>
		class ContinuousMemoryEnumerator
			: public Object
			, public IEnumerator<T>
		{
		private:
			T* _Field;
			int _Length, _Current;

		public:
			ContinuousMemoryEnumerator(T* field, int length, int current)
				: _Field(field)
				, _Length(length)
				, _Current(current)
			{
			}
			~ContinuousMemoryEnumerator()
			{
			}

			virtual T& Current() override final
			{
				return *(_Field + _Current);
			}

			virtual bool Equals(IEnumerator<T>* value) override final
			{
				return &value->Current() == &Current();
			}

			virtual bool MoveNext() override final
			{
				if (_Current < _Length)
				{
					++_Current;
					return true;
				}
				else
					return false;
			}

			virtual void Reset() override final
			{
				_Current = 0;
			}

			// IDisposable

			virtual void Dispose() override final
			{
			}
		};
	}
}
#endif