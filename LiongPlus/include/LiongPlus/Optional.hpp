// File: Optional.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include <cstddef>
#include <utility>
#include "_"

_L_NS_BEG
/*
 * Help store optional obj which can be nullptr.
 *
 * Typeparams:
 *   T: Type of obj.
 */
template<typename T>
class Optional
{
private:
	typedef Optional<T> TSelf;
		
	static bool Equals(TSelf& n1, TSelf& n2)
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
	Optional() : _Object(nullptr) {}
	Optional(std::nullptr_t) : _Object(nullptr) {}
	Optional(const T& obj) : _Object(new T(obj)) {}
	Optional(T&& obj) : _Object(new T(std::forward<T>(obj))) {}
	Optional(const TSelf& nullable)
		: _Object(nullptr)
	{
		if (nullable._Object == nullptr)
		{
			CleanUp();
			_Object = nullptr;
		}
		else
		{
			if (_Object == nullptr) _Object = new T(*nullable._Object);
			else *_Object = *nullable._Object;
		}
	}
	Optional(TSelf&& nullable) : Optional() { std::swap(_Object, nullable._Object); }
	~Optional() { CleanUp(); }

	TSelf& operator=(std::nullptr_t) { CleanUp(); }
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
			*_Object = std::forward<T>(obj);
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
			if (_Object == nullptr) _Object = new T(*nullable._Object);
			else *_Object = *nullable._Object;
		}
		return *this;
	}
	TSelf& operator=(TSelf&& nullable)
	{
		std::swap(_Object, nullable._Object);
		return *this;
	}

	bool operator==(const TSelf& nullable) const { return Equals(*this, nullable); }
	bool operator==(const std::nullptr_t&) const { return _Object == nullptr; }
	bool operator!() const { return _Object == nullptr; }
	bool operator!=(const TSelf& nullable) const { return !Equals(*this, nullable); }

	operator bool() const { return _Object != nullptr; }

	void CleanUp()
	{
		if (_Object != nullptr)
		{
			delete _Object;
			_Object = nullptr;
		}
	}

	T& GetValueOrDefault(T& defaultValue) const { return _Object != nullptr ? *_Object : defaultValue; }
	bool HasValue() const { return _Object != nullptr; }
	T& Value() const { return *_Object; }
};

_L_NS_END
