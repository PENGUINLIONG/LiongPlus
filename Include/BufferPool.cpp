// File: BufferPool.cpp
// Author: Rendong Liang(Liong)

#include "BufferPool.hpp"
#include <sstream>

namespace LiongPlus
{
	using namespace std;


	// BufferPool::Unit


	BufferPool::Unit::Unit(const size_t size)
		: _Buffer(size), _RefCount(0)
	{ }


	// BufferPool::Ref


	BufferPool::Ref::Ref(const shared_ptr<Unit>& ref)
		: _Ref(ref)
	{
		++_Ref->_RefCount;
	}
	BufferPool::Ref::Ref(const Ref& instance)
		: _Ref(instance._Ref)
	{
		++_Ref->_RefCount;
	}
	BufferPool::Ref::~Ref()
	{
		--_Ref->_RefCount;
	}

	BufferPool::Ref& BufferPool::Ref::operator=(const BufferPool::Ref& instance)
	{
		++instance._Ref->_RefCount; return *this;
	}

	Buffer& BufferPool::Ref::operator*()
	{
		return _Ref->_Buffer;
	}
	Buffer* BufferPool::Ref::operator->()
	{
		return &_Ref->_Buffer;
	}

	//
	// BufferPool
	//

	void BufferPool::_UnsafeAddOne(const size_t sizeOfBuffer)
	{
		if (nullptr == _Pool)
		{
			_Pool = new UnitChainNode{ make_shared<BufferPool::Unit>(sizeOfBuffer), nullptr };
			_Pool->Next = _Pool;
		}
		else
			_Pool->Next = new UnitChainNode{ make_shared<BufferPool::Unit>(sizeOfBuffer), _Pool->Next };
	}

	BufferPool::BufferPool(unsigned long poolId) : _Lock(), _Pool(nullptr) { }

	BufferPool::Ref BufferPool::AddOne(const size_t sizeOfBuffer)
	{
		if (sizeOfBuffer == 0)
			throw length_error("Cannot add 0-sized buffer into the pool.");

		lock_guard<recursive_mutex> lock(_Lock);
		_UnsafeAddOne(sizeOfBuffer);

		return _Pool->Next->Unit;
	}

	size_t BufferPool::Add(const size_t sizeOfBuffer, const size_t num)
	{
		if ((sizeOfBuffer | num) == 0)
			return 0;

		size_t toAdd = num;
		lock_guard<recursive_mutex> lock(_Lock);

		while (toAdd-- > 0)
		{
			try
			{
				_UnsafeAddOne(sizeOfBuffer);
			}
			catch (...) { ++toAdd; break; }
		}
		return num - toAdd;
	}

	BufferPool::Ref BufferPool::Get(size_t minSize)
	{
		lock_guard<recursive_mutex> lock(_Lock);
		if (nullptr == _Pool) _UnsafeAddOne(_MinimumSize);

		auto pos = _Pool, end = _Pool;

		do
		{
			if (pos->Unit->_Buffer.Length() >= minSize &&
				pos->Unit->_RefCount == 0)
				return pos->Unit;
			else
				pos = pos->Next;
		} while (pos != end);
		return AddOne(_MinimumSize > minSize ? _MinimumSize : minSize);
	}

	size_t BufferPool::RemoveIf(Predicate<const Buffer&> condition)
	{
		if (condition == nullptr) throw invalid_argument("Null func.");

		size_t removed = 0;
		lock_guard<recursive_mutex> lock(_Lock);
		auto pos = _Pool, end = _Pool;

		do
		{
			if (condition(pos->Next->Unit->_Buffer))
			{
				auto temp = pos->Next->Next;
				delete pos->Next;
				pos->Next = temp;
				++removed;
				// $end is removed, break to prevent another round of looping.
				if (temp == end) break;
			}
			else pos = pos->Next;
		} while (pos != end);
		return removed;
	}

	size_t BufferPool::RemoveIdle()
	{
		size_t removed = 0;
		lock_guard<recursive_mutex> lock(_Lock);
		auto pos = _Pool, end = _Pool;

		do
		{
			if (0 == pos->Next->Unit->_RefCount)
			{
				auto temp = pos->Next->Next;
				delete pos->Next;
				pos->Next = temp;
				++removed;
				if (temp == end) break;
			}
			else pos = pos->Next;
		} while (pos != end);
		return removed;
	}

	// Static

	BufferPool& BufferPool::Ginst(unsigned long groupId)
	{
		lock_guard<mutex> lock(s_GlobalLock);
		if (t_CurrentPool == nullptr)
			return *(t_CurrentPool = make_unique<BufferPool>(groupId));
		else return *t_CurrentPool;
	}

	// Private
	// Static

	thread_local unique_ptr<BufferPool> BufferPool::t_CurrentPool;
	mutex BufferPool::s_GlobalLock;
}