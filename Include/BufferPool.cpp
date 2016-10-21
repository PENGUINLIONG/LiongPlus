// File: BufferPool.cpp
// Author: Rendong Liang(Liong)
#include "BufferPool.hpp"

namespace LiongPlus
{
	using namespace std;


	// BufferPool::Unit


	BufferPool::Unit::Unit(const size_t size)
		: _Buffer(size), _RefCount(0)
	{ }


	// BufferPool::Ref


	BufferPool::Ref::Ref(shared_ptr<Unit> ref)
		: _Ref(move(ref))
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


	// BufferPool


	BufferPool::BufferPool(unsigned long poolId)
		: _PoolId(poolId)
		, _Lock()
		, _Pool()
	{ }

	unsigned long BufferPool::PoolId()
	{
		return _PoolId;
	}

	BufferPool::Ref BufferPool::AddOne(const size_t sizeOfBuffer)
	{
		if (sizeOfBuffer == 0)
			throw runtime_error("Cannot add 0-sized buffer into the pool.");
		lock_guard<recursive_mutex> lock(_Lock);

		_Pool.emplace_back(make_shared<BufferPool::Unit>(sizeOfBuffer));
		return _Pool.back();
	}

	size_t BufferPool::Add(const size_t sizeOfBuffer, const size_t num)
	{
		if ((sizeOfBuffer | num) == 0)
			return 0;

		size_t toAdd = num;
		lock_guard<recursive_mutex> lock(_Lock);

		while (toAdd-- > 0)
			try { _Pool.push_back(make_shared<BufferPool::Unit>(sizeOfBuffer)); }
		catch (...) { ++toAdd; break; }
		return num - toAdd;
	}

	BufferPool::Ref BufferPool::Get(size_t minSize)
	{
		lock_guard<recursive_mutex> lock(_Lock);
		auto pos = _Pool.begin(), end = _Pool.end();

		while (pos != end)
			if ((*pos)->_Buffer.Length() >= minSize &&
				(*pos)->_RefCount == 0)
				return *pos;
			else
				++pos;
		return AddOne(_MinimumSize > minSize ? _MinimumSize : minSize);
	}

	size_t BufferPool::RemoveIf(Predicate<const Buffer&> condition)
	{
		if (condition == nullptr) throw runtime_error("Null func.");

		size_t removed = 0;
		lock_guard<recursive_mutex> lock(_Lock);
		auto pos = _Pool.begin(), end = _Pool.end();

		while (pos != end)
			if (condition((*pos)->_Buffer)) _Pool.erase(pos++), ++removed;
			else ++pos;
			return removed;
	}

	size_t BufferPool::RemoveIdle()
	{
		size_t removed = 0;
		lock_guard<recursive_mutex> lock(_Lock);
		auto pos = _Pool.begin(), end = _Pool.end();

		while (pos != end)
			if ((*pos)->_RefCount == 0) _Pool.erase(pos++), ++removed;
			else ++pos;
			return removed;
	}

	// Static

	BufferPool& BufferPool::Ginst(unsigned long groupId)
	{
		lock_guard<mutex> lock(_GlobalLock);
		auto context = _ContextMap.find(groupId);
		if (context == _ContextMap.end())
			return *(_ContextMap.emplace(groupId, make_unique<BufferPool>(groupId)).first->second);
		else return *context->second;
	}

	void BufferPool::Release(unsigned long poolId)
	{
		_ContextMap.erase(poolId);
	}

	string BufferPool::Report()
	{
		stringstream ss;
		ss << _ContextMap.size() << " pools are allocated:" << endl;
		lock_guard<mutex> globalLock(_GlobalLock);
		for (auto& pr in _ContextMap)
		{
			lock_guard<recursive_mutex> lock(pr.second->_Lock);
			size_t idleCount = 0;
			for (auto& b in pr.second->_Pool)
				if (b->_RefCount == 0) ++idleCount;
			ss << "Pool #" << pr.first << ": " << pr.second->_Pool.size() << " (" << idleCount << " idle)" << endl;
		}
		return ss.str();
	}

	// Private
	// Static

	map<unsigned long, unique_ptr<BufferPool>> BufferPool::_ContextMap;
	mutex BufferPool::_GlobalLock;
}