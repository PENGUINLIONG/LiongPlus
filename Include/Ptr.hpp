// File: Ptr.hpp
// Author: Rendong Liang (Liong)

#ifndef _L_Ptr
#define _L_Ptr
#include "Fundamental.hpp"
#include "Exception.hpp"
#include "ReferenceCounter.hpp"

namespace LiongPlus
{
	/// <summary>
	/// Smart pointer with reference count.
	/// </summary>
	/// <typeparam name="T">Type of object.</typeparam>
	template<typename T>
	class Ptr
		: public Object
	{
	private:
		typedef Ptr<T> TSelf;

		ReferenceCounter* _Counter;
		/// <summary>
		/// Target object.
		/// </summary>
		T* _Ptr;
		bool _ShouldDelete;
	public:
		Ptr(bool shouldDelete = true)
			: _Counter(nullptr)
			, _Ptr(nullptr)
			, _ShouldDelete(true)
		{
		}
		Ptr(const nullptr_t null, bool shouldDelete = true)
			: _Counter(nullptr)
			, _Ptr(nullptr)
			, _ShouldDelete(true)
		{
		}
		Ptr(T* ptr, bool shouldDelete = true)
			: _Counter(new ReferenceCounter)
			, _Ptr(ptr)
			, _ShouldDelete(shouldDelete)
		{
			_Counter->Inc();
		}
		Ptr(const TSelf& instance)
			: _Counter(instance._Counter)
			, _Ptr(instance._Ptr)
			, _ShouldDelete(instance._ShouldDelete)
		{
			if (_Counter != nullptr)
				_Counter->Inc();
		}
		Ptr(TSelf&& instance)
			: _Counter(nullptr)
			, _Ptr(nullptr)
			, _ShouldDelete(false)
		{
			Swap(_Counter, instance._Counter);
			Swap(_Ptr, instance._Ptr);
			Swap(_ShouldDelete, instance._ShouldDelete);
		}
		~Ptr()
		{
			Clear();
		}

		TSelf& operator=(const nullptr_t null)
		{
			Clear();
			return *this;
		}
		TSelf& operator=(T* ptr)
		{
			Clear();
			_Counter = new ReferenceCounter();
			_Counter->Inc();
			_Ptr = ptr;
			return *this;
		}
		TSelf& operator=(const TSelf& instance)
		{
			Clear();
			_Counter = instance._Counter;
			if (_Counter != nullptr)
				_Counter->Inc();
			_Ptr = instance._Ptr;
			return *this;
		}
		TSelf& operator=(TSelf&& instance)
		{
			Clear();
			_Counter = instance._Counter;
			_Counter->Inc();
			_Ptr = instance._Ptr;
			instance.Clear();
			return *this;
		}

		inline bool operator==(const nullptr_t null) const
		{
			return !_Ptr;
		}
		inline bool operator==(T* ptr) const
		{
			return _Ptr == ptr;
		}
		inline bool operator==(const TSelf& instance) const
		{
			return _Ptr == instance._Ptr;
		}
		inline bool operator!=(const nullptr_t null) const
		{
			return _Ptr != nullptr;
		}
		inline bool operator!=(T* ptr) const
		{
			return _Ptr != ptr;
		}
		inline bool operator!=(const TSelf& instance) const
		{
			return _Ptr != instance._Ptr;
		}
		inline T* operator->() const
		{
			if (!_Ptr)
				throw NullReferenceException("Cannot access to null pointer.");
			return _Ptr;
		}
		inline T& operator*() const
		{
			if (!_Ptr)
				throw NullReferenceException("Cannot access to null pointer.");
			return *_Ptr;
		}

		inline operator bool() const
		{
			return _Ptr != nullptr;
		}

		/// <summary>
		/// Clean up and decrease reference count of object.
		/// </summary>
		void Clear()
		{
			if (_Counter && !_Counter->Dec())
			{
				if (_Ptr && _ShouldDelete)
					delete _Ptr;
				delete _Counter;
			}
			_Ptr = nullptr;
			_Counter = nullptr;
		}

		/// <summary>
		/// Get count of references to the object pointed by this [int::Ptr].
		/// </summary>
		/// <returns>Reference count if this [int::Ptr] is pointing to something. Otherwise, 0 will be returned.</returns>
		int GetReferenceCount()
		{
			return _Counter ? _Counter->_RefCount : 0;
		}

		/// <summary>
		/// Get native C-style pointer to the object.
		/// </summary>
		/// <remarks>To prevent access violation, recreating/deleting/modifying the object outside [int::Ptr<T>] are not recommended.</remarks>
		/// <returns>Native C-style pointer to target object.</returns>
		T* GetNativePointer()
		{
			return _Ptr;
		}
	};
}
#endif