// File: Nullable.hpp
// Author: Rendong Liang (Liong)

#ifndef _L_Nullable
#define _L_Nullable
#include "Fundamental.hpp"

namespace LiongPlus
{
	/// <summary>
	/// Help store optional obj which can be nullptr.
	/// </summary>
	/// <typeparam name="T">Type of obj</typeparam>
	template<typename T>
	class Nullable
	{
	private:
		typedef Nullable<T> TSelf;
		
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
	protected:
		T* _Object;
	public:
		Nullable()
			: _Object(nullptr)
		{
		}
		Nullable(nullptr_t)
			: _Object(nullptr)
		{
		}
		Nullable(const T& obj)
			: _Object(new T(obj))
		{
		}
		Nullable(T&& obj)
			: Nullable()
		{
			_Object = new T(std::forward<T>(obj));
		}
		Nullable(const TSelf& nullable)
			: _Object(nullptr)
		{
			if (nullable._Object == nullptr)
			{
				CleanUp();
				_Object = nullptr;
			}
			else
			{
				if (_Object == nullptr)
					_Object = new T(*nullable._Object);
				else
					*_Object = *nullable._Object;
			}
		}
		Nullable(TSelf&& nullable)
			: Nullable()
		{
			swap(_Object, nullable._Object);
		}
		~Nullable()
		{
			CleanUp();
		}

		TSelf& operator=(nullptr_t)
		{
			CleanUp();
		}
		TSelf& operator=(const T& obj)
		{
			if (_Object == nullptr)
				_Object = new T(obj);
			else
				*_Object = obj;
			return *this;
		}
		TSelf& operator=(T&& obj)
		{
			if (_Object == nullptr)
				_Object = new T(std::forward<T>(obj));
			else
				*_Object = obj;
			return *this;
		}
		TSelf& operator=(const TSelf& nullable)
		{
			if (nullable._Object == nullptr)
			{
				CleanUp();
				_Object = nullptr;
			}
			else
			{
				if (_Object == nullptr)
					_Object = new T(*nullable._Object);
				else
					*_Object = *nullable._Object;
			}
			return *this;
		}
		TSelf& operator=(TSelf&& nullable)
		{
			swap(_Object, nullable._Object);
			return *this;
		}

		bool operator==(const TSelf& nullable) const
		{
			return Equals(*this, nullable);
		}
		bool operator==(const nullptr_t nullptr) const
		{
			return _Object == nullptr;
		}
		bool operator!() const
		{
			return _Object == nullptr;
		}
		bool operator!=(const TSelf& nullable) const
		{
			return !Equals(*this, nullable);
		}

		operator bool() const
		{
			return _Object != nullptr;
		}

		void CleanUp()
		{
			if (_Object != nullptr)
			{
				delete _Object;
				_Object = nullptr;
			}
		}

		T& GetValueOrDefault(T& defaultValue) const
		{
			return _Object != nullptr ? *_Object : defaultValue;
		}
		bool HasValue() const
		{
			return _Object != nullptr;
		}
		T& Value() const
		{
			return *_Object;
		}
	};
}
#endif