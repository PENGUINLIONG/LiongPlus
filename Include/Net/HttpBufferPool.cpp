// File: HttpBufferPool.cpp
// Author: Rendong Liang (Liong)
#include "HttpBufferPool.hpp"

namespace LiongPlus
{
	namespace Net
	{
		using namespace std;

		vector<HttpBufferPool::Unit> HttpBufferPool::_Pool;

		HttpBufferPool::Unit::Unit()
			: Ptr(nullptr)
			, IsOccupied(false)
			, Lock()
		{
		}
		HttpBufferPool::Unit::Unit(const Unit& instance)
			: Ptr(instance.Ptr)
			, IsOccupied(instance.IsOccupied)
			, Lock()
		{
		}
		HttpBufferPool::Unit::Unit(Unit&& instance)
			: Unit()
		{
			using std::swap;

			swap(Ptr, instance.Ptr);
			swap(IsOccupied, instance.IsOccupied);
		}
		HttpBufferPool::Unit::~Unit()
		{
			Lock.lock();
			IsOccupied = true;
			if (Ptr != nullptr)
			{
				delete[] Ptr;
				Ptr = nullptr;
			}
			Lock.unlock();
		}

		HttpBufferToken HttpBufferPool::Apply()
		{
			for (;;)
			{
				for (size_t pos = 0; pos < _Pool.size(); ++pos)
				{
					Unit& unit = _Pool[pos];
					unit.Lock.lock();
					if (!unit.IsOccupied)
					{
						unit.IsOccupied = true;
						unit.Lock.unlock();
						return pos;
					}
					unit.Lock.unlock();
				}
				Unit newUnit;
				newUnit.Ptr = new Buffer(64 * 1024);
				newUnit.IsOccupied = false;

				_Pool.push_back(move(newUnit));
			}
		}

		Buffer& HttpBufferPool::Fetch(HttpBufferToken token)
		{
			return *_Pool[token].Ptr;
		}

		void HttpBufferPool::Release(HttpBufferToken token)
		{
			Unit& unit = _Pool[token];
			unit.Lock.lock();
			unit.IsOccupied = false;
			unit.Lock.unlock();
		}
	}
}
