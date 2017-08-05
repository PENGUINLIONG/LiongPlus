// File: YieldIterator.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include <memory>
#include <mutex>
#include <thread>
#include <vector>
#include "_"
#include "Interface.hpp"

_L_NS_BEG

template<typename T>
struct YielderSharedObject
{
public:
	bool IsFinished;
	std::mutex Lock;
	std::vector<T> YieldList;
};

template<typename T>
class YieldIterator
{
private:
	using shared_obj = std::shared_ptr<YielderSharedObject<int>>;

	shared_obj _Shared;
	typename std::vector<T>::iterator _Iter;

	YieldIterator(const shared_obj& shared, typename std::vector<T>::iterator iter) :
		_Shared(shared), _Iter(iter) {}

public:
	YieldIterator() = default;
	YieldIterator(const YieldIterator<T>&) = default;
	YieldIterator(YieldIterator<T>&&) = default;

	YieldIterator<T>& operator=(const YieldIterator<T>&) = default;
	YieldIterator<T>& operator=(YieldIterator<T>&&) = default;

	T& operator*() const
	{
		return *_Iter;
	}

	bool operator==(const YieldIterator<T>& other) const
	{
		return _Iter == other._Iter;
	}

	YieldIterator<T> operator++()
	{
		auto temp = _Iter; ++temp;
		for (;;)
		{
			if (temp != _Shared->YieldList.end())
				_Iter = temp;
			else if (_Shared->IsFinished)
				_Iter = _Shared->YieldList.end();
			else
				std::this_thread::yield();
		}
		
		return *this;
	}
	YieldIterator<T> operator++(int)
	{
		YieldIterator temp = *this;
		++(*this);
		return temp;
	}
};

/*
 * Yielder doesn't check whether multiple [YieldEnumerator]s are generated for efficiency.
 * Please share pointers after consideration.
 */
template<typename T>
class Yielder
{
private:
	using shared_obj = std::shared_ptr<YielderSharedObject<T>>;

	shared_obj _Shared;

public:
	void Finish()
	{
		std::lock_guard<std::mutex> lock(_Shared->Lock);

		_Shared->IsFinished = true;
	}
	YieldIterator<T> begin()
	{
		return YieldIterator<T>(_Shared, _Shared->YieldList.begin());
	}
	YieldIterator<T> end()
	{
		return YieldIterator<T>(_Shared, _Shared->YieldList.end());
	}
	void Add(T&& value)
	{
		std::lock_guard<std::mutex> lock(_Shared->Lock);

		_Shared->YieldQueue.push(std::forward<T>(value));
	}
};

_L_NS_END
