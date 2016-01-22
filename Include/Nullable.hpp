// File: Nullable.hpp
// Author: Rendong Liang (Liong)

#ifndef _L_Nullable
#define _L_Nullable
#include "Fundamental.hpp"

namespace LiongPlus
{
	/// <summary>
	/// Help store optional object which can be nullptr.
	/// </summary>
	/// <typeparam name="T">Type of object</typeparam>
	template<typename T>
	class Nullable
		: public Object
	{
	private:
		typedef Nullable<T> TSelf;
	protected:
		T* _Object;

		inline void Clear()
		{
			if (_Object)
			{
				delete _Object;
				_Object = nullptr;
			}
		}
	public:
		Nullable()
			:_Object(nullptr)
		{
		}
		Nullable(const T& object)
			:_Object(new T(Move(object)))
		{
		}
		Nullable(T&& object)
			:_Object(Move(object))
		{
		}
		Nullable(const Nullable<T>& nullable)
			:_Object(nullable._Object ? new T(*nullable._Object) : nullptr)
		{
		}
		Nullable(Nullable<T>&& nullable)
			:_Object(nullable._Object)
		{
			nullable._Object = nullptr;
		}
		~Nullable()
		{
			Clear();
		}

		TSelf& operator=(Null nullptr)
		{
			Clear();
		}
		TSelf& operator=(const T& object)
		{
			Clear();
			_Object = new T(object);
			return *this;
		}
		TSelf& operator=(T&& object)
		{
			Clear();
			_Object = &object;
			return *this;
		}
		TSelf& operator=(const TSelf& nullable)
		{
			if (this != &nullable)
			{
				Clear();
				_Object = nullable._Object
					? new T(*nullable._Object)
					: nullptr;
			}
			return *this;
		}
		TSelf& operator=(TSelf&& nullable)
		{
			if (this != &nullable)
			{
				Clear();
				_Object = nullable._Object;
				nullable = nullptr;
			}
			return *this;
		}

		bool operator==(const TSelf& nullable) const
		{
			return Equals(*this, nullable);
		}
		bool operator==(const Null nullptr) const
		{
			return !_Object;
		}
		bool operator!() const
		{
			return !_Object;
		}
		bool operator!=(const TSelf& nullable) const
		{
			return !Equals(*this, nullable);
		}

		operator bool() const
		{
			return _Object;
		}

		bool Equals(TSelf& n1, TSelf& n2)
		{
			return
				n1._Object ?
				n2._Object ?
				*n1._Object == *n2._Object
				: false
				: n2._Object ?
				false
				: true;
		}
		const T& GetValueOrDefault(T& defaultValue)
		{
			return _Object ? *_Object : defaultValue;
		}
		bool HasValue() const
		{
			return _Object;
		}
		const T& Value() const
		{
			return *_Object;
		}
	};
}
#endif