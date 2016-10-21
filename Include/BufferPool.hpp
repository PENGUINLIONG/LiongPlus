// File: BufferPool.hpp
// Author: Rendong Liang(Liong)
#pragma once
#include "Fundamental.hpp"
#include "Buffer.hpp"

namespace LiongPlus
{
	namespace IO
	{
		/*
		 * This class privide a buffer pool for each thread.
		 * Usage:
		 *   The main purpose of this class is to provide a central control of large memory chunks.
		 *   Pools are also grouped by an ID, so users can neatly divide the buffers into groups.
		 * Warning:
		 *   Each allocation costs time, so users should prevent frequent allocation and removal of buffers.
		 *   This class is helpful when:
		 *     1. Similar works are done repetatively within a scope,
		 *        which means buffers will be allocated and released frequently.
		 *     2. Those works need large chunk of memory as buffer, e.g. image processing and data frames.
		 *     3. Those works are usually thread-independent,
		 *        which means any threads other than the worker thread itself will not access to the buffer.
		 *   Although the implementation does not limit you from inter-thread memory sharing,
		 *   it is thread-unsafe and is not recommendded to do so.
		 */
		class BufferPool
		{
		private:
			class Unit
			{
				friend class BufferPool;
			private:
				Buffer _Buffer;
				std::atomic<unsigned long> _RefCount;
			public:
				Unit(const size_t size);
				Unit(const Unit&) = default;
				Unit(Unit&&) = default;

				Unit& operator=(const Unit&) = default;
				Unit& operator=(Unit&&) = default;
			};

			unsigned long _PoolId;
			std::recursive_mutex _Lock;
			std::list<std::shared_ptr<Unit>> _Pool;

			static const size_t _MinimumSize = 512 * 1024;

			static std::map<unsigned long, std::unique_ptr<BufferPool>> _ContextMap;
			static std::mutex _GlobalLock;
		public:
			/*
			 * Manage the idleness of a buffer.
			 */
			class Ref
			{
				friend class BufferPool;
			private:
				std::shared_ptr<Unit> _Ref;

				Ref(std::shared_ptr<Unit>);

			public:
				Ref(const Ref&);
				Ref(Ref&&) = default;
				~Ref();

				Ref& operator=(const Ref&);
				Ref& operator=(Ref&&) = default;

				Buffer& operator*();
				Buffer* operator->();
			};

			BufferPool() = delete;
			BufferPool(unsigned long poolId);
			BufferPool(const BufferPool&) = default;
			BufferPool(BufferPool&&) = default;
			~BufferPool() = default;

			BufferPool& operator=(const BufferPool&) = default;
			BufferPool& operator=(BufferPool&&) = default;

			unsigned long PoolId();

			/*
			 * Add a single buffer to the pool.
			 * Params:
			 *   $sizeOfBuffer: The exact size of buffer to be added.
			 * Returns:
			 *   The reference class of 
			 */
			Ref AddOne(const size_t sizeOfBuffer);
			/*
			 * Add buffer(s) to the pool.
			 * Params:
			 *   $sizeOfBuffer: The exact size of buffer(s) to be added.
			 *   $num: Number of such buffers to be added.
			 * Return:
			 *   Number of buffers successfully added.
			 */
			size_t Add(const size_t sizeOfBuffer, const size_t num = 1);
			/*
			 * Get a buffer sized larger than $minSize. If there is no idle buffer available, a new buffer will be added to the pool.
			 * Params:
			 *   $minSize: The minimum size of a qualified buffer.
			 * Returns:
			 *   An idle buffer of size equals to or larger than $minSize.
			 * Note:
			 *   If a new buffer is needed to be added and $minSize is smaller than $_MinimumSize, the buffer size will be $_MinimumSize.
			 */
			Ref Get(const size_t minSize = _MinimumSize);
			/*
			 * Remove the buffers satisfying $condition.
			 * Params:
			 *   $condition: The condition determines whether a buffer should be removed.
			 * Returns:
			 *   Number of buffers successfully removed.
			 */
			size_t RemoveIf(Predicate<const Buffer&> condition);
			/*
			 * Remove all idle buffers.
			 * Returns:
			 *   Number of buffers successfully removed.
			 */
			size_t RemoveIdle();

			/*
			 * Get an instance of [LiongPlus::IO::BufferPool].
			 * Returns:
			 *   The reference of instance allocated restrictively for current thread.
			 */
			static BufferPool& Ginst(unsigned long poolId = 0);
			/*
			 * Release the current instance.
			 */
			static void Release(unsigned long poolId);
			/*
			 * Print a report according to current state of all the pools.
			 * Returns:
			 *   String of state report.
			 * Warning:
			 *   This method will inquire every pools which are used or to be used by another thread, which means serious blockage of threads will occur.
			 *   Thus it should never be used in any other cases than debugging.
			 */
			static std::string Report();
		};
	}
}
